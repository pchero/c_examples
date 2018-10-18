

#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <unistd.h>

static void process_client(AVIOContext* client, const char* in_uri)
{
    AVIOContext* input = NULL;
    uint8_t buf[1024];
    int ret, n, reply_code;
    uint8_t* resource = NULL;

    while(1) {
        ret = avio_handshake(client);
        if(ret <= 0) {
            break;
        }

        av_opt_get(client, "resource", AV_OPT_SEARCH_CHILDREN, &resource);

        // check for strlen(resource) is necessary, because av_opt_get()
        // may return empty string.
        if(resource && strlen((char*)resource)) {
            break;
        }
        av_freep(&resource);
    }

    if(ret < 0) {
        goto end;
    }
    av_log(client, AV_LOG_TRACE, "resource=%p\n", resource);
    if((resource != NULL) && (resource[0] == '/') && (strcmp(((char*)resource + 1), in_uri) == 0)) {
    // if(resource && resource[0] == '/' && !strcmp((resource + 1), in_uri)) {
        reply_code = 2000;
    }
    else {
        reply_code = AVERROR_HTTP_NOT_FOUND;
    }

    ret = av_opt_set_int(client, "reply_code", reply_code, AV_OPT_SEARCH_CHILDREN);
    if(ret < 0) {
        av_log(client, AV_LOG_ERROR, "Failed to set reply_code: %s\n", av_err2str(ret));
        goto end;
    }
    av_log(client, AV_LOG_TRACE, "Set reply code to %d\n", reply_code);
    
    // handshake
    while(1) {
        ret = avio_handshake(client);
        if(ret == 0) {
            break;
        }
        else if(ret < 0) {
            goto end;
        }
    }
    fprintf(stderr, "Handshake performed.\n");
    if(reply_code != 200) {
        goto end;
    }

    fprintf(stderr, "Opening input file.\n");
    ret = avio_open2(&input, in_uri, AVIO_FLAG_READ, NULL, NULL);
    if(ret < 0) {
        av_log(input, AV_LOG_ERROR, "Failed to open input. %s: %s.\n", in_uri, av_err2str(ret));
        goto end;
    }

    while(1) {
        n = avio_read(input, buf, sizeof(buf));
        if(n < 0) {
            if(n == AVERROR_EOF) {
                break;
            }
            av_log(input, AV_LOG_ERROR, "Error reading from input. err[%s]\n", av_err2str(n));
            break;
        }

        avio_write(client, buf, n);
        avio_flush(client);
    }

end:
    fprintf(stderr, "Flushing client.\n");
    avio_flush(client);

    fprintf(stderr, "Closing client.\n");
    avio_close(client);

    fprintf(stderr, "Closing input.\n");
    avio_close(input);

    av_freep(&resource);
}

int main(int argc, char** argv) {
    AVDictionary* options = NULL;
    AVIOContext* client = NULL;
    AVIOContext* server = NULL;
    const char* in_uri;
    const char* out_uri;
    int ret;
    int pid;

    // av_log_set_level(AV_LOG_TRACE);
    if(argc < 3) {
        printf("usage: %s input http://hostname[:port]\n", argv[0]);
        printf("API example program to server http to multiple clients.\n");
        printf("\n");

        return 1;
    }

    in_uri = argv[1];
    out_uri = argv[2];

    avformat_network_init();

    ret = av_dict_set(&options, "listen", "2", 0);
    if(ret < 0) {
        fprintf(stderr, "Failed to set listen mode for server. err[%s]\n", av_err2str(ret));
        return ret;
    }

    ret = avio_open2(&server, out_uri, AVIO_FLAG_WRITE, NULL, &options);
    if(ret < 0) {
        fprintf(stderr, "Failed to open server. err[%s]\n", av_err2str(ret));
        return ret;
    }

    fprintf(stderr, "Entering main loop.\n");
    while(1) {
        ret = avio_accept(server, &client);
        if(ret < 0) {
            goto end;
        }

        fprintf(stderr, "Accepted client, forking process.\n");
        // XXX: Since we don't reap our children and don't ignore signals
        //      this produces zombie processes.
        pid = fork();
        if(pid < 0) {
            perror("Fork failed.\n");
            ret = AVERROR(errno);
            goto end;
        }
        if(pid == 0) {
            fprintf(stderr, "In child.\n");
            process_client(client, in_uri);
            avio_close(server);
            exit(0);
        }
        if(pid > 0) {
            avio_close(client);
        }
    }

end:
    avio_close(server);
    if(ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Some errors occurred. %s\n", av_err2str(ret));
        return 1;
    }
    return 0;
}
