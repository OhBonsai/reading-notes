#include <stdio.h>


int main(void){
	float x;

	printf("Please input the value\n");
	scanf("%f", &x);

	if ( x<750) {
		printf("%f\n", x * 0.01);
	} else if ( x<2250) {
		printf("%f\n", (x-750) * 0.02 + 7.5);
	} else if ( x<3750) {
		printf("%f\n", (x-2250) * 0.03 + 37.50);
	} else if ( x<5250) {
		printf("%f\n", (x-3750) * 0.04 + 82.50);
	} else if ( x<7000) {
		printf("%f\n", (x-5250) * 0.05 + 142.50);
	} else {
		printf("%f\n", (x-7000) * 0.06 + 230.00);
	}
}
