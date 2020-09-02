server: server.o utils.o put.o get.o
	cc server.o utils.o put.o get.o -o server
server.o: server.c utils.h put.h get.h
	cc -c server.c
utils.o: utils.c utils.h
	cc -c utils.c
put.o: put.c utils.h
	cc -c put.c
get.o: get.c utils.h
	cc -c get.c