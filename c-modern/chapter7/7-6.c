#include <stdio.h>


int main(void) {

	int a = 1;
	short b = 1;
	long c = 1;
	float d = 1.0;

	double e = 1.0;
	long double f = 1.0;


	printf("int %ld\n", sizeof a);
	printf("short %ld\n", sizeof b);
	printf("long %ld\n", sizeof c);
	printf("float %ld\n", sizeof d);
	printf("double %ld\n", sizeof e);
	printf("long double %ld\n", sizeof f);


	return 0;
}
