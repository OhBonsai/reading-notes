#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define STACK_SIZE 100

char stack[STACK_SIZE];
int top = 0;

void make_empty(void) {
	top = 0;
}

bool is_empty(void) {
	return top == 0;
}


bool is_full(void) {
	return top == STACK_SIZE;
}


void push(char i) {
	if (is_full()) {
		printf("stack overflow!\n");
	} else {
		stack[top++] = i;
	}
}


char pop(void) {
	printf("do pop");
	if (is_empty()) { 
		printf("stack underflow!\n");
		exit(1);
	} else { 
		return stack[--top];
	}
	

}



int main(void) {
	char *c;
	printf("Enter parenteses and/or braces: ");
	scanf("%s", c);

	for(int i=0;;i++) {
		char cc = *(c+i);
		if (cc  == '\0') {
			break;
		} else if ( cc == ')' & stack[top-1] == '(') {
			pop();
		} else if ( cc == ']' & stack[top-1] == '[' ) {
			pop();
		} else if ( cc == '}' & stack[top-1] == '{')  {
			pop();
		} else {
			push(cc);
		}

		printf("%c\n", *(c+i));
	}

	printf("top is %d\n", top);
	printf("Enter value is %s\n", &stack[0]);
	
	return 0;
}
