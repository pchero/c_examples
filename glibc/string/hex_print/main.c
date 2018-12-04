#include <stdio.h>

int main(int argc, char** argv)
{
	unsigned short magic = 'BA';

	printf("%c\n", magic);
	printf("%x\n", magic);
	printf("%c\n", (char*)magic+1);

	return 0;
}
