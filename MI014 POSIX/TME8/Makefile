.PHONY: all miniServeur mychatTest sonar connex
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : miniServeur mychatTest sonar connex


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/miniServeur.o : ${SRC}/miniServeur.c
	${CC} -c -o ${OBJ}/miniServeur.o ${SRC}/miniServeur.c -I${INC}

${BIN}/miniServeur : ${OBJ}/miniServeur.o 
	${CC} -o $@ $^ -pthread

miniServeur : ${BIN}/miniServeur
	${BIN}/miniServeur 7100 fs 



${BIN}/mychat : ${SRC}/mychat.c
	${CC} -o ${BIN}/mychat ${SRC}/mychat.c 

${BIN}/serverChat : ${SRC}/serverChat.c
	${CC} -o ${BIN}/serverChat ${SRC}/serverChat.c 
	
mychatTest : ${BIN}/mychat ${BIN}/serverChat
	${BIN}/serverChat 225.0.0.10 8100 &
	${BIN}/mychat 225.0.0.10 8100 demande
	
	
	
${BIN}/mySonar : ${SRC}/mySonar.c
	${CC} -o ${BIN}/mySonar ${SRC}/mySonar.c -pthread

${BIN}/myPong : ${SRC}/myPong.c
	${CC} -o ${BIN}/myPong ${SRC}/myPong.c 
	
sonar : ${BIN}/mySonar ${BIN}/myPong
	${BIN}/mySonar &
	${BIN}/myPong
	
	

${OBJ}/connexCli.o : ${SRC}/connexCli.c
	${CC} -c -o ${OBJ}/connexCli.o ${SRC}/connexCli.c -I${INC}

${BIN}/connexCli : ${OBJ}/connexCli.o
	${CC} -o $@ $^
	
${OBJ}/connexServ.o : ${SRC}/connexServ.c
	${CC} -c -o ${OBJ}/connexServ.o ${SRC}/connexServ.c -I${INC}

${BIN}/connexServ : ${OBJ}/connexServ.o
	${CC} -o $@ $^

connex : ${BIN}/connexServ ${BIN}/connexCli
	${BIN}/connectserver 1234 2345 3456 4567 5678 6789
