/*
 * Zhongming Qu, 2014 Jun
 */

/*
 * Code a telnet-like utility for:
 * 	1.	establishing tcp connection to a remote host identified by the 
 * 		<dst,port> pair,
 * 	2.	send text to the remote host, and
 * 	3.	receive message from the remote host.
 *
 * Usage:
 * 	ipmsg [domain_name|ip_address] port
 *
 * IP address can be either ipv4 or ipv6.
 * Built-in timeout is 5s. 
 * Type 'quit' to terminate.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>

static void perr(const char * msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(1);
}

static struct addrinfo *get_ipaddrinfo_list(const char *addr, const char *port,
		const int socktype)
{
/*
 * Copied from Divakar Viswanath, added port
 */
	struct addrinfo hint;
	hint.ai_family   = AF_UNSPEC;
	hint.ai_socktype = socktype;
	struct addrinfo *res;
	int rval = getaddrinfo(addr,port,&hint,&res);
	if (rval) {
		perror("getaddrinfo");
		exit(1);
	}
	return res;
}

static void print_addrinfo(const struct addrinfo *p)
{
	char ipver[200];
	void * addr;
	int port;
	char addrstr[INET6_ADDRSTRLEN];
	switch (p->ai_family) {
	case AF_INET:
		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		addr = &(ipv4->sin_addr);
		inet_ntop(AF_INET,addr,addrstr,INET_ADDRSTRLEN);
		strcpy(ipver,"IPV4");
		port = ipv4->sin_port;
		break;
	case AF_INET6:
		struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
		addr = &(ipv6->sin6_addr);
		inet_ntop(AF_INET6,addr,addrstr,INET6_ADDRSTRLEN);
		strcpy(ipver,"IPV6");
		port = ipv6->sin6_port;
		break;
	default:
		fprintf(stderr,"invalid ip address family, abort\n");
		exit(1);
		break;
	}
	printf("%s[%5d]: %s\n",ipver,port,addrstr);
}

/*
 * Try connecting until the first success, return -1 upon failure
 */
static int try_connect(struct addrinfo *plist)
{
	int fd;
	struct addrinfo *p;
	for(p=plist; p!=NULL; p=p->ai_next) {
		fprintf(stderr,"Connecting to:    ");
		print_addrinfo(p);
		fd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if (fd==-1) { 
			perror("socket"); 
			continue; 
		}
		int err = connect(fd,p->ai_addr,p->ai_addrlen);
		if (err==-1) {
			perror("connect");
			continue;
		}
		break; // if reach here, must have succeded
	}
	if (p==NULL) 
		fd=-1;
	else 
		fprintf(stderr,"Connected\n");
	return fd;
}

static int pollerr(const int revents)
{
	int err=-1;
	if (revents & POLLHUP)
		fprintf(stderr,"Remote connection closed\n");
	else if (revents & POLLERR)
		fprintf(stderr,"Socket error\n");
	else if (revents & POLLNVAL)
		fprintf(stderr,"Invalid fd\n");
	else
		err=0;
	return err;
}
static void *sendmgr(void *args)
{
	fprintf(stderr,"sendmgr starts\n");
	int fd=*(int*)args;
	char buf[BUFSIZ];
	buf[0]=0;

	// poll the stdin for user input
	struct pollfd ufds;
	ufds.fd     = STDIN_FILENO;
	ufds.events = POLLIN;

	// timeout after 5min
	fprintf(stderr,"local>");
	while(1) {
		int rval=poll(&ufds,1,500000);
		if (rval==-1) {
			perror("poll (sendmgr)");
			break;
		} else if (rval==0) {
			fprintf(stderr,"Timeout occured after 5min\n");
			return NULL;
		}

		int len = read(STDIN_FILENO,buf,BUFSIZ-1);
		buf[len-1]=0;

		/*
		 *fprintf(stderr,"echo :%s[end]\n",buf);
		 *for (int i = 0; i < len; i++)
		 *        fprintf(stderr,"%d ",buf[i]);
		 *fprintf(stderr,"\n");
		 */

		if (!strcmp(buf,"quit")) {
			fprintf(stderr,"Quit\n");
			exit(1);
		}

		write(fd,buf,len-1);
		fprintf(stderr,"local>");
		//printf("local>");
	}

	fprintf(stderr,"sendmgr ends\n");
	return NULL;
}

static void *recvmgr(void *args)
{
	fprintf(stderr,"recvmgr starts\n");
	int fd=*(int*)args;
	char buf[BUFSIZ];
	const size_t BLKSZ=128;

	// poll the socket fd for incoming message
	struct pollfd ufds;
	ufds.fd     = fd;
	ufds.events = POLLIN;

	while(1) {
		int rval = poll(&ufds,1,300000);
		if (rval==-1) {
			perror("poll (recvmgr)");
			break;
		} else if (rval==0) {
			fprintf(stderr,"Timeout occured after 5min\n");
			exit(1);
		}
		if (pollerr(ufds.revents)) exit(1);
		int bytesread = read(fd,buf,BLKSZ);
		buf[bytesread]=0;
		//fprintf(stderr,"%s",buf);
		printf("%s\n",buf);
	}
	fprintf(stderr,"recvmgr ends\n");
	return NULL;
}

int main(int argc, char const* argv[])
{
	if (argc<3) perr("usage: ipmsg [address] [port]");

	struct addrinfo *plist = 
		get_ipaddrinfo_list(argv[1],argv[2],SOCK_STREAM);
	int fd = try_connect(plist);
	if (fd==-1) perr("cannot connect to host");

	freeaddrinfo(plist);

	fprintf(stderr,"Enter \"quit\" to terminate process\n");

	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,recvmgr,&fd);
	pthread_create(&tid2,NULL,sendmgr,&fd);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	close(fd);
	fprintf(stderr,"Disconnected\n");
	return 0;
}
