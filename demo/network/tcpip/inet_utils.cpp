#include "inet_utils.h"

void *get_sockaddr_in(const void *addr)
{
	struct sockaddr *p = (struct sockaddr*)addr;
	switch (p->sa_family) {
	case AF_INET:
		return &(((struct sockaddr_in  *)p)->sin_addr);
	case AF_INET6:
		return &(((struct sockaddr_in6 *)p)->sin6_addr);
	default:
		return NULL;
	}
}

int get_sockaddr_strlen(const void *addr)
{
	struct sockaddr *p = (struct sockaddr*)addr;
	switch (p->sa_family) {
	case AF_INET:
		return INET_ADDRSTRLEN;
	case AF_INET6:
		return INET6_ADDRSTRLEN;
	default:
		return -1;
	}
}

int get_sockaddr_str(const void *addr, char *addrstr)
{
	struct sockaddr *p = (struct sockaddr*)addr;
	int addrstr_len = get_sockaddr_strlen(addr);
	if (addrstr_len==-1) {
		perror("get_sockaddr_str: unexpectied value of addrstr_len");
		exit(1);
	}
	inet_ntop(p->sa_family,get_sockaddr_in(p),addrstr,addrstr_len);
	return addrstr_len;
}

struct addrinfo *get_addrinfo_list( 
		const char *addr, 
		const char *port,
		const int socktype)
{
	struct addrinfo hint;
	memset(&hint,0,sizeof(hint));
	hint.ai_family   = AF_UNSPEC;
	hint.ai_socktype = socktype;
	struct addrinfo *res;
	int rval = getaddrinfo(addr,port,&hint,&res);
	if (rval) {
		perror("get_addrinfo_list: getaddrinfo");
		exit(1);
	}
	return res;
}

struct addrinfo *get_addrinfo_list_server( 
		const char *port,
		const int socktype)
{
	struct addrinfo hint;
	memset(&hint,0,sizeof(hint));
	hint.ai_family   = AF_UNSPEC;
	hint.ai_socktype = socktype;
	hint.ai_flags    = AI_PASSIVE; // passive open
	struct addrinfo *res;
	int rval = getaddrinfo(NULL,port,&hint,&res);
	if (rval) {
		perror("get_addrinfo_list_server: getaddrinfo");
		exit(1);
	}
	return res;
}


static struct sockaddr_in *ipv4;
static struct sockaddr_in6 *ipv6;

void print_sockaddr(const struct sockaddr *p)
{
	void *addr;
	int  port;
	char addrstr[INET6_ADDRSTRLEN];
	switch (p->sa_family) {
	case AF_INET:
		ipv4 = (struct sockaddr_in *)p;
		addr = &(ipv4->sin_addr);
		inet_ntop(AF_INET,addr,addrstr,INET_ADDRSTRLEN);
		port = ipv4->sin_port;
		break;
	case AF_INET6:
		ipv6 = (struct sockaddr_in6 *)p;
		addr = &(ipv6->sin6_addr);
		inet_ntop(AF_INET6,addr,addrstr,INET6_ADDRSTRLEN);
		port = ipv6->sin6_port;
		break;
	default:
		fprintf(stderr,"print_sockaddr: Invalid ip address family, abort\n");
		fprintf(stderr,"sa_family = %d\n",p->sa_family);
		fprintf(stderr,"AF_INET   = %d\n",AF_INET);
		fprintf(stderr,"AF_INET6  = %d\n",AF_INET6);
		exit(1);
		break;
	}
	printf("%s.%d\n",addrstr,port);
}

void print_addrinfo(const struct addrinfo *p)
{
	print_sockaddr(p->ai_addr);
}

void print_addrinfo_list(const struct addrinfo *list)
{
	while (list!=NULL) {
		fprintf(stderr,"\t");
		print_addrinfo(list);
		list = list->ai_next;
	}
}

int try_connect(struct addrinfo *plist)
{
	int fd;
	struct addrinfo *p;
	for(p=plist; p!=NULL; p=p->ai_next) {
		fprintf(stderr,"Connecting to:    ");
		print_addrinfo(p);
		fd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if (fd==-1) { 
			perror("try_connect: socket"); 
			continue; 
		}
		int err = connect(fd,p->ai_addr,p->ai_addrlen);
		if (err==-1) {
			perror("try_connect: connect");
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

int try_bind(struct addrinfo *plist)
{
	int fd;
	struct addrinfo *p;
	for(p=plist; p!=NULL; p=p->ai_next) {
		fd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if (fd==-1) {
			perror("try_bind: socket");
			continue;
		}
		int yes=1;
		if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,
					&yes,sizeof(int))==-1) {
			perror("try_bind: setsockopt");
			continue;
			//exit(1);
		}
		if (bind(fd,p->ai_addr,p->ai_addrlen)==-1) {
			close(fd);
			perror("try_bind: bind");
			continue;
		}
		break;
	}
	if (p==NULL) {
		fd=-1;
		fprintf(stderr,"try_bind: fail\n");
	} else 
		fprintf(stderr,"try_bind: success\n");
	return fd;
}

int pollerr(const int revents)
{
	int err=-1;
	if (revents & POLLHUP)
		fprintf(stderr,"pollerr: Remote connection closed\n");
	else if (revents & POLLERR)
		fprintf(stderr,"pollerr: Socket error\n");
	else if (revents & POLLNVAL)
		fprintf(stderr,"pollerr: Invalid fd\n");
	else
		err=0;
	return err;
}
