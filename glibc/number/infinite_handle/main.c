#include <stdio.h>
#include <math.h>
#include <jansson.h>

int main(int argc, char** argv)
{
	double tmp_double;
	json_t* j_tmp;
	char* tmp;
	
	tmp_double = INFINITY;

	printf("%f\n", tmp_double);

	if (isinf(tmp_double)) {
		printf("infinite\n");
	}
	printf("%d\n", isfinite(tmp_double));

	j_tmp = json_object();
	json_object_set_new(j_tmp, "test", json_real(tmp_double));

	tmp = json_dumps(j_tmp, JSON_DECODE_ANY);
	printf("%s\n", tmp);


	tmp_double = NAN;
	printf("%f\n", tmp_double);

	if (isinf(tmp_double)) {
		printf("infinite\n");
	}
	printf("%d\n", isfinite(tmp_double));

	return 0;
}
