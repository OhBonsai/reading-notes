#include "csapp.h"


jmp_buf buf;

int error1 = 0;
int error2 = 1;

void foo(void), bar(void);

int main()
{
		switch(setjmp(buf)) {
				case 0:
						foo();
						break;
				case 1:
						printf("Detected an eror1 condition in foo\n");
						break;
				case 2:
	                    printf("Detected an eror2 condition in foo\n");
						break;
				default:
						printf("unknown error condition in foo\n");


		}

		exit(0);
}

void foo(void)
{
		if (error1)
				longjmp(buf, 1);
		bar()
}


void bar(void)
{
		if (error2)
				longjmp(buf, 2);
}
