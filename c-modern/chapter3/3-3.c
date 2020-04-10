#include <stdio.h>

int main(void){

	int a,b,c,d,e;

	printf("Enter ISBN:\n");
	scanf("%d-%d-%d-%d-%d", &a, &b, &c, &d, &e);

	printf("GS1 prefix: %d\n", a);
	printf("GS1 prefix: %d\n", b);
	printf("GS1 prefix: %d\n", c);
	printf("GS1 prefix: %d\n", d);
	printf("GS1 prefix: %d\n", e);

	return 0;
}
