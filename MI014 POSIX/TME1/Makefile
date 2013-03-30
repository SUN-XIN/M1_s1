.PHONY: all clean max_calcul test_nfork exec execl execv chaine synchro kill
CC=gcc -Wall -ansi
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all : max_calcul test_nfork exec execl execv chaine synchro kill


clean :
	rm -f ${OBJ}/* ${BIN}/*


${OBJ}/max_func.o : ${INC}/max_func.h ${SRC}/max_func.c
	${CC} -c -o ${OBJ}/max_func.o  ${SRC}/max_func.c -I${INC}

${OBJ}/max_calcul.o : ${INC}/max_func.h ${SRC}/max_calcul.c
	${CC} -c -o ${OBJ}/max_calcul.o ${SRC}/max_calcul.c -I${INC}

${BIN}/max_calcul : ${OBJ}/max_calcul.o ${OBJ}/max_func.o 
	${CC} -o $@ $^

max_calcul : ${BIN}/max_calcul
	${BIN}/max_calcul 4 8 12 7



${OBJ}/nfork.o : ${INC}/nfork.h ${SRC}/nfork.c
	${CC} -c -o ${OBJ}/nfork.o  ${SRC}/nfork.c -I${INC}

${OBJ}/test_nfork.o : ${INC}/nfork.h ${SRC}/test_nfork.c
	${CC} -c -o ${OBJ}/test_nfork.o ${SRC}/test_nfork.c -I${INC}

${BIN}/test_nfork : ${OBJ}/test_nfork.o ${OBJ}/nfork.o 
	${CC} -o $@ $^

test_nfork : ${BIN}/test_nfork
	${BIN}/test_nfork
	

${OBJ}/exec.o : ${SRC}/exec.c
	${CC} -c -o ${OBJ}/exec.o ${SRC}/exec.c -I${INC}

${BIN}/exec : ${OBJ}/exec.o ${OBJ}/exec.o 
	${CC} -o $@ $^

exec : ${BIN}/exec
	${BIN}/exec
	
${OBJ}/execl.o : ${SRC}/execl.c
	${CC} -c -o ${OBJ}/execl.o ${SRC}/execl.c -I${INC}

${BIN}/execl : ${OBJ}/execl.o ${OBJ}/execl.o 
	${CC} -o $@ $^

execl : ${BIN}/execl
	${BIN}/execl	
	
${OBJ}/execv.o : ${SRC}/execv.c
	${CC} -c -o ${OBJ}/execv.o ${SRC}/execv.c -I${INC}

${BIN}/execv : ${OBJ}/execv.o ${OBJ}/execv.o 
	${CC} -o $@ $^

execv : ${BIN}/execv
	${BIN}/execv
	

${OBJ}/chaine.o : ${SRC}/chaine.c
	${CC} -c -o ${OBJ}/chaine.o ${SRC}/chaine.c -I${INC}

${BIN}/chaine : ${OBJ}/chaine.o ${OBJ}/chaine.o 
	${CC} -o $@ $^

chaine : ${BIN}/chaine
	${BIN}/chaine 5


${OBJ}/synchro.o : ${SRC}/synchro.c
	${CC} -c -o ${OBJ}/synchro.o ${SRC}/synchro.c -I${INC}

${BIN}/synchro : ${OBJ}/synchro.o ${OBJ}/synchro.o 
	${CC} -o $@ $^

synchro : ${BIN}/synchro
	${BIN}/synchro


${OBJ}/kill.o : ${SRC}/kill.c
	${CC} -c -o ${OBJ}/kill.o ${SRC}/kill.c -I${INC}

${BIN}/kill : ${OBJ}/kill.o ${OBJ}/kill.o 
	${CC} -o $@ $^

kill : ${BIN}/kill
	${BIN}/kill 5
