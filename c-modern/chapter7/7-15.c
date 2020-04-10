#include <stdio.h>


int main(void) {
	long long int n;

	long long int result = 1;

	printf("Enter a positive integer: \t");
	scanf("%lld", &n);

	while (n > 0 ) {
		result = result * n;
		n = n - 1;
	}

	printf("Facorial of %lld\n", result);



	return 0;
}
