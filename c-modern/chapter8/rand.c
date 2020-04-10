#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int main(void) {
	srand((unsigned)time(NULL));

	for (int i=0; i< 100; i++) {
		printf("%d\n", rand());
	}

	return 0;
}
