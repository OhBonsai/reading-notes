#include <stdio.h>

#define STU 5
#define SUJ 5

int main(void){

	int a[STU][SUJ] = {0};

	for (int i=0; i<STU; i++) {
		printf("Enter student %d: ", i);
	
		for (int j = 0; j<SUJ; j++) {
			scanf("%d", &a[i][j]);
		}
	}


	// Total of student
	for (int i =0; i<STU; i++ ) {
		int sum = 0;
		for (int j = 0; j<SUJ; j++) {
			sum += a[i][j];
		}
		printf("Sum is %d\n", sum);
	} 
	return 0;
}
