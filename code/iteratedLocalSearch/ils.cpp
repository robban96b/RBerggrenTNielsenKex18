#include "Control.h"
#include "Problem.h"
#include "Solution.h"
#include "Random.h"

#include <fstream>
#include <math.h>
#include "Timer.h"

#define MAX_VALUE 10000000
Random* rnd;
Timer timer;

double TIMEINTERVALPRINT = 420.0;


int main( int argc, char** argv) {

  int event1,val;
  int i;
  float annealing;
  int n_done_perturbations = 1;


  // create a control object from the command line arguments
  Control control(argc, argv);

  // create a problem, control tells you what input file stream to use
  Problem *problem = new Problem(control.getInputStream());



  /*-------------PARAMETERS OF ITERATED LOCAL SEARCH ---------------*/

  int type_perturbation;
  int iterations_perturbation;
  int move_type;
  int acceptance_criteria;
  float T;

  if (control.problemType == 1){             //-----------EASY INSTANCE ------------
    type_perturbation = 1;
    move_type =  1;
    iterations_perturbation = 1;
    acceptance_criteria = 3;
    T = 0.025;
  }
  else if (control.problemType == 2) {       //-----------MEDIUM INSTANCE -----------
    type_perturbation = 1;
    move_type = 1;
    iterations_perturbation = 5;
    acceptance_criteria = 2;
    T = 0.1;
  }
  else {                                     //-----------HARD INSTANCE -----------
    type_perturbation = 1;
    move_type = 1;
    iterations_perturbation = 5;
    acceptance_criteria = 2;
    T = 0.1;
  }

  control.maxSteps = MAX_VALUE;                 // Huge value for porpouse. Do not change !!!


  /* ------------ END OF DEFINITION OF PARAMETERS ------------------*/

  // create a Random object
  rnd = new Random((unsigned) control.getSeed());

  // run a number of tries, control knows how many tries there should be done
  while( control.triesLeft()){
    cout << "begin try" << control.getNrTry() + 1 << "(" << control.getNumberOfTries() << ")";
    cout << ", " << control.getNumberOfTries() - control.getNrTry() << " left" << endl;
    timer.resetTime();

    // tell control we are starting a new try
    control.beginTry();

    // initialize a random solution
    Solution *currentSolution = new Solution(problem, rnd);


    currentSolution->RandomInitialSolution();

    //apply local search with paramters given by control for a time limit and probability of each type of move to be performed

    currentSolution->localSearch(control.getMaxSteps(),control.timeLimit - timer.elapsedTime(Timer::VIRTUAL));

    currentSolution->computeFeasibility();

    currentSolution->computeScv();
    currentSolution->computeHcv();

    Solution *bestSolution = new Solution(problem, rnd);

    bestSolution ->copy(currentSolution);

    control.setCurrentCost(bestSolution);

    //----------------------------------------------------------------------
    //----------------------------------------------------------------------
    //           MAIN LOOP
    //----------------------------------------------------------------------
    //----------------------------------------------------------------------

    double printTime = 0.0;     // Used to determine if execution has reached print-interval


    while(control.timeLeft() && bestSolution->scv > 0){

      double currentTime = control.getTime();
      if (currentTime - printTime > TIMEINTERVALPRINT) {
        printTime = currentTime;
	cout << "time left: " << control.timeLimit - currentTime << endl;
      }

      Solution *perturbedSolution = new Solution( problem, rnd );

      // ---- PERTURBATION -----------------//

      perturbedSolution->copy(currentSolution);

      if (type_perturbation == 1){
	if (move_type == 1){
	  for (i = 0; i < iterations_perturbation ; i++) {
	    event1 = (int) (rnd->next() * problem->n_of_events);
	    val = (int) (rnd->next() * 45);
	    perturbedSolution->Move1(event1,val);
	    n_done_perturbations++;
	  }
	}
      }

      // ----- LOCAL SEARCH ---------------//


      perturbedSolution->localSearch(control.getMaxSteps(),control.timeLimit - timer.elapsedTime(Timer::VIRTUAL));


      //-----ACCEPTANCE CRITERIA-----------//

      perturbedSolution->computeScv();
      perturbedSolution->computeHcv();

      // ------- Accept annealing with fixed temperature -------//
      // acceptance_criteria == 2
      // ------- Accept annealing with different temperature -------//
      // acceptance_criteria == 3
      if (acceptance_criteria == 2 || acceptance_criteria == 3){
        int TmultiSCV = 1;
        int TmultiHCV = 1;
        if (acceptance_criteria == 3) {
          TmultiSCV = bestSolution->scv;
          TmultiHCV = bestSolution->hcv;
        }

	if ((perturbedSolution->feasible) && (currentSolution->feasible)){
	  if (perturbedSolution->scv > currentSolution->scv) {
	    annealing = exp(-(perturbedSolution->scv - currentSolution->scv)/(T*TmultiSCV));
	    if (annealing >rnd->next())
	      currentSolution->copy(perturbedSolution);
	  }
	  else if (perturbedSolution->scv < currentSolution->scv) {
	    currentSolution->copy(perturbedSolution);
	    if (perturbedSolution->scv < bestSolution->scv)
	      bestSolution->copy(perturbedSolution);
	  }
	}

	else if ((perturbedSolution->feasible) && (!currentSolution->feasible)){
	  currentSolution->copy(perturbedSolution);
	  if (bestSolution->feasible){
	    if (perturbedSolution->scv < bestSolution->scv)
	      bestSolution->copy(perturbedSolution);
	  }
	  else if (!bestSolution->feasible)
	    bestSolution->copy(perturbedSolution);
	}

	else if ((!perturbedSolution->feasible) && (!currentSolution->feasible)){
	  if (perturbedSolution->hcv > currentSolution->hcv) {
	    annealing = exp(-(perturbedSolution->hcv - currentSolution->hcv)/(T*TmultiHCV));
	    if (annealing > rnd->next())
	      currentSolution->copy(perturbedSolution);
	  }
	  else if (perturbedSolution->hcv < currentSolution->hcv){
	    currentSolution->copy(perturbedSolution);
	    if (perturbedSolution->hcv < bestSolution->hcv)
	      bestSolution->copy(perturbedSolution);
	  }
	}
      }

      //-------- END ACCEPTANCE----------------//


      control.setCurrentCost(bestSolution);

      //cout<< " Constraints: " << perturbedSolution->hcv << " " << perturbedSolution->scv <<  " "<< currentSolution->hcv << " " << currentSolution->scv <<  " " << bestSolution->hcv << " " <<bestSolution->scv << endl;
      delete perturbedSolution;
    }
    control.endTry(bestSolution);
    delete currentSolution;
    delete bestSolution;
    //break;
  }

  delete problem;
}
