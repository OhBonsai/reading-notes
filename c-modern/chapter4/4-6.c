#include <stdio.h>


int main(void) {
	int a, b, ta, tb;
	ta = 0;
	tb = 0;


	printf("Enter the first 12 digits of an EAN: \n");

	scanf("%6d%6d", &a, &b);
	
	printf("a=%d\n", a);
	printf("b=%d\n", b);

	for (int i=0; i<6; i++) {
		int curPosA = a;
		int curPosB = b;
		for (int j =0; j<5- i; j++) {
			curPosA= curPosA / 10;
			printf("curPosA is %d\n", curPosA);


			curPosB= curPosB / 10;
			printf("curPosB is %d\n", curPosB);
		}	
		curPosA = curPosA % 10;
		curPosB = curPosB % 10;
		printf("--------------------------");
		printf("curPosA is %d\n", curPosA);
		printf("--------------------------");
		printf("curPosB is %d\n", curPosB);

		if (i % 2 == 1) {
			tb += curPosA;
			tb += curPosB;
		} else {
			ta += curPosA;
			ta += curPosB;
		
		}
		printf("ba=%d\n", ta);
		printf("bb=%d\n", tb);
		
	}
	

	printf("Check digit: %d\n", 9-((tb*3+ta-1)%10));
}
