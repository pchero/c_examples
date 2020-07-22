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
static int compress_data(char *data, char *out, int len);
static char *uncompress_data(char *data, int len);

int main(int argc, char** argv)
{
    char *data;
    char *comp;
    char *uncomp;
    int len;
    int ret;

    data = argv[1];
    printf("Original message: %s\n", data);

    len = strlen(data);
    comp = calloc(len, sizeof(char));
    ret = compress_data(data, comp, len * sizeof(char));

    uncomp = uncompress_data(comp, ret);
    if (uncomp == NULL) {
        printf("Something went wrong.\n");
        return 0;
    }
    printf("Uncompress: %s\n", uncomp);

    printf("Compare: %d\n", strcmp(data, uncomp) == 0? 1:0);
    free(comp);
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

    if ((buf[0] & 0x78) && (buf[1] & 0xDA)) {
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

static int compress_data(char* data, char* out, int len)
{
    z_stream stream;
    int ret;

    init_stream(&stream);
    ret = deflateInit2(&stream, Z_BEST_COMPRESSION, Z_DEFLATED, windowBits, 8, Z_DEFAULT_STRATEGY);
    if (ret != Z_OK) {
        return -1;
    }

    stream.next_in = (Bytef *)data;
    stream.avail_in = strlen(data);

    stream.next_out = (Bytef *)out;
    stream.avail_out = len;

    ret = deflate(&stream, Z_FINISH);
    deflateEnd(&stream);
    if ((ret != Z_OK) && (ret != Z_STREAM_END)) {
        return -1;
    }

    printf("total: %lu\n", stream.total_out);

    return stream.total_out;
}

static char* uncompress_data(char* data, int len)
{
    z_stream stream;
    int ret;
    char tmp[5];
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

        res_tmp = res;
        asprintf(&res, "%s%s", res_tmp? : "", tmp);
        // printf("ret: %d, avail_in: %d, sofar: %s\n", ret, stream.avail_in, res);

        if (res_tmp != NULL) {
            free(res_tmp);
        }

        if (ret == Z_STREAM_END && stream.avail_in == 0) {
            break;
        }
   }
   inflateEnd(&stream);

   return res;
}
