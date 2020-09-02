client: client.o utils.o put.o get.o
	cc client.o utils.o put.o get.o -o client
client.o: client.c utils.h put.h get.h
	cc -c client.c
utils.o: utils.c utils.h
	cc -c utils.c
put.o: put.c utils.h
	cc -c put.c
get.o: get.c utils.h
	cc -c get.c