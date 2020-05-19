#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int ari_endpoint_parser(char* dialstring);

int main(int argc, char** argv) {

    ari_endpoint_parser("pjsip/test-voip/test@test.com");
    ari_endpoint_parser("pjsip/has_no_endpointtest@test.com");
    ari_endpoint_parser("has_no_endpointtest@test.com");
    ari_endpoint_parser("/has_no_endpointtest@test.com");
    ari_endpoint_parser("//has_no_endpointtest@test.com");
    ari_endpoint_parser("/has_no_endpointtest@test.com/");

    return 0;
}

static int ari_endpoint_parser(char* dialstring) {
    char* tech = NULL;
    char* stuff = NULL;
    char* dialdevice = NULL;
    
    tech = strdup(dialstring);
	if ((stuff = strchr(tech, '/'))) {
		*stuff++ = '\0';
		dialdevice = stuff;
        printf("test\n");
    }
    printf("org: %s,\ttech: %s,\tdialdevice: %s,\tstuff: %s\n", dialstring, tech, dialdevice, stuff);

    free(tech);
    return 0;
}