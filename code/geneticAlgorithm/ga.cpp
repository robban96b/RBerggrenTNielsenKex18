#include "Control.h"
#include "Problem.h"
#include "Solution.h"

#include <fstream>
#include <algorithm>

#include <list>
#include <vector>

Random* rnd;

double TIMEINTERVALPRINT = 60.0;


/*class compareSolution{
public:
  bool operator()(Solution* sol1, Solution* sol2) const
  { return sol1->penalty < sol2->penalty;}
  }*/

Solution* selection(Solution** pop, int popSize ){

  // tournament selection with tornament size 2
  int first, second;
  first = (int)(rnd->next()*popSize);
  second = (int)(rnd->next()*popSize);

  if(pop[first]->penalty < pop[second]->penalty)
      return(pop[first]);
  else
      return(pop[second]);

}

Solution* selection5(Solution** pop, int popSize ){

  // tournament selection with tornament size 5
  int tournament[5];
  int best;

  tournament[0] = (int)(rnd->next()*popSize);

  best =  tournament[0];
  for(int i= 1; i<5; i++){
    tournament[i] = (int)(rnd->next()*popSize);
    if(pop[tournament[i]]->penalty < pop[best]->penalty)
      best = tournament[i];
  }

  return(pop[best]);

}

bool compareSolution(Solution * sol1, Solution * sol2)
{
 return sol1->penalty < sol2->penalty;
}

int main( int argc, char** argv) {

  Control control(argc, argv);

  int problemType = control.getProblemType();
  int popSize = 10;
  int maxSteps;
  if (problemType == 1){
    maxSteps = 200;
  }
  else if (problemType == 2) {
    maxSteps = 1000;
  }
  else{
    maxSteps = 2000;
  }

  Problem *problem = new Problem(control.getInputStream());

  rnd = new Random((unsigned) control.getSeed());

  while( control.triesLeft()){

    std::cout << "begin try" << control.getNrTry() + 1 << "(" << control.getNumberOfTries() << ")";
    std::cout << ", " << control.getNumberOfTries() - control.getNrTry() << " left" << std::endl;


    control.beginTry();

    int generation = 0;

    Solution* pop[popSize];

    for(int i=0; i < popSize; i++){
      pop[i] = new Solution(problem, rnd);
      pop[i]->RandomInitialSolution();
      pop[i]->localSearch(maxSteps);
      pop[i]->computePenalty();
      //std::cout<< pop[i]->penalty<<std::endl;
    }

    // sort the population by penalty
    std::sort(pop, pop + popSize, compareSolution);

    /*for(int i=0; i < popSize; i++){
      std::cout<< pop[i]->penalty<<std::endl;
      }*/

    control.setCurrentCost(pop[0]);

    double printTime = 0.0;

    while(control.timeLeft()){

      double currentTime = control.getTime();
      if (currentTime - printTime > TIMEINTERVALPRINT) {
        printTime = currentTime;
        std::cout << "time left: " << control.getTimeLimit() - currentTime << std::endl;
      }

      /*
      int loop;
      int elite = 5;
      double cross_rate = 0.5;

      Solution* newpop[popSize];
      for(int i=0; i < popSize; i++)
	newpop[i] = new Solution(problem, rnd);

      for(loop=0; loop < elite; loop++){
	newpop[loop]->copy(pop[loop]);
	newpop[loop]->localSearch(control.getMaxSteps());
	newpop[loop]->computePenalty();
      }
      for (; loop < popSize;) {
	Solution*parent1 = selection(pop, popSize);
	Solution*parent2 = selection(pop, popSize);
	if (rnd->next() <= cross_rate) {

	  newpop[loop]->crossover(parent1, parent2);

	  // do some mutation
	  newpop[loop]->mutation();

	  newpop[loop]->localSearch(control.getMaxSteps());
	  newpop[loop]->computePenalty();

	} else {
	  newpop[loop]->copy(parent1);
	  newpop[loop]->localSearch(control.getMaxSteps());
	  newpop[loop]->computePenalty();
	}
	loop++;
      }
      for(int i = 0; i < popSize; i++){
	pop[i]->copy(newpop[i]);
	delete newpop[i];
      }

      sort(pop ,pop + popSize-1, compareSolution);
      control.setCurrentCost(pop[0]);
      */

      // start reproduction (steady-state GA)
      Solution* child= new Solution(problem,rnd);

      // select parents
      Solution* parent1 = selection5(pop, popSize);
      Solution* parent2 = selection5(pop, popSize);

      // generate child
      if(rnd->next() < 0.8)
	child->crossover(parent1,parent2);
      else{
	child->copy(parent1);
      }

      // do some mutation
      if(rnd->next() < 0.5){
	  child->mutation();
      }

      //apply local search to offspring
      child->localSearch(maxSteps);

      //evaluate the offspring
      child->computePenalty();
      //std::cout << "Child " << child->penalty << std::endl;
      //std::cout<< "Parent1 "<< parent1->penalty<< " Parent2 " << parent2->penalty<<std::endl;
      generation ++;
      // replace worst member of the population with offspring
      //if(child->penalty < pop[popSize - 1]->penalty){
	pop[popSize - 1]->copy(child);
	std::sort(pop, pop + popSize, compareSolution);
	//std::cout<< "generation " << generation << std::endl;
	control.setCurrentCost(pop[0]);
	//}
      // replace if better
	// if(parent1->penalty < parent2->penalty){
	//if(child->penalty < parent2->penalty ){
	 // parent2->copy(child);
	//}
      //}
      //else{
	//if(child->penalty < parent1->penalty ){
	  //parent1->copy(child);
	//}
	//}
      //std::cout<<"New elements in the pop " << parent1->penalty<< " " << parent2->penalty<< std::endl;

      // sort the new pop

      delete child;
    }
    control.endTry(pop[0]);

    // remember to delete the population
    for(int i=0; i < popSize; i++){
      delete pop[i];
    }
  }

  delete problem;
  delete rnd;

}
