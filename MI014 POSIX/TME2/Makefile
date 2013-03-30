.PHONY: all creation creation2 creation3 creation4.1 creation4.2
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : creation creation2 creation3 creation4.1 creation4.2


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/creation.o : ${SRC}/creation.c
	${CC} -c -o ${OBJ}/creation.o ${SRC}/creation.c -I${INC}

${BIN}/creation : ${OBJ}/creation.o ${OBJ}/creation.o 
	${CC} -o $@ $^ -lpthread

creation : ${BIN}/creation
	${BIN}/creation

${OBJ}/creation2.o : ${SRC}/creation2.c
	${CC} -c -o ${OBJ}/creation2.o ${SRC}/creation2.c -I${INC}

${BIN}/creation2 : ${OBJ}/creation2.o ${OBJ}/creation2.o 
	${CC} -o $@ $^ -lpthread

creation2 : ${BIN}/creation2
	${BIN}/creation2
	
${OBJ}/creation3.o : ${SRC}/creation3.c
	${CC} -c -o ${OBJ}/creation3.o ${SRC}/creation3.c -I${INC}

${BIN}/creation3 : ${OBJ}/creation3.o ${OBJ}/creation3.o 
	${CC} -o $@ $^ -lpthread

creation3 : ${BIN}/creation3
	${BIN}/creation3
	
${OBJ}/creation4.1.o : ${SRC}/creation4.1.c
	${CC} -c -o ${OBJ}/creation4.1.o ${SRC}/creation4.1.c -I${INC}

${BIN}/creation4.1 : ${OBJ}/creation4.1.o ${OBJ}/creation4.1.o 
	${CC} -o $@ $^ -lpthread

creation4.1 : ${BIN}/creation4.1
	${BIN}/creation4.1
	
${OBJ}/creation4.2.o : ${SRC}/creation4.2.c
	${CC} -c -o ${OBJ}/creation4.2.o ${SRC}/creation4.2.c -I${INC}

${BIN}/creation4.2 : ${OBJ}/creation4.2.o ${OBJ}/creation4.2.o 
	${CC} -o $@ $^ -lpthread

creation4.2 : ${BIN}/creation4.2
	${BIN}/creation4.2
	
${OBJ}/creation5.o : ${SRC}/creation5.c
	${CC} -c -o ${OBJ}/creation5.o ${SRC}/creation5.c -I${INC}

${BIN}/creation5 : ${OBJ}/creation5.o ${OBJ}/creation5.o 
	${CC} -o $@ $^ -lpthread

creation5 : ${BIN}/creation5
	${BIN}/creation5
