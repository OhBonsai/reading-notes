#include <stdio.h>
#include <stdlib.h>

#define LEN 5



void selection_sort(int a[], int count);
int find_max(int a[], int count);



int main(void) {
	int a[LEN];
	
	printf("Please input %d number: ", LEN);

	for (int i=0; i<LEN; i++) {
		scanf("%d", &a[i]);
		
	}

	selection_sort(a, LEN);

	for (int i=0; i<LEN; i++) {
	
		printf("%d ", a[i]);
	}


	printf("\n");

	return  0;
}


void print_array(int a[]) {
	for (int i=0; i<LEN; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
}


void selection_sort(int a[], int count){
	int tmp =0;
	for (int i=LEN - 1; i>0; i--) {
		int x_idx = find_max(a, i);
		tmp = a[x_idx];
		a[x_idx] = a[i];
		a[i] = tmp;
		printf("array ias");
		print_array(a);
	}

}



int find_max(int a[], int count) {
	int max = 0;
	int x = 0;
	for (int i=0; i<=count; i++) {	
		printf("a[i]: %d\n", a[i]);
		max = a[i] > max ? a[i] : max;
		x = max == a[i] ? i : x;
		printf("max: %d x: %d i: %d \n", max, x, i);
	}
	return x;
}



