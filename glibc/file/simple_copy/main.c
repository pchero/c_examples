
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    FILE* src;
    FILE* des;
    char str[100];
    int ret;
    char* tmp;

    src = fopen("src.txt", "rt");
    des = fopen("des.txt", "wt");

    if((src == NULL) || (des == NULL)) {
        printf("Could not open the file.\n");
        return 1;
    }

    while(1) {
        tmp = fgets(str, sizeof(str), src);
        if(tmp == NULL) {
            break;
        }

        fputs(str, des);
    }

    // if(foef(src) != 0) {
    //     printf("Copy done.\n");
    // }
    // else {
    //     printf("Could not finished copy.\n");
    // }

    return 0;
}