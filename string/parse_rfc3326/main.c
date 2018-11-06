
#include <stdio.h>
#include <stdbool.h>
#include <jansson.h>
#include <string.h>
#include <ctype.h>

json_t* parse_rfc3326(const char* header);
bool parse_detail(json_t* j_res, const char* str);
char* trimwhitespace(char* str);


int main(int argc, char** argv)
{
    json_t* j_res;
    char* tmp;

    j_res = parse_rfc3326(" Q.850;cause=1");
    tmp = json_dumps(j_res, JSON_ENCODE_ANY);
    printf("Result. res[%s]\n", tmp);
    free(tmp);
    json_decref(j_res);

    return 0;
}

json_t* parse_rfc3326(const char* header)
{
    char tmp[1024];
    int len;
    int i;
    int j;
    json_t* j_res;

    if(header == NULL) {
        fprintf(stderr, "Wrong input parameter.\n");
        return NULL;
    }

    // Q.850;cause=1
    len = strlen(header);
    printf("Go. len[%d]\n", len);

    j = 0;
    j_res = json_array();
    for(i = 0; i <= len; i++) {

        if((header[i] == ';') || (i == len)) {
            tmp[j] = '\0';

            printf("temp. %s\n", tmp);
            parse_detail(j_res, tmp);
            memset(tmp, 0x00, sizeof(tmp));

            j = 0;
            continue;
        }

        tmp[j] = header[i];
        j++;
    }

    return j_res;
}

bool parse_detail(json_t* j_res, const char* str)
{
    char* org;
    char* tmp;
    char name[1024];
    char value[1024];
    int i;
    int j;
    int len;
    int flag;
    json_t* j_tmp;

    if((j_res == NULL) || (str == NULL)) {
        fprintf(stderr, "Wrong input parameter.\n");
        return false;
    }
    printf("Debug. %s\n", str);

    org = strdup(str);
    tmp = trimwhitespace(org);

    memset(name, 0x00, sizeof(name));
    memset(value, 0x00, sizeof(value));

    flag = 0;
    len = strlen(tmp);
    for(i = 0, j = 0; i < len; i++) {
        if(tmp[i] == '=') {
            flag = 1;
            j = 0;
            continue;
        }

        if(flag == 0) {
            name[j] = tmp[i];
        }
        else {
            value[j] = tmp[i];
        }
        j++;
    }

    j_tmp = json_object();
    if(flag == 0) {
        json_object_set_new(j_tmp, "protocol", json_string(name));
        printf("No vlaue. name[%s]\n", name);
    }
    else {
        json_object_set_new(j_tmp, name, json_string(value));
        printf("With value. name[%s], value[%s]\n", name, value);
    }
    free(org);

    json_array_append_new(j_res, j_tmp);

    return true;
}


char* trimwhitespace(char* str)
{
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) {
        str++;
    }

    if(*str == 0) { // All spaces?
        return str;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) {
        end--;
    }

    // Write new null terminator character
    end[1] = '\0';

    return str;
}
