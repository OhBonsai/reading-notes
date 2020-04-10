#include <stdio.h>


int main(void){
	int x,y;

	printf("Enter value of trade:\n");
	scanf("%d/%d", &x, &y);

	int u,v;
	u = x;
	v = y;

        if (x < y) {
                int t = x;
                x = y;
                y = t;
        }

        while (x != 0) {
                int t = x  % y;
                y = x;
                x = t;
        }


	printf("In lowest terms: %d/%d", u/y, v/y);
	return 0;


}
