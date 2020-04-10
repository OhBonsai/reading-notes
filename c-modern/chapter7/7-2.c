#include <stdio.h>


int main(void){
	int x, n;
	short int i = 9;
	long int y = 10;

	scanf("%d", &n);

	i = 1;
	while (i <= n ) {
		printf("%20d%20d\n", i, i * i);
		i ++;
		if (i % 24 == 0) {

		   printf("请输入字符：");
		   char c = getchar();

		   printf("输入的字符：");
		   putchar(c);
		   
		   if (c != '\n') {
			   goto finish;
		   }

		}
	}
	
	finish: 

	return 0;
}

