#include <stdio.h>

int main(void){
	int day, month, year;

	printf("Enter a date (mm/dd/yyyy): \n");
	scanf("%d/%d/%d", &day, &month, &year);
	
	printf("You enterd the date, %.4d%.2d%.2d\n", year, month, day);

	return 0;
}
