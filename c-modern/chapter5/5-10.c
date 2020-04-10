#include <stdio.h>


int main(void) {
	int x,y;


	printf("Enter numerical grade: \n");
	scanf("%d", &x);


	y = x / 10;

	if (x == 100 ){
		printf("A");
		return 0;
	} else if (x > 100) {
		printf("ERROR\n");
		return 0;
	} else if (x < 0) {
		printf("ERROR\n");
		return 0;
	} else {
	} 

	switch (y) {
	
		case 9:
			printf("A");
			break;
		case 8:
			printf("B");
			break;
		case 7:
			printf("C");
			break;
		case 6:
			printf("D");
			break;
		default:
			printf("E");
	}


	return 0;

}
