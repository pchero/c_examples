

#include <stdio.h>

#include <libavformat/avformat.h>
#include <libavutil/dict.h>

int main(int argc, char** argv)
{
    AVFormatContext* fmt_ctx = NULL;
    AVDictionaryEntry* tag = NULL;
    int ret;

    if(argc != 2) {
        printf("usage: %s <input_file>\n"
            "example program to demonstrate the use of the libavformat metadata API.\n"
            "\n", argv[0]
        );
        return 1;
    }

    ret = avformat_open_input(&fmt_ctx, argv[1], NULL, NULL);
    if(ret != 0) {
        printf("Could not open file. filename[%s]\n", argv[1]);
        return ret;
    }

    while(1) {
        tag = av_dict_get(fmt_ctx->metadata, "", tag, AV_DICT_IGNORE_SUFFIX);
        if(tag == NULL) {
            break;
        }

        printf("%s=%s\n", tag->key, tag->value);
    }
    avformat_close_input(&fmt_ctx);

    return 0;
}