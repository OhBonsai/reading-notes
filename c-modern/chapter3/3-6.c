#include <stdio.h>

int main(void){
	int a,b,c,d;
	printf("Enter two fractions separated by a plus sign:\n");
	
	scanf("%d/%d+%d/%d", &a, &b, &c, &d);
	printf("%d/%d", a*d+b*c, b*d);
	return 0;
}
