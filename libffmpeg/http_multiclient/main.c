

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
        if(resource && strlen(resource)) {
            break;
        }
        av_freep(&resource);
    }
    
}