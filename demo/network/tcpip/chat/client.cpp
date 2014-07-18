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
