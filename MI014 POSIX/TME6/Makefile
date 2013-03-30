.PHONY: all analyse mycp mapping echange posixL cL extended-grep inverseur pread
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : analyse mycp mapping echange posixL cL extended-grep inverseur pread


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/analyse.o : ${SRC}/analyse.c
	${CC} -c -o ${OBJ}/analyse.o ${SRC}/analyse.c -I${INC}

${BIN}/analyse : ${OBJ}/analyse.o 
	${CC} -o $@ $^ 

analyse : ${BIN}/analyse
	${BIN}/analyse
	
	
	
${OBJ}/mycp.o : ${SRC}/mycp.c
	${CC} -c -o ${OBJ}/mycp.o ${SRC}/mycp.c -I${INC}

${BIN}/mycp : ${OBJ}/mycp.o 
	${CC} -o $@ $^ 

mycp : ${BIN}/mycp
	${BIN}/mycp f1 f2
	
	
	
${OBJ}/mapping.o : ${SRC}/mapping.c
	${CC} -c -o ${OBJ}/mapping.o ${SRC}/mapping.c -I${INC}

${BIN}/mapping : ${OBJ}/mapping.o 
	${CC} -o $@ $^ 

mapping : ${BIN}/mapping
	${BIN}/mapping f3 f4
	
	

${OBJ}/echange.o : ${SRC}/echange.c
	${CC} -c -o ${OBJ}/echange.o ${SRC}/echange.c -I${INC}

${BIN}/echange : ${OBJ}/echange.o 
	${CC} -o $@ $^ 

echange : ${BIN}/echange
	${BIN}/echange 
	
	
	
${OBJ}/posixL.o : ${SRC}/posixL.c
	${CC} -c -o ${OBJ}/posixL.o ${SRC}/posixL.c -I${INC}

${BIN}/posixL : ${OBJ}/posixL.o 
	${CC} -o $@ $^ 

posixL : ${BIN}/posixL
	${BIN}/posixL posixVSc
	
	
	
${OBJ}/cL.o : ${SRC}/cL.c
	${CC} -c -o ${OBJ}/cL.o ${SRC}/cL.c -I${INC}

${BIN}/cL : ${OBJ}/cL.o 
	${CC} -o $@ $^ 

cL : ${BIN}/cL
	${BIN}/cL posixVSc



${OBJ}/extended-grep.o : ${SRC}/extended-grep.c
	${CC} -c -o ${OBJ}/extended-grep.o ${SRC}/extended-grep.c -I${INC}

${BIN}/extended-grep : ${OBJ}/extended-grep.o 
	${CC} -o $@ $^ 

extended-grep : ${BIN}/extended-grep
	${BIN}/extended-grep test /home/xin/桌面/POSIX/TME6




${OBJ}/inverseur.o : ${SRC}/inverseur.c
	${CC} -c -o ${OBJ}/inverseur.o ${SRC}/inverseur.c -I${INC}

${BIN}/inverseur : ${OBJ}/inverseur.o 
	${CC} -o $@ $^ 

inverseur : ${BIN}/inverseur
	${BIN}/inverseur posixVSc



${OBJ}/pread.o : ${SRC}/pread.c
	${CC} -c -o ${OBJ}/pread.o ${SRC}/pread.c -I${INC}

${BIN}/pread : ${OBJ}/pread.o 
	${CC} -o $@ $^ 

pread : ${BIN}/pread
	${BIN}/pread posixVSc
