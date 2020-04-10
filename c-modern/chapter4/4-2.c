#include <stdio.h>


int main(void) {
	char c;
	printf("Enter a digit number:\n");
	scanf("%s", &c);
	
	printf("The reversal is :\n");
	for (int i=0; i<3; i++) {
		printf("%c", *(&c+4-i));
		if (i==2){
			printf("\n");
		}
	}

	return 0;
}
