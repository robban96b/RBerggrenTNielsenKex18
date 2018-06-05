# Makefile for the ACO-ACS timetaling program
# ---
# Sunday March 10 2002
#

#all: ACO-ACS debug_ACO-ACS

runAllTest: runHard1 runHard2 runMedium1 runMedium2 runMedium3 runMedium4 runMedium5 runEasy1 runEasy2 runEasy3 runEasy4 runEasy5

all: ACO-ACS

run: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy01.tim -t 300 -n 100 -s 1337 -p 1 -o result

OBJECTS = Control.o Timer.o Random.o util.o Problem.o Solution.o Ant.o ACO-ACS.o
#CFLAGS = -Wall -ansi
CFLAGS = -Wall -ansi -O3 -std=c++98

ACO-ACS: $(OBJECTS)
	g++ $(OBJECTS) $(CFLAGS) -o timetabling.aco.be.1

runEasy: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy01.tim -t 90 -n 20 -s 1337 -p 1 -o resultEasy

runMedium: ACO-ACS
	./timetabling.aco.be.1 -i ../data/medium01.tim -t 900 -n 20 -s 1337 -p 2 -o resultMedium

runHard: ACO-ACS
	./timetabling.aco.be.1 -i ../data/hard01.tim -t 9000 -n 20 -s 1337 -p 3 -o resultHard




runEasy1: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy01.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy1

runEasy2: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy02.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy2

runEasy3: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy03.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy3

runEasy4: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy04.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy4

runEasy5: ACO-ACS
	./timetabling.aco.be.1 -i ../data/easy05.tim -t 90 -n 5 -s 1337 -p 1 -o results/resultEasy5

runMedium1: ACO-ACS
	./timetabling.aco.be.1 -i ../data/medium01.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium1

runMedium2: ACO-ACS
	./timetabling.aco.be.1 -i ../data/medium02.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium2

runMedium3: ACO-ACS
	./timetabling.aco.be.1 -i ../data/medium03.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium3

runMedium4: ACO-ACS
	./timetabling.aco.be.1 -i ../data/medium04.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium4

runMedium5: ACO-ACS
	./timetabling.aco.be.1 -i ../data/medium05.tim -t 450 -n 4 -s 1337 -p 2 -o results/resultMedium5

runHard1: ACO-ACS
	./timetabling.aco.be.1 -i ../data/hard01.tim -t 3600 -n 2 -s 1337 -p 3 -o results/resultHard1

runHard2: ACO-ACS
	./timetabling.aco.be.1 -i ../data/hard02.tim -t 3600 -n 2 -s 1337 -p 3 -o results/resultHard2





Timer.o: Timer.C Timer.h
	g++ $(CFLAGS) -c Timer.C
Random.o: Random.cc Random.h
	g++ $(CFLAGS) -c Random.cc
Control.o: Control.cpp Control.h Timer.h Solution.h
	g++ $(CFLAGS) -c Control.cpp
util.o: util.cpp util.h
	g++ $(CFLAGS) -c util.cpp
Problem.o: Problem.cpp Problem.h util.h
	g++ $(CFLAGS) -c Problem.cpp
Solution.o: Solution.cpp Solution.h Problem.h Timer.h
	g++ $(CFLAGS) -c Solution.cpp
Ant.o: Ant.cpp Ant.h Problem.h Solution.h
	g++ $(CFLAGS) -c Ant.cpp
ACO-ACS.o: ACO-ACS.cpp Random.h Problem.h util.h Solution.h Timer.h Control.h
	g++ $(CFLAGS) -c ACO-ACS.cpp


CDEBUG = -g -DDEBUG
DEBUG_OBJECTS = debug_Timer.o debug_Random.o debug_Control.o debug_util.o debug_Problem.o debug_Solution.o debug_Ant.o debug_ACO-ACS.o

debug: debug_ACO-ACS

debug_ACO-ACS: $(DEBUG_OBJECTS)
	g++ $(DEBUG_OBJECTS) $(CDEBUG) $(CFLAGS) -o debug_ACO-ACS

debug_Timer.o: Timer.C Timer.h
	g++ $(CDEBUG) $(CFLAGS) -c Timer.C -o debug_Timer.o
debug_Random.o: Random.cc Random.h
	g++ $(CDEBUG) $(CFLAGS) -c Random.cc -o debug_Random.o
debug_Control.o: Control.h Control.cpp Timer.h Solution.h
	g++ $(CDEBUG) $(CFLAGS) -c Control.cpp -o debug_Control.o
debug_util.o: util.cpp util.h
	g++ $(CDEBUG) $(CFLAGS) -c util.cpp -o debug_util.o
debug_Problem.o: Problem.cpp Problem.h util.h
	g++ $(CDEBUG) $(CFLAGS) -c Problem.cpp -o debug_Problem.o
debug_Solution.o: Solution.cpp Solution.h Problem.h Timer.h
	g++ $(CDEBUG) $(CFLAGS) -c Solution.cpp -o debug_Solution.o
debug_Ant.o: Ant.cpp Ant.h Problem.h Solution.h
	g++ $(CDEBUG) $(CFLAGS) -c Ant.cpp -o debug_Ant.o
debug_ACO-ACS.o: ACO-ACS.cpp Random.h Problem.h util.h Solution.h Timer.h Control.h
	g++ $(CDEBUG) $(CFLAGS) -c ACO-ACS.cpp -o debug_ACO-ACS.o


.PHONY: clean
clean:
	rm timetabling.aco.be.1 $(OBJECTS) debug_ACO-ACS $(DEBUG_OBJECTS) *~
