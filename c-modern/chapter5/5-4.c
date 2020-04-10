#include <stdio.h>


int main(void){
	float x;

	printf("Please input the speed\n");
	scanf("%f", &x);


	if ( x<1) {
		printf("Calm\n");
	} else if ( x<3) {
		printf("Light air\n");
	} else if ( x<27) {
		printf("Breeze`\n");
	} else if ( x<47) {
		printf("Gale\n");
	} else if ( x<63) {
		printf("Storm\n");
	} else {
		printf("Hurricane\n");
	}
}
