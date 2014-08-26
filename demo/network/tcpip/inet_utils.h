#ifndef _INET_UTILS_H_
#define _INET_UTILS_H_

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
#include <netinet/in.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * *addr is one of : struct sockaddr*, sockaddr6, and sockaddr_storage*
 * return addr->sin_addr or sin6_addr depending on sa_family
 */
void *get_sockaddr_in(const void *addr);

/*
 * return INET_ADDRSTRLEN or INET6_ADDRSTRLEN depending on sa_family
 */
int get_sockaddr_strlen(const void *addr);

/*
 * put ascii representation of ip address in *addrstr
 * return the addstr_len
 */
int get_sockaddr_str(const void *addr, char *addrstr);


/*
 * Copied from Divakar Viswanath, added port
 */
struct addrinfo *get_addrinfo_list(
		const char *addr, 
		const char *port,
		const int socktype);

struct addrinfo *get_addrinfo_list_server(
		const char *port,
		const int socktype);

/*
 * Print ipv4/6 address
 */
void print_sockaddr(const struct sockaddr *p);
void print_addrinfo(const struct addrinfo *p);
void print_addrinfo_list(const struct addrinfo *p);

/*
 * Try connecting until the first success, return -1 upon failure
 */
int try_connect(struct addrinfo *plist);

/*
 * Try binding to port until the first success, return -1 upon failure
 */
int try_bind(struct addrinfo *plist);

/*
 * Print to stderr error message for poll revents, return 0 if no error
 */
int pollerr(const int revents);

#ifdef __cplusplus
}
#endif

#endif
