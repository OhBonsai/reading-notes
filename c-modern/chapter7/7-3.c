#include <stdio.h>

int main(void){
	long n, sum =0;
	printf("THis program sums a series of integers.\n");
	printf("Enter integers : ");

	scanf("%ld", &n);

	while (n != 0) {
		sum += n;
		scanf("%ld", &n);
	}
	printf("The sum is %ld\n", sum);

	return 0;
}
