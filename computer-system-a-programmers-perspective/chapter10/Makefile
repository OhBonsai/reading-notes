CC = gcc

CSAPP_INC = .
CSAPP_SRC = .

# WALL允许发出告警信息
# -O2 二级别优化

CFLAGS = -Wall -O2 -I $(CSAPP_INC) 
LZDLIBS = -lpthread


PROGS = statcheck\
		cpfile\
		cpstdin\
		hello\
		ffiles1\
		ffiles2\
		ffiles3\
		openfile\


all: $(CSAPP_SRC)/csapp.o $(PROGS) 

$(PROGS): $(CSAPP_SRC)/csapp.o

$(CSAPP_SRC)/csapp.o: $(CSAPP_SRC)/csapp.c $(CSAPP_INC)/csapp.h




clean:
		rm -f $(PROGS) *.o *~
