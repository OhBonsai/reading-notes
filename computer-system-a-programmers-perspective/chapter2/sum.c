#include <stdio.h>

void inplace_swap(int *x, int *y) 
{
		*y = *x ^ *y;
		*x = *x ^ *y;
		*y = *x ^ *y;
}


void reverser_array(int a[], int cnt) {
		int first, last;
		for (first = 0, last = cnt-1; first <= last; first ++, last--)
				inplace_swap(&a[first], &a[last]);
}

int main(void)
{
		int len = 5;
		int b[len] = {1,2,3,4,5};
		reverser_array(b, len);
		int i = 0;
		for(i=0; i<len; i++){
				printf("%d,", b[i]);
		}
}
