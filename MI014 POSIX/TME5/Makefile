.PHONY: all  memoirePartagee semaphore serveur diner
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : memoirePartagee semaphore serveur diner


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/memoirePartagee.o : ${SRC}/memoirePartagee.c
	${CC} -c -o ${OBJ}/memoirePartagee.o ${SRC}/memoirePartagee.c -I${INC}

${BIN}/memoirePartagee : ${OBJ}/memoirePartagee.o 
	${CC} -o $@ $^ -lrt

memoirePartagee : ${BIN}/memoirePartagee
	${BIN}/memoirePartagee



${OBJ}/semaphore.o : ${SRC}/semaphore.c
	${CC} -c -o ${OBJ}/semaphore.o ${SRC}/semaphore.c -I${INC}

${BIN}/semaphore : ${OBJ}/semaphore.o 
	${CC} -o $@ $^ -lpthread

semaphore : ${BIN}/semaphore
	${BIN}/semaphore




${OBJ}/chat_server.o : ${INC}/chat_server.h ${SRC}/chat_server.c
	${CC} -c -o ${OBJ}/chat_server.o ${SRC}/chat_server.c -I${INC}

${BIN}/chat_server : ${OBJ}/chat_server.o
	${CC} -o $@ $^ -lpthread -lrt
	
${OBJ}/chat_client.o : ${INC}/chat_server.h ${SRC}/chat_client.c
	${CC} -c -o ${OBJ}/chat_client.o ${SRC}/chat_client.c -I${INC}

${BIN}/chat_client : ${OBJ}/chat_client.o
	${CC} -o $@ $^ -lpthread -lrt

serveur : ${BIN}/chat_server ${BIN}/chat_client
	${BIN}/chat_server 123456
	
	
	
${OBJ}/diner.o : ${SRC}/diner.c
	${CC} -c -o ${OBJ}/diner.o ${SRC}/diner.c -I${INC}

${BIN}/diner : ${OBJ}/diner.o ${OBJ}/diner.o 
	${CC} -o $@ $^ -lpthread -lrt

diner : ${BIN}/diner
	${BIN}/diner
