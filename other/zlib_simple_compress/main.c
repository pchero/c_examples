// gcc -o main -lz ./main.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static char* uncompress_data(char* data, uLong len);
static char* compress_data(char* data, int len, uLong *outLen);


int main(int argc, char** argv)
{
    char* comp_buf;
    uLong comp_len;
    int ret;
    int i;

    if(argc < 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 0;
    }
    printf("hello world!\n");

    // compress
    comp_buf = compress_data(argv[1], strlen(argv[1]), &comp_len);

    // uncompress
    uncompress_data(comp_buf, comp_len);

    return 1;
}

static char* compress_data(char* data, int len, uLong *outLen)
{
    uLong comp_len;
    char* comp_buf;
    int ret;

    // calculate compressed size
    comp_len = compressBound((uLong)len);
    comp_buf = calloc(comp_len, sizeof(char));

    ret = compress((Bytef *)comp_buf, &comp_len, (Bytef *)data, len);
    if (ret != Z_OK) {
        printf("Could not compress the message correctly\n");
        return NULL;
    }
    printf("compressed size: %lu\n", comp_len);
    *outLen = comp_len;

    return comp_buf;
}

static char* uncompress_data(char* data, uLong len)
{
    char *res;
    char *tmp;
    int ret;

    char uncomp_buf[1000];
    unsigned long uncomp_len;
    char *comp_buf;
    unsigned long comp_len;
    unsigned long read_len;

    comp_buf = data;
    comp_len = len;

    read_len = 0;
    memset(uncomp_buf, 0x00, sizeof(uncomp_buf));
    uncomp_len = sizeof(uncomp_buf) - 1;
    comp_len -= read_len;

    printf("before...\n");
    ret = uncompress((Bytef *)uncomp_buf, &uncomp_len, (Bytef *)comp_buf, comp_len);
    printf("ret: %d\n", ret);
    if ((ret != Z_BUF_ERROR) && (ret != Z_OK)) {
        return NULL;
    }

    uncomp_buf[uncomp_len] = '\0';
    printf("%s", uncomp_buf);
    // printf("%s", uncomp_buf);

    return NULL;
}
