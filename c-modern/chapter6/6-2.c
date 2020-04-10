#include <stdio.h>



int main(void){
	int x, y;

	printf("Enter two integer: \t");
	scanf("%d %d", &x, &y);

	if (x < y) {
		int t = x;
		x = y;
		y = t;
	}

	while (x != 0) {
		int t = x  % y;
		y = x;
		x = t;
	}

	printf("%d\n", y);

	return 0;
}
