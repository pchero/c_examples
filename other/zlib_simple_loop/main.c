// gcc -o main -lz ./main.c

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <zlib.h>
#include <errno.h>

#define windowBits 15

#define GZIP_ENCODING 16
#define ENABLE_ZLIB_GZIP 32

static int is_compressed(char* buf);
static char* compress_data(char* data, int len, int *outLen);
static char *uncompress_data(char *data, int len);

int main(int argc, char** argv)
{
    char *data;
    char *comp_buf;
    char *uncomp;
    int comp_len;

    if (argc < 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 1;
    }

    data = argv[1];
    printf("Original message: %s\n", data);

    // compress
    comp_buf = compress_data(data, strlen(data), &comp_len);
    if (comp_buf == NULL) {
        printf("Something went wrong.\n");
        return 1;
    }

    // uncompress
    uncomp = uncompress_data(comp_buf, comp_len);
    if (uncomp == NULL) {
        printf("Something went wrong.\n");
        return 1;
    }
    printf("Uncompress: %s\n", uncomp);

    printf("Compare: %d\n", strcmp(data, uncomp) == 0? 1:0);
    free(comp_buf);
    free(uncomp);
}

// returns 1 if the given buffer is compressed buffer.
// currently checks only ZLIB over level 7.
// Level | ZLIB  | GZIP
//   1   | 78 01 | 1F 8B
//   2   | 78 5E | 1F 8B
//   3   | 78 5E | 1F 8B
//   4   | 78 5E | 1F 8B
//   5   | 78 5E | 1F 8B
//   6   | 78 9C | 1F 8B
//   7   | 78 DA | 1F 8B
//   8   | 78 DA | 1F 8B
//   9   | 78 DA | 1F 8B
static int is_compressed(char* buf) {
    if (strlen(buf) < 2) {
        return 0;
    }

    if ((buf[0] & 0x78) || (buf[0] & 0x1F)) {
        return 1;
    }

    return 0;
}

static void init_stream(z_stream *stream)
{
	stream->zalloc = Z_NULL;
	stream->zfree = Z_NULL;
	stream->opaque = Z_NULL;
}

// compress_data compress the data of thegiven length.
// returns compressed buffer and set the buffer length to the outLen.
static char* compress_data(char* data, int len, int *outLen)
{
    uLong comp_len;
    char* comp_buf;
    int ret;

    // calculate compressed size
    comp_len = compressBound((uLong)len);
    comp_buf = calloc(comp_len, sizeof(char));

    ret = compress2((Bytef *)comp_buf, &comp_len, (Bytef *)data, len, Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        printf("Could not compress the message correctly\n");
        return NULL;
    }
    printf("compressed size: %lu\n", comp_len);
    *outLen = comp_len;

    return comp_buf;
}

// uncompress_data uncompress the compressed data with given length.
// it compare the first byte to check the compressed data or not.
static char* uncompress_data(char* data, int len)
{
    z_stream stream;
    int ret;
    char tmp[500];
    char *res;
    char *res_tmp;

    if (is_compressed(data) != 1) {
        return NULL;
    }

    init_stream(&stream);
    ret = inflateInit2(&stream, windowBits | ENABLE_ZLIB_GZIP);
    if (ret != Z_OK) {
        return NULL;
    }

    stream.next_in = (Bytef *)data;
    stream.avail_in = len;

    res = NULL;
    res_tmp = NULL;
    while(1) {
        memset(tmp, 0x00, sizeof(tmp));
        stream.next_out = (Bytef *)tmp;
        stream.avail_out = sizeof(tmp) - 1;

        ret = inflate(&stream, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&stream);
            return NULL;
        }

        // add the uncompressed string to the result
        res_tmp = res;
        asprintf(&res, "%s%s", res_tmp? : "", tmp);
        if (res_tmp != NULL) {
            free(res_tmp);
        }

        if (ret == Z_STREAM_END || stream.avail_in == 0) {
            break;
        }
   }
   inflateEnd(&stream);

   return res;
}
