.PHONY: all  fileMessages partageMemoire multi file_unique semaphore prod_cons
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : fileMessages partageMemoire multi file_unique semaphore prod_cons


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/fileMessages.o : ${SRC}/fileMessages.c
	${CC} -c -o ${OBJ}/fileMessages.o ${SRC}/fileMessages.c -I${INC}

${BIN}/fileMessages : ${OBJ}/fileMessages.o 
	${CC} -o $@ $^ 

fileMessages : ${BIN}/fileMessages
	${BIN}/fileMessages
	


${OBJ}/partageMemoire.o : ${SRC}/partageMemoire.c
	${CC} -c -o ${OBJ}/partageMemoire.o ${SRC}/partageMemoire.c -I${INC}

${BIN}/partageMemoire : ${OBJ}/partageMemoire.o 
	${CC} -o $@ $^ 

partageMemoire : ${BIN}/partageMemoire
	${BIN}/partageMemoire
	
	
	
${OBJ}/multi.o : ${SRC}/multi.c
	${CC} -c -o ${OBJ}/multi.o ${SRC}/multi.c -I${INC}

${BIN}/multi : ${OBJ}/multi.o 
	${CC} -o $@ $^ 

multi : ${BIN}/multi
	${BIN}/multi
	
	
	
${OBJ}/semaphore.o : ${SRC}/semaphore.c
	${CC} -c -o ${OBJ}/semaphore.o ${SRC}/semaphore.c -I${INC}

${BIN}/semaphore : ${OBJ}/semaphore.o 
	${CC} -o $@ $^ 

semaphore : ${BIN}/semaphore
	${BIN}/semaphore



${OBJ}/file_unique.o : ${SRC}/file_unique.c
	${CC} -c -o ${OBJ}/file_unique.o ${SRC}/file_unique.c -I${INC}

${BIN}/file_unique : ${OBJ}/file_unique.o 
	${CC} -o $@ $^ 

file_unique : ${BIN}/file_unique
	${BIN}/file_unique



${OBJ}/prod_cons.o : ${SRC}/prod_cons.c
	${CC} -c -o ${OBJ}/prod_cons.o ${SRC}/prod_cons.c -I${INC}

${BIN}/prod_cons : ${OBJ}/prod_cons.o 
	${CC} -o $@ $^ -lpthread

prod_cons : ${BIN}/prod_cons
	${BIN}/prod_cons
