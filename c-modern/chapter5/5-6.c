#include <stdio.h>


int main(void){
	int a[4];


	printf("Enter the four number:\n");
	scanf("%d %d %d %d", a, a+1, a+2, a+3);

	int max=0, min=a[1];

	for (int i=0; i<4; i++) {
		if (max < a[i] ) {
			max = a[i];
		}

		if (min > a[i] ) {
			min = a[i];
		}

	}

	

	printf("Largest: \n %d ", max);
	printf("Min: \n %d ", min);
}
