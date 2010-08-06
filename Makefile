# a simple make file for FSM

CC = gcc
LDFLAGS = -lm
# set DEBUG options
ifdef DEBUG
CFLAGS = -Wall -Wextra -ggdb -pg -DDEBUG
else
CFLAGS = -Wall -Os
endif
#name all the object files
OBJS = main.o fsm.o confpar.o 


all : $(OBJS)
	$(CC) $(LDFLAGS) -o httpserve $^

debug :
	make all DEBUG=1	

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $^

doxy :
	doxygen Doxyfile
	sh update_doc.sh	

clean :
	rm -rf $(OBJS) httpserve

cs :
	cscope -b

cscope :
	cscope -b

