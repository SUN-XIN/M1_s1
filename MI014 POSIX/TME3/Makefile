.PHONY: all conversion conversion2 thread_stack thread_concur chaine
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : conversion conversion2 thread_stack thread_concur chaine


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/conversion.o : ${SRC}/conversion.c
	${CC} -c -o ${OBJ}/conversion.o ${SRC}/conversion.c -I${INC}

${BIN}/conversion : ${OBJ}/conversion.o 
	${CC} -o $@ $^ -lpthread

conversion : ${BIN}/conversion
	${BIN}/conversion ${LIB}/f1.txt ${LIB}/f2.txt ${LIB}/f3.txt



${OBJ}/conversion2.o : ${SRC}/conversion2.c
	${CC} -c -o ${OBJ}/conversion2.o ${SRC}/conversion2.c -I${INC}

${BIN}/conversion2 : ${OBJ}/conversion2.o 
	${CC} -o $@ $^ -lpthread

conversion2 : ${BIN}/conversion2
	${BIN}/conversion2 ${LIB}/f1.txt ${LIB}/f2.txt ${LIB}/f3.txt ${LIB}/f4.txt ${LIB}/f5.txt



${OBJ}/thread_stack.o : ${INC}/thread_stack.h ${SRC}/thread_stack.c
	${CC} -c -o ${OBJ}/thread_stack.o ${SRC}/thread_stack.c -I${INC}

${BIN}/thread_stack : ${OBJ}/thread_stack.o 
	${CC} -o $@ $^ -lpthread

thread_stack : ${BIN}/thread_stack
	${BIN}/thread_stack 



${OBJ}/thread_concur.o : ${INC}/thread_concur.h ${SRC}/thread_concur.c
	${CC} -c -o ${OBJ}/thread_concur.o ${SRC}/thread_concur.c -I${INC}

${BIN}/thread_concur : ${OBJ}/thread_concur.o 
	${CC} -o $@ $^ -lpthread

thread_concur : ${BIN}/thread_concur
	${BIN}/thread_concur 



${OBJ}/chaine.o : ${SRC}/chaine.c
	${CC} -c -o ${OBJ}/chaine.o ${SRC}/chaine.c -I${INC}

${BIN}/chaine : ${OBJ}/chaine.o 
	${CC} -o $@ $^ -lpthread

chaine : ${BIN}/chaine
	${BIN}/chaine 

