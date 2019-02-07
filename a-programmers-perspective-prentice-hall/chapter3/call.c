#include <stdio.h>


int call1(int a)
{
		return call2(a) + 1;
}

int call2(int a)
{
		return a * a;
}

int main(void){
		int r = call1(3);
		printf("result is %d\n", r)
		return 0;
}
