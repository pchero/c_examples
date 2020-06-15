#include <stdio.h>

int main(int argc, char** argv)
{
    int ret;
    char* tmp;
    int val;

    tmp = "-12345";
    ret = sscanf(tmp, "%d", &val);
    printf("ret: %d, val: %d\n", ret, val);

    tmp = "wrong val";
    ret = sscanf(tmp, "%d", &val);
    printf("ret: %d, val: %d\n", ret, val);


}