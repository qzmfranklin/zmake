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
 * 	chat_client.exe [domain_name|ip_address] port
 *
 * IP address can be either ipv4 or ipv6.
 * Built-in timeout is 5s. 
 * Type 'quit' to terminate.
 */

#include <pthread.h>
#include "../inet_utils.h"

static void perr(const char * msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(1);
}

struct client_mgr_t {
	int fd;
	double timeout_sec;
};

void *client_sendmgr(void *args)
{
	fprintf(stderr,"sendmgr starts\n");

	int    fd  =((struct client_mgr_t*)args)->fd;
	double sec =((struct client_mgr_t*)args)->timeout_sec;
	int    timeout = sec>=0.0?(int)(sec*1000):-1;

	char buf[BUFSIZ];
	buf[0]=0;

	// poll the stdin for user input
	struct pollfd ufds;
	ufds.fd     = STDIN_FILENO;
	ufds.events = POLLIN;

	//fprintf(stderr,"local>");
	while(1) {
		int rval=poll(&ufds,1,-1);
		if (rval==-1) {
			perror("sendmgr: poll");
			break;
		}

		int len = read(STDIN_FILENO,buf,BUFSIZ-1);
		buf[len-1]=0;

		if (!strcmp(buf,"quit")) exit(1);

		write(fd,buf,len-1);
		//fprintf(stderr,"local>");
	}

	fprintf(stderr,"sendmgr ends\n");
	return NULL;
}

void *client_recvmgr(void *args)
{
	fprintf(stderr,"recvmgr starts\n");

	int    fd  =((struct client_mgr_t*)args)->fd;
	double sec =((struct client_mgr_t*)args)->timeout_sec;
	int    timeout = sec>=0.0?(int)(sec*1000):-1;

	char   buf[BUFSIZ];
	const size_t BLKSZ=128;

	// poll the socket fd for incoming message
	struct pollfd ufds;
	ufds.fd     = fd;
	ufds.events = POLLIN;

	while(1) {
		int rval = poll(&ufds,1,timeout);
		if (rval==-1) {
			perror("recvmgr: poll");
			break;
		} else if (rval==0) {
			fprintf(stderr,"Timeout occured after %fs\n",sec);
			exit(1);
		}
		if (pollerr(ufds.revents)) exit(1);
		int bytesread = read(fd,buf,BLKSZ-1);
		buf[bytesread]=0;
		//fprintf(stderr,"%s",buf);
		printf("%s\n",buf);
	}
	fprintf(stderr,"recvmgr ends\n");
	return NULL;
}

int main(int argc, char const* argv[])
{
	if (argc<3) perr("Usage: client.exe [address] [port]");

	struct addrinfo *plist = 
		get_addrinfo_list(argv[1],argv[2],SOCK_STREAM);
	int fd = try_connect(plist);
	if (fd==-1) perr("Cannot connect to host");

	freeaddrinfo(plist);

	fprintf(stderr,"Enter \"quit\" to terminate process\n");

	struct client_mgr_t args1={fd,300.0}, args2={fd,-1.0};
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,client_recvmgr,&args1);
	pthread_create(&tid2,NULL,client_sendmgr,&args2);
	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);

	close(fd);
	fprintf(stderr,"Disconnected\n");
	return 0;
}
