#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define WIDTH 10


// 0 up 1 down 2 left 3 right
char* set_next(char a[WIDTH][WIDTH], int row, int col) {
	bool hit = false;
	char last_char = a[row][col];
	int new_row, new_col;


	for (int i=0; i<100; i++) {
		if (hit) break;
		int rand_value = rand() % 4;

		switch (rand_value) {
			case 0: // up
				new_row =  row - 1;
				new_col =  col;

				if ( new_row  >= 0 && a[new_row][new_col] == '.'){
					a[new_row][new_col] = last_char + 1;
					hit = true;
				}
				break;
			case 1: // down
				new_row =  row + 1;
				new_col =  col;

				if ( new_row  < WIDTH && a[new_row][new_col] == '.'){
					a[new_row][new_col] = last_char + 1;
					hit = true;
				}
				break;
			case 2: // left
				new_row =  row;
				new_col =  col - 1;

				if ( new_col  >= 0 && a[new_row][new_col] == '.'){
					a[new_row][new_col] = last_char + 1;
					hit = true;
				}
				break;
			case 3: // right
				new_row =  row;
				new_col =  col + 1;

				if ( new_col  < WIDTH && a[new_row][new_col] == '.'){
					a[new_row][new_col] = last_char + 1;
					hit = true;
				}
				break;

		}

	}


	if (hit) {
		return &(a[new_row][new_col]);
	} else {
		return NULL;
	}
	
}

void print_array(char a[WIDTH][WIDTH]) {
	for (int i=0; i<WIDTH; i++) {
		for (int j=0; j<WIDTH; j++) {
			if (a[i][j] == ' ') {
				a[i][j] = '.';
			}
			
			printf("%c ", a[i][j]);
		}
		printf("\n");
	}
}




int main(void) {
	srand((unsigned)time(NULL));
	char a[WIDTH][WIDTH] = {'A'};

	for(int i=0; i<WIDTH; i++) {
		for(int j=0; j<WIDTH; j++) {
			a[i][j] = '.';
		}
	}
	a[0][0] = 'A';

	int x=0;
	int y=0;

	for (int i=0; i<25; i++) {
		char* n = set_next(a, x, y);
		if ( NULL == n) {
			break;
		}
		x = (n - &a[0][0]) / WIDTH;
		y = (n - &a[0][0]) % WIDTH;
	}
	print_array(a);

	return 0;
}
