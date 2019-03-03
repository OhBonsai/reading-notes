void unix_error(char *msg) 
{
		fprintf(stderr, "%s: %s\n", msg, strerror(errorno));
		exit(0);
}

void posix_error(int code, char *msg)
{
		fprintf(stderr, "%s: %s\n", msg, strerror(errorno));
		exit(0);
}


