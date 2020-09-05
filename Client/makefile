client: client.o utils.o put.o get.o run.o list.o
	cc client.o utils.o put.o get.o run.o list.o -o client
client.o: client.c utils.h put.h get.h run.h list.h
	cc -c client.c
utils.o: utils.c utils.h
	cc -c utils.c
put.o: put.c utils.h
	cc -c put.c
get.o: get.c utils.h
	cc -c get.c
run.o: run.c utils.h
	cc -c run.c
list.o: list.c utils.h
	cc -c list.c