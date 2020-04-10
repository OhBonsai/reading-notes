#include <stdio.h>


int main(void){

	float now, max=0.0;

	for (;;) {
		printf("Enter a number: \t");

		scanf("%f", &now);

		if (now <= 0) {
			break;
		}

		if (now > max) {
			max = now;
		}
		
	}


	printf("The max is %f", max);

	return 0;

}
