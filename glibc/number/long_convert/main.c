#include <stdio.h>

int main(int argc, char** argv)
{
	unsigned long tmp_long;
	unsigned int tmp_int;

	tmp_long = 0;

	printf("%lu\n", tmp_long);

	tmp_int = 0xffffffff;
	printf("%u\n", tmp_int);
	printf("%u\n", tmp_int + 1);

	tmp_long = tmp_int;
	printf("%lu\n", tmp_long + 1);



	return 0;
}
