TARGET = timetabling.ga.uk.2
CXXFLAGS = -Wall -ansi -O3

OBJS = Problem.o Solution.o Random.o Timer.o Control.o util.o

runAllTest: runHard1 runHard2 runMedium1 runMedium2 runMedium3 runMedium4 runMedium5 runEasy1 runEasy2 runEasy3 runEasy4 runEasy5

all: ${TARGET}

runEasy: ${TARGET}
	./${TARGET} -i ../data/easy01.tim -t 90 -n 20 -s 1337 -p 1 -o resultEasy

runMedium: ${TARGET}
	./${TARGET} -i ../data/medium01.tim -t 900 -n 20 -s 1337 -p 2 -o resultMedium

runHard: ${TARGET}
	./${TARGET} -i ../data/hard01.tim -t 9000 -n 20 -s 1337 -p 3 -o resultHard

timetabling.ga.uk.2: ga.cpp $(OBJS)
	${CXX} ${CXXFLAGS} -o $@ $^

clean:
	@rm -f *~ *.o ${TARGET} core DEADJOE


runEasy1: ${TARGET}
	./${TARGET} -i ../data/easy01.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy1

runEasy2: ${TARGET}
	./${TARGET} -i ../data/easy02.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy2

runEasy3: ${TARGET}
	./${TARGET} -i ../data/easy03.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy3

runEasy4: ${TARGET}
	./${TARGET} -i ../data/easy04.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy4

runEasy5: ${TARGET}
	./${TARGET} -i ../data/easy05.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy5

runMedium1: ${TARGET}
	./${TARGET} -i ../data/medium01.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium1

runMedium2: ${TARGET}
	./${TARGET} -i ../data/medium02.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium2

runMedium3: ${TARGET}
	./${TARGET} -i ../data/medium03.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium3

runMedium4: ${TARGET}
	./${TARGET} -i ../data/medium04.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium4

runMedium5: ${TARGET}
	./${TARGET} -i ../data/medium05.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium5

runHard1: ${TARGET}
	./${TARGET} -i ../data/hard01.tim -t 3600 -n 2 -s 1337 -p 3 -o results/resultHard1

runHard2: ${TARGET}
	./${TARGET} -i ../data/hard02.tim -t 3600 -n 2 -s 1337 -p 3 -o results/resultHard2
