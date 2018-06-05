CXXFLAGS = g++ -Wall -std=c++11
RE1 = results/resultEasy1b
RE2 = results/resultEasy2b
RE3 = results/resultEasy3b
RM1 = results/resultMedium1b
RM2 = results/resultMedium2b
RM3 = results/resultMedium3b
RH1 = results/resultHard1
RH2 = results/resultHard2
PATHVAR = antColonyOptimization/
#PATHVAR = geneticAlgorithm/
#PATHVAR = iteratedLocalSearch/
#PATHVAR = simulatedAnnealing/
#PATHVAR = tabuSearch/
#PATHVAR = orginalCode/antColonyOptimizationOriginal/
#PATHVAR = orginalCode/geneticAlgorithmOrginal/
#PATHVAR = orginalCode/iteratedLocalSearchOrginal/
#PATHVAR = orginalCode/simulatedAnnealingOrginal/
#PATHVAR = orginalCode/tabuSearchOrginal/

runWhole: runE1 runE2 runE3 runM1 runM2 runM3 runH1 runH2

runE1: resultReader.out
	./resultReader.out ${PATHVAR}${RE1}

runE2: resultReader.out
	./resultReader.out ${PATHVAR}${RE2}

runE3: resultReader.out
	./resultReader.out ${PATHVAR}${RE3}

runM1: resultReader.out
	./resultReader.out ${PATHVAR}${RM1}

runM2: resultReader.out
	./resultReader.out ${PATHVAR}${RM2}

runM3: resultReader.out
	./resultReader.out ${PATHVAR}${RM3}

runH1: resultReader.out
	./resultReader.out ${PATHVAR}${RH1}

runH2: resultReader.out
	./resultReader.out ${PATHVAR}${RH2}

resultReader.out: resultReader.cpp
	${CXXFLAGS} resultReader.cpp -o resultReader.out

clean:
	@rm -f *~ *.o
