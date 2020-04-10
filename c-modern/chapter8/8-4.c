#include <stdio.h>

#define NUM ( (int) (sizeof input / sizeof input[0]))


int main(void){
	int number = 0;
	
	printf("Please input how many item you input: ");
	scanf("%d", &number);

	int input[number];
	printf("Please input %d number\n", number);

	for(int i=0; i<number; i++) {
		scanf("%d", &input[i]);
	}

	int tmp = 0;
	for(int i=0; i<NUM / 2; i++) {
		tmp = input[i];
		input[i] = input[NUM -1 - i];
		input[NUM -1 - i] = tmp;
	}
	
	for (int i=0; i<NUM; i++) {
		printf("%d ", input[i]);
	}
	
	
	printf("\n");
	


	return 0;
}
