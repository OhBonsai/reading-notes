#include <stdio.h>
#include <stdbool.h>


int main(void) {

	int number;

	printf("Enter your number: ");
	scanf("%d", &number);
	printf("number is %d\n", number);
	

	short unsigned int a[number][number];
	_Bool b[number*number];
	
	for (int i=0; i<number*number; i++) {
		b[i] = false;
	}

	int x = 0;
	int y = (number - 1) / 2;

	for (int i =1; i<= number * number; i++ ) {
		a[x][y] = i;
		b[x * number + y] = true;
		printf("x: %d, y: %d, i: %d\n", x+1, y+1, i);
		int new_x, new_y;

		new_x = (x > 0) ? x - 1: number-1;
		new_y = (y < number-1) ? y + 1 : 0;

		if (b[new_x * number + new_y]) {
			new_x = (x < number-1) ? x + 1: 0;
			new_y = y;
		}

		x = new_x;
		y = new_y;

	}

	for (int i =0; i< number; i++ ){
		printf("\n");
		for (int j=0; j< number; j++) {
			printf("%8d", a[i][j]);
		}
	}

	printf("\n");


	return 0;
}
