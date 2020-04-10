#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int roll_dice(void);
_Bool play_game(void);

int main(void) {
	play_game();
	
	while(1) {
		printf("Play again? ");
		char *a;
		scanf("%s", a);
		if (*a != 'y') {
			return 0;
		} else {
			printf("\n");
			play_game();
		}
	}

}

int roll_dice(void) {
	return (rand() % 6 + 1) + (rand() % 6 + 1);
}

_Bool play_game(void) {
	int dice_1 = roll_dice();

	printf("You rolled: %d\n", dice_1);
	if (dice_1 == 7 || dice_1 == 11) {
		printf("You win!\n");
		return true;
	} else if (dice_1 == 2 || dice_1 == 3 || dice_1 == 12) {
		printf("You lose!\n");
		return false;
	} else {
		printf("Your point is %d\n", dice_1);
	}


	while(1) {
		int dice_n = roll_dice();
		printf("You rolled %d\n", dice_n);

		if (dice_n == dice_1) {
			printf("You win!\n");
			return true;
		} else if (dice_n == 7) {
			printf("You lose!\n");
			return false;
		} else {
		
		}
	}
}

