#include "csapp.h"

void unix_error(char *msg) 
{
		fprintf(stderr, "%s: %s\n", msg, strerror(errno));
		exit(0);
}

void posix_error(int code, char *msg)
{
		fprintf(stderr, "%s: %s\n", msg, strerror(code));
		exit(0);
}

pid_t Fork(void)
{
		pid_t pid;
		
		if ((pid=fork()) < 0)
				unix_error("Fork error");
		return pid;
}

void Kill(pid_t pid, int signum)
{
		int rc;

		if ((rc = kill(pid, signum)) < 0)
				unix_error("Kill error");
}

void Pause(void)
{
		(void)pause();
		return;
}

unsigned int Sleep(unsigned int secs)
{
		unsigned int rc;

		if ((rc=sleep(secs)) < 0)
				unix_error("Sleep error");
		return rc;
}
