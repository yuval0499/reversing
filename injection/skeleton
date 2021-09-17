#include <stdio.h>
#include <stdarg.h>
#include <dlfcn.h>

#define OBJ_PATH "/lib/x86_64-linux-gnu/libc.so.6"
#define HIDDEN_FILE ".hi"

typedef int (*sym)(const char *, ...);
void* handle;
FILE* fd;
static void myinit() __attribute__((constructor));
static void mydest() __attribute__((destructor));

void myinit() 
{
}

void mydest() 
{
}

int printf ( const char * format, ... )
{
	// TODO
}
