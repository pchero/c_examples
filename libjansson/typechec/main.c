#include <stdio.h>
#include <jansson.h>

int main(int argc, char** argv)
{
	json_t* j_tmp;

	j_tmp = json_object();
	json_object_set_new(j_tmp, "test", json_null());

	if (json_is_real(json_object_get(j_tmp, "test")) == 1) {
		printf("Real\n");
	}

	if (json_is_object(json_object_get(j_tmp, "test")) == 1) {
		printf("Ojbect\n");
	}

	if (json_is_null(json_object_get(j_tmp, "test")) == 1) {
		printf("Null\n");
	}


	return 0;
}
