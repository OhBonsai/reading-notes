#include <stdio.h>

int main(void) {

	int a, b;

	printf("Enter a 24-hour time:\t");
	scanf("%2d:%2d", &a, &b);

	printf("Equivalent 12-hour time:\t %2d:%-2d\n", a % 12, b);
}
