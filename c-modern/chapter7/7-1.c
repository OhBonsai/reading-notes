#include <stdio.h>


int main(void){
	int x, n;
	short int i = 9;
	long int y = 10;

	printf("int length %lu\n", sizeof y);
	printf("short int length %lu\n", sizeof x);
	printf("int length %lu\n", sizeof i);

	printf("This program prints a table of squares.\n");

	printf("Enter number of entries in talbe: ");

	scanf("%d", &n);

	i = 1;
	while (i <= n ) {
		printf("%20d%20d\n", i, i * i);
		i ++;
	}

	return 0;
}

