#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


void reserve_words(const char *a, char *b) {
	int len = strlen(a);

	for (int i=0; i<len; i++) {
		*(b+len-i-1) = *(a+i);
	}
	
}



int main(void) {
	char *a;
	char b[100] = "";
	_Bool yes = true;

	printf("ENTER MESSAGE: ");
	scanf("%s", a);
	strcpy(b, a);

	int len = strlen(a);
	reserve_words(a, b);

	for(int i=0; i<len ;i++) {
		if ( *(a+i) != *(b+i)) {
			yes = false;
			
		}
	
	}

	printf("Enter %d", yes);



	return 0;
}
