#include <stdio.h>
#include <stdlib.h>



int main(void) {

	float result = 0.0;
	int num_pos = 0;


	char t = '\0';
	float tmp = 0.0;

	printf("Enter an expression: ");


	for (int i=0;;i++) {
		char tt = getchar();
		printf("c-----------%c\n", tt);

		if ( tt == '\n') {
			break;
		}

		if ( tt == '+' || tt == '-' || tt == '*' || tt == '/') {
//			*(t+num_pos+1) = '\0'; 
//			tmp = atof(t);
//			printf("got float %f", tmp);
//			num_pos = 0;
//			switch (tt) {
//				case '+':
//					result = tmp + result;
//					break;
//				case '-':
//					result = result - tmp;
//					break;
//				case '*':
//					result = result * tmp;
//					break;
//				default:
//					result = result / tmp;
//					break;
//			}

		} else {
			printf("n------------%d\n", num_pos);
			*(&t+num_pos) = tt;
			num_pos++;
			*(&t+num_pos+1) = '\0';
			printf("t--------------%s\n", &t);
		}

	}

	printf("Value of expression: %f", result);
	return 0;
}
