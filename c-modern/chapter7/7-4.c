#include <stdio.h>


int main(void) {
	printf("Enter phone number: \n");

	char *c;

	for(int i=0;;i++) {
		*(c+i) = getchar();
		if (*(c+i) == '\n') {
			break;
		} 

		int tmp = *(c+i);

		if ( ! (( tmp >= 65 && tmp <= 90) || ( tmp >= 97 && tmp <= 122) ) ) {
			continue;
			
		}  



		int t = *(c+i) & 0xdf - 'A';


		if ( 0 <= t && t < 3 ) {
			*(c+i) = '2';
		} else if ( 3 <= t && t < 6) {
			*(c+i) = '3';
		} else if ( 6 <= t && t < 9) {
			*(c+i) = '4';
		} else if ( 9 <= t && t < 12) {
			*(c+i) = '5';
		} else if ( 12 <= t && t < 15) {
			*(c+i) = '6';
		} else if ( 15 <= t && t < 19) {
			*(c+i) = '7';
		} else if ( 19 <= t && t < 22) {
			*(c+i) = '8';
		} else if ( 22 <= t && t <= 26) {
			*(c+i) = '9';
		} else {

		}

	}

	printf("Input is %s", c);
	return 0;
}
