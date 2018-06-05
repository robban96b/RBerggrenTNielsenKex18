/* -*- mode: c++; mode: font-lock -*- */
#include "Control.h"
#include "Problem.h"
#include "Solution.h"
#include "Random.h"

#include <fstream>

Random* rnd;

//#define DEBUGMAIN 1

#ifdef DEBUGASSIGNAMENT
double matching_time;
#endif

time_t t;

int main( int argc, char** argv) {


  // create a control object from the command line arguments
  Control control(argc, argv);

  Timer ini_timer;
  double initialization;
   ini_timer.resetTime();
  // create a problem, control tells you what input file stream to use
  Problem *problem = new Problem(control.getInputStream());
  initialization = ini_timer.elapsedTime(Timer::VIRTUAL);

  // create a Random object
  rnd = new Random((unsigned) control.getSeed());

    //Setting of Simulated Annealing Parameters
    //--------------------------------------------------

  int method=0;
  double alfa=0;//cooling rate
  double rateNeighbourhood=0;
  int nrep=0;
  double iniTempFactorHcv=0;
  double iniTempFactorScv=0;
  double rejectedLimHcv=0;
  double rejectedLimScv=0;

 int neighbourhoodSize =  problem->n_of_events
    + ((int)(pow(problem->n_of_events,2)-problem->n_of_events))/2;
  //+ ((int)(pow(problem->n_of_events,3))-3*((int)(pow(problem->n_of_events,2)))
  //	 +2*(problem->n_of_events));

/*
  vector<int>::iterator p = max_element(problem->studentNumber.begin(),
					problem->studentNumber.end());
  int maxsc1 = 5 * (*p);
  int maxsc2 = 40 * problem->n_of_students;
  int maxsc3 = 5 * problem->n_of_students;
  int weight = maxsc1+maxsc2+maxsc3;
*/

  control.setSAParameters(neighbourhoodSize, method, rateNeighbourhood, alfa, nrep, iniTempFactorHcv, iniTempFactorScv, rejectedLimHcv, rejectedLimScv);

  control.informationsReport(method, rateNeighbourhood, alfa, iniTempFactorHcv, iniTempFactorScv, rejectedLimHcv, rejectedLimScv, initialization);


#ifdef DEBUGMAIN
  std::cerr << "Hard constraints weight: " << weight <<"\n";
  std::cerr << "Configuration: "<<control.getConfiguration()<<std::endl;
#endif



  switch (method) {
  case 1:
    //---------- SIMULATED ANNEALING IN LOCAL SEARCH -------------------------------------
    // run a number of tries, control knows how many tries there should be done
    while( control.triesLeft()){
      std::cout << "1begin try" << control.getNrTry() + 1 << "(" << control.getNumberOfTries() << ")";
      std::cout << ", " << control.getNumberOfTries() - control.getNrTry() << " left" << std::endl;
	// tell control we are starting a new try
	control.beginTry();
	// initialize a random solution
	Solution *currentSolution = new Solution(problem,rnd);
	currentSolution->RandomInitialSolution();
	  currentSolution->simulatedAnnealing_LS(control.getOutputStream(),control.getTimeLimit(), alfa, nrep,
						 iniTempFactorHcv, iniTempFactorScv,
						 rejectedLimHcv, rejectedLimScv,
						 control.getProb1(), control.getProb2(), control.getProb3());
	  currentSolution->computeFeasibility();
	  currentSolution->computeScv();
	  //currentSolution->computeHcv();
	  control.endTry(currentSolution);
	delete currentSolution;
      }

	  break;
    std::cout << "2begin try" << control.getNrTry() + 1 << "(" << control.getNumberOfTries() << ")";
    std::cout << ", " << control.getNumberOfTries() - control.getNrTry() << " left" << std::endl;
      case 2:
    //---------- SIMULATED ANNEALING WITH RANDOM MOVES ------------------------------
      // run a number of tries, control knows how many tries there should be done

      while( control.triesLeft()){
	// tell control we are starting a new try
	control.beginTry();
	// initialize a random solution
	Solution *currentSolution = new Solution(problem,rnd);
	currentSolution->RandomInitialSolution();
    currentSolution->simulatedAnnealing_LS_Tfixed(control.getOutputStream(),control.getTimeLimit(),
						  iniTempFactorHcv, iniTempFactorScv,
						  control.getProb1(), control.getProb2(), control.getProb3());
	 currentSolution->computeFeasibility();
	  currentSolution->computeScv();
	  //currentSolution->computeHcv();
control.endTry(currentSolution);
	delete currentSolution;
      }

    break;
      case 3:
		  //---------- SIMULATED ANNEALING WITH RANDOM MOVES ------------------------------
      // run a number of tries, control knows how many tries there should be done
      while( control.triesLeft()){
	std::cout << "begin try" << control.getNrTry() + 1 << "(" << control.getNumberOfTries() << ")";
      std::cout << ", " << control.getNumberOfTries() - control.getNrTry() << " left" << std::endl;
	// tell control we are starting a new try
	control.beginTry();
	// initialize a random solution
	Solution *currentSolution = new Solution(problem,rnd);
	currentSolution->RandomInitialSolution();
	  currentSolution->simulatedAnnealing_Random(control.getOutputStream(),control.getTimeLimit(), alfa, nrep, iniTempFactorHcv, iniTempFactorScv, rejectedLimHcv, rejectedLimScv, control.getProb1(), control.getProb2(), control.getProb3());
	 currentSolution->computeFeasibility();
	  currentSolution->computeScv();
	  //currentSolution->computeHcv();
	  control.endTry(currentSolution);
	delete currentSolution;
      }

	  break;
      default:
	  std::cerr<<"Errors in method settings\n";
  }
  /*
#ifdef DEBUGMAIN
  config=control.getConfiguration();
  if (config>0 && config<=36)
    {
      int val = config;
      r = div(val,2);
      rateNeighInd = r.rem + 1;
      r = div(val,3);
      alfaInd = r.rem + 1;
      r = div(r.quot,2);
      TempHcvInd = r.rem + 1;
      r = div(r.quot,3);
      TempScvInd = r.rem + 1;
      std::cout << val <<" "<< alfaInd<<" " << TempScvInd<<" " <<TempHcvInd<<" " <<rateNeighInd<<" "<<std::endl;
    }

  else if (config>36 && config<=46)
     {
       int val = config -36;
       r = div(val,5);
      TempScvInd = r.rem + 1;
      r = div(r.quot,2);
      TempHcvInd = r.rem + 1;
      std::cout << val << " " << TempScvInd<<" " <<TempHcvInd<<" "<<std::endl;
     }
  else if (config>46 && config<=70)
    {
      int val = config - 46;
      r = div(val,2);
      rateNeighInd = r.rem + 1;
      r = div(val,3);
      alfaInd = r.rem + 1;
      r = div(r.quot,2);
      TempHcvInd = r.rem + 1;
      r = div(r.quot,2);
      TempScvInd = r.rem + 1;

      std::cout << val << " " <<alfaInd<<" " << TempScvInd<<" " <<TempHcvInd<<" "<<rateNeighInd<<std::endl;
    }
  std::cout<<"--------------------------------------------------\n";
  std::cout << "Simulated Annealing parameters:\n";
  std::cout << "Cooling rate: " << alfa << "\n";
  std::cout << "Repetition at each temperature: " << nrep
       << " ( = "<<rateNeighbourhood<<" x "<<neighbourhoodSize<<")\n";
  std::cout << "Initial Temperature Hcv Factor: " << iniTempFactorHcv << "\n";
  std::cout << "Initial Temperature Scv Factor: " << iniTempFactorScv << "\n";
#endif
  */
  delete problem;
}
