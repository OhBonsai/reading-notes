#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <wait.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void unix_error(char *msg);
void posix_error(int code, char *msg);
void sio_error(char *msg);

pid_t Fork(void);
void Execve(const char *filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
unsigned int Sleep(unsigned int secs);
void Pause(void);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp();



