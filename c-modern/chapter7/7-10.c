#include <stdio.h>


int main(void) {
	printf("Etner a sentence: \t");

	char *a;
	int number = 0;
	for(int i=0;;i++) {
		*(a+i) = getchar();	
			
		char t = *(a+i);

		if (t == '\n') {
			break;
		}

		if (t == 'a' || t == 'e' || t=='i' || t=='o' || t=='u') {
			number ++;
		}
	}


	printf("your sententce contains %d vowels.\n", number);


	return 0;
}
