#include <stdio.h>


void print_array(int length, int a[length]) {
	for(int i=0; i<length; i++){
		printf("%2d  ", a[i]);
	}
	printf("\n");
}


int main(void){
	int a[16];
	int row_sums[4], col_sums[4], diagonal_sums[2];

	printf("Enter the numbers from 1 to 16 in any order\n");

	for (int i=0; i<16; i++){
		scanf("%d", a+i);
	}
	
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			if (i==j){
				diagonal_sums[0] += a[i*4+j];
			}

			if (i==4-j){
				diagonal_sums[1] += a[i*4+j];
			}

			row_sums[i] += a[i*4+j];
			col_sums[j] += a[i*4+j];
			printf("%2d\t", a[i*4+j]);
			if (j == 3) {
				printf("\n");
			} 
		}
	}
	


	print_array(4, row_sums);
	print_array(4, col_sums);
	print_array(2, diagonal_sums);

	
	return 0;
}
