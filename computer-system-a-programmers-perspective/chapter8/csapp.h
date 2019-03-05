#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <wait.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void unix_error(char *msg);
void posix_error(int code, char *msg);


pid_t Fork(void);
