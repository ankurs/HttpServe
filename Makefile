LIBXMLCFLAGS = `xml2-config --cflags`
LIBXMLLDFLAGS = `xml2-config --libs`
LIBEVENTCFLAGS = -I/usr/local/include
LIBEVENTLDFLAGS = -L/usr/local/lib -levent

HTTP_PARSER_CFLAGS = -Ihttp-parser/

CC = gcc $(LIBXMLCFLAGS) $(LIBEVENTCFLAGS) -fPIC $(HTTP_PARSER_CFLAGS)
LDFLAGS = -lm $(LIBXMLLDFLAGS) $(LIBEVENTLDFLAGS) -ldl -rdynamic -lpthread

# set DEBUG options
ifdef DEBUG
CFLAGS = -Wall -Wextra -ggdb -pg -DDEBUG
else
CFLAGS = -Wall -O2
endif

#name all the object files
OBJS = main.o fsm.o confpar.o libe.o  http-parser/http_parser.o
SHAREDOBJS = stest.o
SHAREDLIBS = sharedtest.so.1

all : httpserve libs 

httpserve : $(OBJS)
	$(CC) $(LDFLAGS) -o httpserve $^

libs: $(SHAREDOBJS) $(SHAREDLIBS)

debug :
	make all DEBUG=1

sharedtest.so.1 : stest.o
	$(CC) $(CFLAGS) -shared -Wl,-soname,$@ -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $^

doxy :
	doxygen Doxyfile
	sh update_doc.sh	

clean :
	rm -rf $(OBJS) $(SHAREDOBJS) $(SHAREDLIBS) httpserve doc/ manual.pdf

cs :
	cscope -bR

cscope :
	cscope -bR

