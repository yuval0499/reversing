#include <stdio.h>
#include <sys/types.h>
#include <dlfcn.h>

typedef unsigned char uint8_t;
static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abc!d@#$%^&*()?efghijklmnopqrstuvwxyzABCDEFGHIJK...";
size_t n;
    if (size)
    {
        --size;
        for (n = 0; n < size; n++) 
	{
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

typedef void (*enc_func)(uint8_t*,uint8_t*,uint8_t*);
int main (int argc, const char* argv[])
{
	void* dl_handler;
	enc_func func;
	uint8_t i;

	srand(time(NULL));
	if ((dl_handler = dlopen("aes.so", RTLD_LAZY)) == NULL)
		return -1;

	if ((func = (enc_func)dlsym(dl_handler, "AES128_ECB_encrypt")) == NULL)
		goto exit;

	uint8_t key[16];
	for (i = 0; i < 16; ++i)
		key[i] = rand() % 256;

	char pw[16];
	uint8_t output[16];
	rand_string(pw, sizeof(pw));
	func(pw, key, output);

	for (i = 0; i < sizeof(key); ++i)
		printf("%x ", key[i]);
	printf("\n");
	for (i = 0; i < sizeof(output); ++i)
		printf("%x ", output[i]);
	printf("\n");

	exit:
	dlclose(dl_handler);
	return 0;
}
