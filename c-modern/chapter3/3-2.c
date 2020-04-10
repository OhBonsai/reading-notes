#include <stdio.h>

int main(void){
	int number;
	char *date;
	float price;

	printf("Enter item number\n");
	scanf("%d", &number);

	printf("Enter unit price\n");
	scanf("%f", &price);

	if (price > 9999.99) {
		printf("price must lower than 9999.99");
		return 1;
	}

	printf("Enter a date (mm/dd/yyyy): \n");
	scanf("%s", date);

	printf("Item\tUnit\tPurchase\n");
	printf("\tPrice\tDate\n");

	printf("%d\t$%6.2f\t%s\n", number,price, date);
	
	

	return 0;
}
