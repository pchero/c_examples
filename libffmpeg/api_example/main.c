
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef HAVE_AV_CONFIG_H
    #undef HAVE_AV_CONFIG_H
#endif

#include "libavcodec/avcodec.h"
#include "libavutil/mathemetics.h"

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

/*
 * Audio encoding example
 */
static void audio_encode_example(const char* filename)
{
    AVCodec* codec;
    AVCodecContext *c = NULL;
    int frame_size, i, j, out_size, outbuf_size;
    FILE* f;
    short* samples;
    float t, tincr;
    uint8_t* outbuf;

    printf("Audio encoding\n");

    // find the MP2 encoder
    codec = avcodec_find_encoder(CODEC_ID_MP2);
    if(!codec) {
        fprintf(stderr, "codec not found\n");
        exit(1);
    }

    c = avcodec_alloc_context3(codec);
    

}