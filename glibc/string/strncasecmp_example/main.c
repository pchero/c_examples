
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
  int ret;
  char* lang;

  lang = "test";

  ret = strncasecmp(lang, "testttt", 7);
  printf("Result. ret[%d]\n", ret);

  return 0;
}
