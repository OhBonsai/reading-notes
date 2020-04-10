#include <stdio.h>



int power(int x, int n) {
	if (n == 0) {
		return 1;
	} else if (n % 2 == 0) {
		return power(x, n / 2) * power(x, n / 2);
	} else {
		return x * power(x, n - 1);
	}
}


int main(void) {
	int x, n;

	printf("Please input x, n  ");
	scanf("%d %d", &x, &n);

	int r = power(x, n);
	printf("The result is %d\n", r);
}
