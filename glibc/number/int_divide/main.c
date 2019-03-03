#include <stdio.h>

int main(int arrc, char** argv)
{
    int tmp_val;

    tmp_val = 163250;
    printf("%d\n", tmp_val);

    tmp_val = tmp_val/65536;
    printf("%d\n", tmp_val);


    return 0;
}
