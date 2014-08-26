#include "inet_utils.h"

int main(int argc, char const* argv[])
{
	if (argc<2) {
		perror("Usage: showip.exe [host] [port=0]");
		exit(1);
	}

	struct addrinfo *plist;
	if (argc==2)
		plist = get_addrinfo_list(argv[1],"0",SOCK_STREAM);
	else
		plist = get_addrinfo_list(argv[1],argv[2],SOCK_STREAM);
	printf("\t\t%s\n",argv[1]);
	print_addrinfo_list(plist);

	return 0;
}
