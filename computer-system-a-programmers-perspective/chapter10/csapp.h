#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define DEF_MODE   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH
#define DEF_UMASK  S_IWGRP|S_IWOTH


typedef struct sockaddr SA;
extern int h_errno;        /* defined by BIND for DNS errors */
extern char **environ;    / * defined by libc */

#define MAXLINE 8192
#define MAXBUF 8192
#define LISTENQ 1024


#define RIO_BUFSIZE 8192
type def struct {
		int rio_fd;
		int rio_cnt;
		char *rio_bufptr;
		char rio_buf[RIO_BUFSIZE];
} rio_t;



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

/* Signal wrappers */

typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);


/* Sio (Signal-safe I/O) routines*/
ssize_t sio_puts(char s[]);
ssize_t sio_putl(long v);
void sio_error(char s[]);

ssize_t Sio_puts(char []);
ssize_t Sio_putl(long v);
void Sio_error(char s[]);


/* Unix IO Wrapper */
int Open(const char *pathname, int flags, mode_t mode);
void Close(int fd);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *userbuf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);


ssize_t Rio_readn(int fd, void *usrbuf, size_t n);
void Rio_writen(int fd, void *userbuf, size_t n);
void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
