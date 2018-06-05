#include "Control.h"
#include "Problem.h"
#include "Solution.h"
#include "Random.h"

#include <fstream>

Random* rnd;
extern std::ostream *outs;

int main( int argc, char** argv) {

 // create a control object from the command line arguments
 Control control(argc, argv);
 outs = control.os;

  // create a problem, control tells you what input file stream to use
  Problem *problem = new Problem(control.getInputStream());

  // create a Random object
  rnd = new Random((unsigned) control.getSeed());

  // run a number of tries, control knows how many tries there should be done
  while( control.triesLeft())
  {
	  // tell control we are starting a new try
      control.beginTry();

	  // initialize a random solution
      Solution *currentSolution = new Solution(problem, rnd);
      currentSolution->RandomInitialSolution();
  	  currentSolution->computeHcv();
	  if(currentSolution->hcv == 0)
	  {
		control.setCurrentCost(currentSolution);
	  }else
	  {
	      control.setCurrentCost(currentSolution);
	   }

	  //apply tabu search with paramters given by control for a time limit and probability of each type of move to be performed
	  currentSolution->tabuSearch(control.getTimeLimit(), control.alfa, control.prob1, control.prob2);
	  currentSolution->computeHcv();
	  if(currentSolution->hcv == 0)
		currentSolution->computeScv();

      control.endTry(currentSolution);
	  delete currentSolution;
  }

  delete problem;
}
