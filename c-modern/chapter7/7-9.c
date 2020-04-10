#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



#define bool        _Bool
#define true        1
#define false        0
#define __bool_true_false_are_defined        1


int main(void) {
	printf("Enter a 12-hour time: \n");

	char *c;

	bool has_p = false;


	for(int i=0;;i++) {
		*(c+i) = getchar();
		char t = *(c+i);
		if (t == '\n') {
			break;
		} 


		if (t == 'P' || t == 'p') {
			has_p = true;
		} }


	int hour_bit_size = 0;
	for (int i=1; ;i++) {
		hour_bit_size ++;
		if (*(c+i) == ':') {
			break;
		}
	}
		
	
	char hour[2];
	if (hour_bit_size == 2) {
		hour[0] = c[0];
		hour[1] = c[1];
	} else {
		hour[1] = c[0];
		hour[0] = '0';
	}

	int hour_num = atoi(hour);


	if ( has_p ) {
		hour_num += 12;
	}


	printf("Equivalent 24-hour timeis %d:%c%c\n", hour_num, *(c+hour_bit_size+1), *(c+hour_bit_size+2));
	return 0;
}
