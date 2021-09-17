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
	fd = fopen(HIDDEN_FILE,"w");
    handle = dlopen(OBJ_PATH,RTLD_LAZY);
}

void mydest() 
{
    dlclose(handle);
    fclose(fd);
}

int printf ( const char * format, ... )
{
	char* itr = format;
	unsigned int val_hex;
	int has_val = 0;
	sym orig_printf;

	orig_printf = (sym)dlsym(handle,"printf");
	va_list arg;
	va_start(arg, format);

	while(*itr != '\0')
	{
		if(*itr == '%')
		{
			switch(*(++itr))
            {
				case 0:
					--itr;
					break;
				case 'x':
					has_val = 1;
					val_hex = va_arg(arg,unsigned int);
					fprintf(fd, "%x ",val_hex);
					break;
			}
		}
		++itr;
	}

	if(has_val)
	{
		return orig_printf(format,val_hex);
	}
	return orig_printf(format);
}
