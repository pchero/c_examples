#include <stdio.h>

int main(int argc, char** argv)
{
	int ret;

	ret = 0 / 100;
	printf("%d\n", ret);

	ret = 100 / 0;
	printf("%d\n", ret);

	return 0;
}
