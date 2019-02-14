#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    double tmp_val;
    char tmp[40];
    int dec, frac;

    tmp_val = 12.345678;

    snprintf(tmp, sizeof(tmp), "%lf", tmp_val);
    printf("%s\n", tmp);

    sscanf(tmp, "%d.%d", &dec, &frac);
    printf("%d, %d\n", dec, frac);

    return 0;


//   int counts;
//   char *sign;
//   char str[15];
//   char *base;
//   char *exp10;
//   float real = 0.00001;
  
//   sprintf (str, "%E",  real);
//   sign= ( strpbrk ( str, "+"))? "+" : "-";
  
//   base = strtok (str, sign);
//   exp10 = strtok (NULL, sign);
  
//   counts=atoi(exp10);
  
//   printf("[%d]\n", counts);
  
//   return 0;
}
  