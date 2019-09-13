#include <stdio.h>
#include <stdlib.h>

typedef struct _test_st {
	int num;
} test_st;

int main(int argc, char** argv)
{
	test_st* tmp_st;

	tmp_st = calloc(sizeof(test_st), 1);
	tmp_st->num = 0;

	++tmp_st->num;

	printf("Result: %d\n", tmp_st->num);

	return 0;
}
