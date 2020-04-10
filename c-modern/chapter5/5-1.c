#include <stdio.h>


int main(void) {
	int input = 0;
	int bit = 0;
	printf("Enter a number:\n");
	scanf("%3d", &input);

	if (input / 100 > 0) {
		bit = 3;
	} else if (input / 10 > 0) {
		bit = 2;
	} else {
		bit = 1;
	}
	
	printf("The number %3d has %d digits\n", input, bit);
}
