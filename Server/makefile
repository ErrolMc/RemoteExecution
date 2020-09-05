server: server.o utils.o put.o get.o run.o list.o sys.o
	cc server.o utils.o put.o get.o run.o list.o sys.o -o server
server.o: server.c utils.h put.h get.h run.h list.h sys.h
	cc -c server.c
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
sys.o: sys.c utils.h
	cc -c sys.c