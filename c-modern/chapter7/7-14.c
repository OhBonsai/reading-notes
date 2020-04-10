#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int main(void) {
	
	double number = 0.0;
	double x = 0.0;
	double y = 1.0;


	printf("Enter a positive number: \t");
	scanf("%lf", &number);
	x = number;
	


	while (fabs( y - x ) >= 0.0001) {
		
		y = (x + y) / 2;
		x = number / y;
		
		printf("y: %lf\tx: %lf\n", y, x);

	}

	printf("Square root: \t  %lf\n", y);

	return 0;
}
