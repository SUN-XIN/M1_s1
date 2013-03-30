.PHONY: all asynchrones temporisateur remontee recopie sigTR remonteeSigTR
CC=gcc -Wall -ansi 
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : asynchrones temporisateur remontee recopie sigTR remonteeSigTR


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/asynchrones.o : ${SRC}/asynchrones.c
	${CC} -c -o ${OBJ}/asynchrones.o ${SRC}/asynchrones.c -I${INC}

${BIN}/asynchrones : ${OBJ}/asynchrones.o 
	${CC} -o $@ $^ -lrt

asynchrones : ${BIN}/asynchrones
	${BIN}/asynchrones asyn abcd



${OBJ}/temporisateur.o : ${SRC}/temporisateur.c
	${CC} -c -o ${OBJ}/temporisateur.o ${SRC}/temporisateur.c -I${INC}

${BIN}/temporisateur : ${OBJ}/temporisateur.o 
	${CC} -o $@ $^ -lrt

temporisateur : ${BIN}/temporisateur
	${BIN}/temporisateur tempor abcd
	
	
	
${OBJ}/remontee.o : ${SRC}/remontee.c
	${CC} -c -o ${OBJ}/remontee.o ${SRC}/remontee.c -I${INC}

${BIN}/remontee : ${OBJ}/remontee.o 
	${CC} -o $@ $^ -lrt

remontee : ${BIN}/remontee
	${BIN}/remontee f



${OBJ}/recopie.o : ${SRC}/recopie.c
	${CC} -c -o ${OBJ}/recopie.o ${SRC}/recopie.c -I${INC}

${BIN}/recopie : ${OBJ}/recopie.o 
	${CC} -o $@ $^ -lrt

recopie : ${BIN}/recopie
	${BIN}/recopie copie
	
	
	
${OBJ}/sigTR.o : ${SRC}/sigTR.c
	${CC} -c -o ${OBJ}/sigTR.o ${SRC}/sigTR.c -I${INC}

${BIN}/sigTR : ${OBJ}/sigTR.o 
	${CC} -o $@ $^ 

sigTR : ${BIN}/sigTR
	${BIN}/sigTR 



${OBJ}/remonteeSigTR.o : ${SRC}/remonteeSigTR.c
	${CC} -c -o ${OBJ}/remonteeSigTR.o ${SRC}/remonteeSigTR.c -I${INC}

${BIN}/remonteeSigTR : ${OBJ}/remonteeSigTR.o 
	${CC} -o $@ $^ 

remonteeSigTR : ${BIN}/remonteeSigTR
	${BIN}/remonteeSigTR 
