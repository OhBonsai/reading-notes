#include <stdio.h>


int main(void){
	int x,y;

	printf("Enter input n\n");

	scanf("%d", &x);

	for(int i=1;;i++) {
		
		y = i * i;
		if ( y % 2 == 0) {
			printf("%d\n", y);
		}

		if ( y > x) {
			break;
		}
	}


	return 0;

}
