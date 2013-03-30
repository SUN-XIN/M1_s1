.PHONY: all remonteeVal recopie env envTCP ftp
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : remonteeVal recopie env envTCP ftp


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/remonteeVal.o : ${SRC}/remonteeVal.c
	${CC} -c -o ${OBJ}/remonteeVal.o ${SRC}/remonteeVal.c -I${INC}

${BIN}/remonteeVal : ${OBJ}/remonteeVal.o 
	${CC} -o $@ $^ 

remonteeVal : ${BIN}/remonteeVal
	${BIN}/remonteeVal



${OBJ}/sendfile.o : ${SRC}/sendfile.c
	${CC} -c -o ${OBJ}/sendfile.o ${SRC}/sendfile.c -I${INC}

${BIN}/sendfile : ${OBJ}/sendfile.o 
	${CC} -o $@ $^ 

${OBJ}/recvfile.o : ${SRC}/recvfile.c
	${CC} -c -o ${OBJ}/recvfile.o ${SRC}/recvfile.c -I${INC}

${BIN}/recvfile : ${OBJ}/recvfile.o 
	${CC} -o $@ $^ 

recopie : ${BIN}/recvfile ${BIN}/sendfile
	${BIN}/recvfile 7100 &
	${BIN}/sendfile 127.0.0.1 7100 f_ex2
	
	

${OBJ}/envServeur.o : ${SRC}/envServeur.c
	${CC} -c -o ${OBJ}/envServeur.o ${SRC}/envServeur.c -I${INC}

${BIN}/envServeur : ${OBJ}/envServeur.o 
	${CC} -o $@ $^ 

${OBJ}/envClient.o : ${SRC}/envClient.c
	${CC} -c -o ${OBJ}/envClient.o ${SRC}/envClient.c -I${INC}

${BIN}/envClient : ${OBJ}/envClient.o 
	${CC} -o $@ $^ 

env : ${BIN}/envClient ${BIN}/envServeur
	${BIN}/envServeur &
	${BIN}/envClient



${OBJ}/envServeurTCP.o : ${SRC}/envServeurTCP.c
	${CC} -c -o ${OBJ}/envServeurTCP.o ${SRC}/envServeurTCP.c -I${INC}

${BIN}/envServeurTCP : ${OBJ}/envServeurTCP.o 
	${CC} -o $@ $^ 

${OBJ}/envClientTCP.o : ${SRC}/envClientTCP.c
	${CC} -c -o ${OBJ}/envClientTCP.o ${SRC}/envClientTCP.c -I${INC}

${BIN}/envClientTCP : ${OBJ}/envClientTCP.o 
	${CC} -o $@ $^ 

envTCP : ${BIN}/envClientTCP ${BIN}/envServeurTCP
	${BIN}/envServeurTCP &
	${BIN}/envClientTCP



${OBJ}/ftpServer.o : ${SRC}/ftpServer.c
	${CC} -c -o ${OBJ}/ftpServer.o ${SRC}/ftpServer.c -I${INC}

${BIN}/ftpServer : ${OBJ}/ftpServer.o 
	${CC} -o $@ $^ 

${OBJ}/ftpClient.o : ${SRC}/ftpClient.c
	${CC} -c -o ${OBJ}/ftpClient.o ${SRC}/ftpClient.c -I${INC}

${BIN}/ftpClient : ${OBJ}/ftpClient.o 
	${CC} -o $@ $^ 

envTCP : ${BIN}/ftpClient ${BIN}/ftpServer
	${BIN}/ftpServer 7100 fs &
	${BIN}/ftpClient 7100
