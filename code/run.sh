#!/bin/bash -e

TIMEEASY=80
TIMEMEDIUM=500
TRIESEASY=30
TRIESMEDIUM=20
#Small: -t 80 -n 30
#Medium -t 500 -n 20
echo "--------------------------------------------------"
echo "-------------------ORIGINAL ANT-------------------"
echo "--------------------------------------------------"
echo "Original Ant easy 1"
./orginalCode/antColonyOptimizationOriginal/timetabling.aco.be.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/antColonyOptimizationOriginal/results/resultEasy1b
echo "Original Ant easy 2"
./orginalCode/antColonyOptimizationOriginal/timetabling.aco.be.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/antColonyOptimizationOriginal/results/resultEasy2b
echo "Original Ant easy 3"
./orginalCode/antColonyOptimizationOriginal/timetabling.aco.be.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/antColonyOptimizationOriginal/results/resultEasy3b

echo "Original Ant Medium 1"
./orginalCode/antColonyOptimizationOriginal/timetabling.aco.be.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/antColonyOptimizationOriginal/results/resultMedium1b
echo "Original Ant Medium 2"
./orginalCode/antColonyOptimizationOriginal/timetabling.aco.be.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/antColonyOptimizationOriginal/results/resultMedium2b
echo "Original Ant Medium 3"
./orginalCode/antColonyOptimizationOriginal/timetabling.aco.be.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/antColonyOptimizationOriginal/results/resultMedium3b


echo "--------------------------------------------------"
echo "-------------------EDITED ANT---------------------"
echo "--------------------------------------------------"

echo "Edited Ant easy 1"
./antColonyOptimization/timetabling.aco.be.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o antColonyOptimization/results/resultEasy1b
echo "Edited Ant easy 2"
./antColonyOptimization/timetabling.aco.be.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o antColonyOptimization/results/resultEasy2b
echo "Edited Ant easy 3"
./antColonyOptimization/timetabling.aco.be.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o antColonyOptimization/results/resultEasy3b

echo "Edited Ant Medium 1"
./antColonyOptimization/timetabling.aco.be.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o antColonyOptimization/results/resultMedium1b
echo "Edited Ant Medium 2"
./antColonyOptimization/timetabling.aco.be.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o antColonyOptimization/results/resultMedium2b
echo "Edited Ant Medium 3"
./antColonyOptimization/timetabling.aco.be.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o antColonyOptimization/results/resultMedium3b


echo "--------------------------------------------------"
echo "-----------ORIGINAL GENETIC ALGORITHM-------------"
echo "--------------------------------------------------"

echo "Original GA easy 1"
./orginalCode/geneticAlgorithmOrginal/timetabling.ga.uk.2 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/geneticAlgorithmOrginal/results/resultEasy1b
echo "Original GA easy 2"
./orginalCode/geneticAlgorithmOrginal/timetabling.ga.uk.2 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/geneticAlgorithmOrginal/results/resultEasy2b
echo "Original GA easy 3"
./orginalCode/geneticAlgorithmOrginal/timetabling.ga.uk.2 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/geneticAlgorithmOrginal/results/resultEasy3b

echo "Original GA medium 1"
./orginalCode/geneticAlgorithmOrginal/timetabling.ga.uk.2 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/geneticAlgorithmOrginal/results/resultMedium1b
echo "Original GA medium 2"
./orginalCode/geneticAlgorithmOrginal/timetabling.ga.uk.2 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/geneticAlgorithmOrginal/results/resultMedium2b
echo "Original GA medium 3"
./orginalCode/geneticAlgorithmOrginal/timetabling.ga.uk.2 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/geneticAlgorithmOrginal/results/resultMedium3b


echo "--------------------------------------------------"
echo "---------EDITED GENETIC ALGORITHM-----------------"
echo "--------------------------------------------------"

echo "Edited GA easy 1"
./geneticAlgorithm/timetabling.ga.uk.2 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o geneticAlgorithm/results/resultEasy1b
echo "Edited GA easy 2"
./geneticAlgorithm/timetabling.ga.uk.2 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o geneticAlgorithm/results/resultEasy2b
echo "Edited GA easy 3"
./geneticAlgorithm/timetabling.ga.uk.2 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o geneticAlgorithm/results/resultEasy3b

echo "Edited GA medium 1"
./geneticAlgorithm/timetabling.ga.uk.2 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o geneticAlgorithm/results/resultMedium1b
echo "Edited GA medium 2"
./geneticAlgorithm/timetabling.ga.uk.2 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o geneticAlgorithm/results/resultMedium2b
echo "Edited GA medium 3"
./geneticAlgorithm/timetabling.ga.uk.2 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o geneticAlgorithm/results/resultMedium3b


echo "--------------------------------------------------"
echo "-------------------ORIGINAL ILS-------------------"
echo "--------------------------------------------------"

echo "Original ILS easy 1"
./orginalCode/iteratedLocalSearchOrginal/timetabling.ils.de.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/iteratedLocalSearchOrginal/results/resultEasy1b
echo "Original ILS easy 2"
./orginalCode/iteratedLocalSearchOrginal/timetabling.ils.de.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/iteratedLocalSearchOrginal/results/resultEasy2b
echo "Original ILS easy 3"
./orginalCode/iteratedLocalSearchOrginal/timetabling.ils.de.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/iteratedLocalSearchOrginal/results/resultEasy3b

echo "Original ILS medium 1"
./orginalCode/iteratedLocalSearchOrginal/timetabling.ils.de.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/iteratedLocalSearchOrginal/results/resultMedium1b
echo "Original ILS medium 2"
./orginalCode/iteratedLocalSearchOrginal/timetabling.ils.de.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/iteratedLocalSearchOrginal/results/resultMedium2b
echo "Original ILS medium 3"
./orginalCode/iteratedLocalSearchOrginal/timetabling.ils.de.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/iteratedLocalSearchOrginal/results/resultMedium3b

echo "--------------------------------------------------"
echo "--------------------EDITED ILS--------------------"
echo "--------------------------------------------------"

echo "Edited ILS easy 1"
./iteratedLocalSearch/timetabling.ils.de.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o iteratedLocalSearch/results/resultEasy1b
echo "Edited ILS easy 2"
./iteratedLocalSearch/timetabling.ils.de.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o iteratedLocalSearch/results/resultEasy2b
echo "Edited ILS easy 3"
./iteratedLocalSearch/timetabling.ils.de.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o iteratedLocalSearch/results/resultEasy3b

echo "Edited ILS medium 1"
./iteratedLocalSearch/timetabling.ils.de.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o iteratedLocalSearch/results/resultMedium1b
echo "Edited ILS medium 2"
./iteratedLocalSearch/timetabling.ils.de.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o iteratedLocalSearch/results/resultMedium2b
echo "Edited ILS medium 3"
./iteratedLocalSearch/timetabling.ils.de.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o iteratedLocalSearch/results/resultMedium3b


echo "--------------------------------------------------"
echo "---------------ORIGINAL TABU SEARCH---------------"
echo "--------------------------------------------------"

echo "Original Tabu easy 1"
./orginalCode/tabuSearchOrginal/timetabling.ts.ch.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/tabuSearchOrginal/results/resultEasy1b
echo "Original Tabu easy 2"
./orginalCode/tabuSearchOrginal/timetabling.ts.ch.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/tabuSearchOrginal/results/resultEasy2b
echo "Original Tabu easy 3"
./orginalCode/tabuSearchOrginal/timetabling.ts.ch.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/tabuSearchOrginal/results/resultEasy3b

echo "Original Tabu Medium 1"
./orginalCode/tabuSearchOrginal/timetabling.ts.ch.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/tabuSearchOrginal/results/resultMedium1b
echo "Original Tabu Medium 2"
./orginalCode/tabuSearchOrginal/timetabling.ts.ch.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/tabuSearchOrginal/results/resultMedium2b
echo "Original Tabu Medium 3"
./orginalCode/tabuSearchOrginal/timetabling.ts.ch.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/tabuSearchOrginal/results/resultMedium3b


echo "--------------------------------------------------"
echo "----------------EDITED TABU SEARCH----------------"
echo "--------------------------------------------------"

echo "Edited Tabu easy 1"
./tabuSearch/timetabling.ts.ch.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o tabuSearch/results/resultEasy1b
echo "Edited Tabu easy 2"
./tabuSearch/timetabling.ts.ch.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o tabuSearch/results/resultEasy2b
echo "Edited Tabu easy 3"
./tabuSearch/timetabling.ts.ch.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o tabuSearch/results/resultEasy3b

echo "Edited Tabu Medium 1"
./tabuSearch/timetabling.ts.ch.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o tabuSearch/results/resultMedium1b
echo "Edited Tabu Medium 2"
./tabuSearch/timetabling.ts.ch.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o tabuSearch/results/resultMedium2b
echo "Edited Tabu Medium 3"
./tabuSearch/timetabling.ts.ch.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o tabuSearch/results/resultMedium3b


echo "--------------------------------------------------"
echo "--------------ORIGINAL SIMULATED ANAL-------------"
echo "--------------------------------------------------"

echo "Original Sim Anal easy 1"
./orginalCode/simulatedAnnealingOrginal/timetabling.sa.de.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/simulatedAnnealingOrginal/results/resultEasy1b
echo "Original Sim Anal easy 2"
./orginalCode/simulatedAnnealingOrginal/timetabling.sa.de.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/simulatedAnnealingOrginal/results/resultEasy2b
echo "Original Sim Anal easy 3"
./orginalCode/simulatedAnnealingOrginal/timetabling.sa.de.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o orginalCode/simulatedAnnealingOrginal/results/resultEasy3b

echo "Original Sim Anal Medium 1"
./orginalCode/simulatedAnnealingOrginal/timetabling.sa.de.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/simulatedAnnealingOrginal/results/resultMedium1b
echo "Original Sim Anal Medium 2"
./orginalCode/simulatedAnnealingOrginal/timetabling.sa.de.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/simulatedAnnealingOrginal/results/resultMedium2b
echo "Original Sim Anal Medium 3"
./orginalCode/simulatedAnnealingOrginal/timetabling.sa.de.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o orginalCode/simulatedAnnealingOrginal/results/resultMedium3b


echo "--------------------------------------------------"
echo "---------------EDITED SIMULATED ANAL--------------"
echo "--------------------------------------------------"

echo "Edited Sim Anal easy 1"
./simulatedAnnealing/timetabling.sa.de.1 -i data/easy01.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o simulatedAnnealing/results/resultEasy1b
echo "Edited Sim Anal easy 2"
./simulatedAnnealing/timetabling.sa.de.1 -i data/easy02.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o simulatedAnnealing/results/resultEasy2b
echo "Edited Sim Anal easy 3"
./simulatedAnnealing/timetabling.sa.de.1 -i data/easy03.tim -t $TIMEEASY -n $TRIESEASY -s 1337 -p 1 -o simulatedAnnealing/results/resultEasy3b

echo "Edited Sim Anal Medium 1"
./simulatedAnnealing/timetabling.sa.de.1 -i data/medium01.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o simulatedAnnealing/results/resultMedium1b
echo "Edited Sim Anal Medium 2"
./simulatedAnnealing/timetabling.sa.de.1 -i data/medium02.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o simulatedAnnealing/results/resultMedium2b
echo "Edited Sim Anal Medium 3"
./simulatedAnnealing/timetabling.sa.de.1 -i data/medium03.tim -t $TIMEMEDIUM -n $TRIESMEDIUM -s 1337 -p 2 -o simulatedAnnealing/results/resultMedium3b







#easy(3)   40r 90s  = 3600 * 3 = 10800
#medium(3) 15r 450s = 6750 * 3 = 20250
#=31050s * 10(alg) = 310500s = 86,25h
