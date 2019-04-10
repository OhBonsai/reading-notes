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


/*
 * Wrapper for Unix signal functions
 */
handler_t *Signal(int signum, handler_t *handler)
{
		struct sigaction action, old_action;
		action.sa_handler = handler;  
		sigemptyset(&action.sa_mask); /* Block sigs of type being handled */
		action.sa_flags = SA_RESTART; /* Restart syscalls if possible */
		
		if (sigaction(signum, &action, &old_action) < 0)
				unix_error("Signal error");
		return (old_action.sa_handler);
}

/*
 *  signal-safe I/O
 */

static void sio_reverse(char s[])
{
		int c, i, j;
		for (i=0, j=strlen(s)-1; i<j; i++, j--){
				c = s[i];
				s[i] = s[j];
				s[j] = c;
		}
}

static void sio_ltoa(long v, char s[], int b)
{
		int c, i = 0;
		do {
				s[i++] = ((c = (v % b)) < 10) ? c + '0' : c - 10 + 'a';
		} while((v /= b ) > 0);
		s[i] = '\0';
		sio_reverse(s);
}


static size_t sio_strlen(char s[])
{
		int i = 0;
		while(s[i] != '\0')
				++i;
		return i;
}

ssize_t sio_puts(char s[])
{
		return write(STDOUT_FILENO, s, sio_strlen(s));
}

ssize_t sio_putl(long v)
{
		char s[128];
		sio_ltoa(v, s, 10);
		return sio_puts(s);
}

void sio_error(char s[])
{
		sio_puts(s);
		_exit(1);
}


/**********************
 * Wrappers for the sio routines
 */
ssize_t Sio_putl(long v)
{
		ssize_t n;
		if ((n = sio_putl(v)) < 0)
				sio_error("Sio_putl error");
		return n;
}

ssize_t Sio_puts(char s[])
{
		ssize_t n;
		if ((n = sio_puts(s)) < 0)
				sio_error("Sio_puts error");
		return n;
}

void Sio_error(char s[])
{
		sio_error(s);
}


/*****************
 * Wrapper for the Unix IO
 */
int Open(const char *pathname, int flags, mode_t mode)
{

		int rc;

		if ((rc=open(pathname, flags, mode)) <  0)
				unix_error("open error");
		return rc;
}


void Close(int fd)
{
		int rc;
		if ((rc=close(fd)) < 0)
				unix_error("Close error");

}


ssize_t Read(int fd, void *buf, size_t count)
{
	ssize_t rc;

	if ((rc = read(fd, buf, count)) < 0)
			unix_error("Read error");
	return rc;
}

ssize_t Write(int fd, const void *buf, size_t count)
{
	ssize_t rc;
	if ((rc = write(fd, buf, count)) < 0)
			unix_error("Write error");
	return rc;
}


ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;


	while (nleft > 0) {
		if((nread = read(fd, bufp, nleft)) < 0) {
				if (errno == EINTR)
						nread = 0;
				else 
						return -1;

		}

		else if (nread == 0)
				break;
		nleft -= nread;
		bufp += nread;

	}
	return (n-nleft);
}

ssize_t rio_writen(int fd, void *userbuf, size_t n)
{
		size_T nleft = n;
		ssize_t nwritten;
		char *bufp = usrbuf;

		while (nleft > 0){
				if ((nwritten = write(fd, bufp, nleft)) <= 0) {
						if (errno == EINTR)
								nwritten = 0;
						else 
								return -1;
						nleft -= nwritten;
						bufp += nwritten;
				}
		}
		return n;

}
	
