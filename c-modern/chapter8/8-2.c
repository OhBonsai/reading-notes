#include <stdio.h>


int main(void) {
	char *a;

	printf("Enter a number: ");
	scanf("%s", a);


	int digit[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int number[10] = {0};

	for (int i=0; ;i++) {
		int x = *(a+i) - '0'; 
	
		if ( x > 9 || x < 0 ) {
			break;
		}

		number[x] ++;
	}


	printf("Digit:    ");
	for (int i=0; i<10; i++) {
		printf("%3d", i);		
	} 
	printf("\n");

	printf("Result:   ");
	for (int i=0; i<10; i++) {
		printf("%3d", number[i]);		
	} 
	printf("\n");

}
