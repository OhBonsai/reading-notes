#include "csapp.h"

// 这个函数有问题,由于信号只会存储一个
// 所以第三个来的时候,会被丢弃,所以第三个子进程变成僵尸进程
void handler1(int sig)
{
		int oldErrno = errno;

		if ((waitpid(-1, NULL, 0)) < 0)
				sio_eror("waitpid error");

		Sio_puts("Handler reaped child\n");
		Sleep(1);
		errno = olderrno;
}


// 
void handler2(int sig)
{
		int oldErrno = errno;

		while ((waitpid(-1, NULL, 0)) < 0) {
		    Sio_puts("Handler reaped child\n");
		}
		Sleep(1);
		errno = olderrno;
}

int main()
{
		int i, n;
		char buf[MAXBUF];

		if (signal(SIGCHLD, handler1) == SIG_ERR)
				unix_error("signal error");

		for (i =0 ; i<3; i++) {
				if (Fork() == 0){
						printf("Hello from child %d\n", (int)getpid());
						exit(0);
				}
		}

		if ((n=read(STDIN_FILENO, buf, sizeof(buf))) < 0)
				unix_error("read");

		printf("Parent processing input\n");
		while(1);
		exit(0);

}
