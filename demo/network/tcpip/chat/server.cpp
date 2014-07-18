#include "../inet_utils.h"
#include <signal.h>
#define BACKLOG 3

static void perr(const char * msg)
{
	fprintf(stderr,"%s\n",msg);
	exit(1);
}

static void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

static void *server_recvmgr(void *args)
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
			break;
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
	if (argc<2) perr("usage: server.exe [port]");

	struct addrinfo *plist = 
		get_addrinfo_list("localhost",argv[1],SOCK_STREAM);
	int fd = try_bind(plist);
	if (fd==-1) perr("Cannot bind socket to port");

	freeaddrinfo(plist);

	if (listen(fd,BACKLOG)==-1) {
		perror("listen");
		exit(1);
	}

	// reap dead children
	struct sigaction sa;
	sa.sa_handler = sigchld_handler; 
	sa.sa_flags   = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	if (sigaction(SIGCHLD,&sa,NULL)==-1) {
		perror("sigaction");
		exit(1);
	}

	while(1) {
		struct sockaddr_storage addr;
		socklen_t addr_len;
		//memset(&addr,0,sizeof(addr));
		int client_fd = accept(fd,(struct sockaddr *)&addr,&addr_len);
		if (fd==-1) {
			perror("accept");
			exit(1);
		}
		fprintf(stderr,"Got connection from\n");
		fprintf(stderr,"helloe\n");
		print_sockaddr((struct sockaddr*)&addr);
		fprintf(stderr,"\n");

		int pid=fork();
		if (pid<0) {
			perror("fork");
			exit(1);
		} else if (pid==0) { 
			// child
			close(fd);
			const char welcome[] = "Welcome!";
			//write(client_fd,welcome,sizeof(welcome));
			struct client_mgr_t args={client_fd,60.0};
			server_recvmgr(&args);

			close(client_fd);
			_exit(0);
		}

		// parent
		close(client_fd);
	}

	return 0;
}
