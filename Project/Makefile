all: http_server
http_server:http_server.o lib.o
http_server.o: http_server.c lib.h
lib.o: lib.c lib.h
clean:
	-rm *.o
