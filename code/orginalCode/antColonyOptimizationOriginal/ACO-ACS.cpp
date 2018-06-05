/***************************************************************************
                         ACO-ACS.cpp   -   description
                            -----------------------
begin                   : Sunday March 10 18:46:50 CET 2002
copyright               : (C) 2002 by Max Manfrin
email                   : mmanfrin@ulb.ac.be
***************************************************************************/

/***************************************************************************
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
*                                                                          *
***************************************************************************/

#include "Control.h"
#include "Problem.h"
#include "Solution.h"
#include "Random.h"
#include "Ant.h"

#include <fstream>
#include <algorithm>
#include <math.h>
#include <limits.h>

// ACS PARAMETERS
double TAU0;
double ALPHA;
double BETA;
double GAMMA;
double RHO;
double Q0;
int COLONYSIZE;
double FACTOR;
int FPITER;
int FPSTEPS;
int SPSTEPS;

Random* rnd;
time_t t;
std::vector<int> connection_degree;
std::vector<int> n_of_features;
Problem* problem;
std::vector<int> ordered_events;
std::vector<Ant*> colony;
Solution* bestSolution;
int bestfitness;
int bestquality;
double** pheromoneMatrix;
double** totalMatrix;
double delta_tau;
int mySteps;
//ofstream pheromatrix("file.max");

//Check if the copy between solution is done right
void check_copySolution(Solution* destination, Solution* origin){
  std::cout << "Origin      scv:" << &origin->scv << " value:"<< origin->scv << std::endl;
  std::cout << "Destination scv:" << &destination->scv << " value:" << destination->scv << std::endl;
  std::cout << "Origin      hcv:" << &origin->hcv << " value:" << origin->hcv << std::endl;
  std::cout << "Destination hcv:" << &destination->hcv << " value:" << destination->hcv << std::endl;
  std::cout << "Origin      feasible:" << &origin->feasible << " value:" << origin->feasible << std::endl;
  std::cout << "Destination feasible:" << &destination->feasible << " value:" << destination->feasible <<std::endl;
  std::cout << "Origin      data:" << &origin->data << " value:" << origin->data << std::endl;
  std::cout << "Destination data:" << &destination->data << " value:" << destination->data << std::endl;
  std::cout << "Origin      sln:" << &origin->sln << " value:";
  for (int i=0; i < (int)origin->sln.size(); i++){
    std::cout << origin->sln[i].first << "," << origin->sln[i].second << " ";
  }
  std::cout << std::endl;
  std::cout << "Destination sln:" << &destination->sln << " value:";
  for (int i=0; i < (int)destination->sln.size(); i++){
    std::cout << destination->sln[i].first << "," << destination->sln[i].second << " ";
  }
  std::cout << std::endl;
  std::cout << "Origin      timeslot_events:" << &origin->timeslot_events << " value:" << std::endl;
  for (int i=0; i <45; i++){
    std::cout << "   " << "Timeslot:" << i << " Events:";
    for (int j=0; j < (int)origin->timeslot_events[i].size(); j++){
      std::cout << origin->timeslot_events[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout<< std::endl;
  std::cout << "Destination timeslot_events:" << &destination->timeslot_events << " value:" << std::endl;
  for (int i=0; i < 45; i++){
    std::cout << "   " << "Timeslot:" << i << " Events:";
    for (int j=0; j < (int)destination->timeslot_events[i].size(); j++){
      std::cout << destination->timeslot_events[i][j] << " ";
    }
    std::cout << std::endl;
    }
  std::cout << std::endl;


}

// Print the pheromoneMatrix[ts][events]
//void print_pheromoneMatrix(){
  //  std::cout << "PHEROMONE MATRIX" << std::endl;
//  for (int j=0; j < problem->n_of_events; j++){
    //    std::cout << "Event " << j <<":";
//    for (int i=0; i < 45; i++){
//      pheromatrix << pheromoneMatrix[i][j] << " ";
//    }
//    pheromatrix << std::endl;
//  }
//  pheromatrix << std::endl;
//}

// BINARY PREDICATE USED AS THE SORTING CRITERIA
// FOR BUILDING THE EVENT LIST
// ORDER EVENTS BY CONNECTION DEGREE
// THEN BY NUMBER OF FEATURE
// THEN BY NUMBER OF STUDENTS
// THEN BY LABEL (this is why stable_sort)
//
bool eventsort (const int& a, const int& b) {

  if (connection_degree[a] > connection_degree[b]){
    return true;
  }
  else {
    if ((connection_degree[a] == connection_degree[b])&&(n_of_features[a] < n_of_features[b])){
      return true;
    }
    else{
      if ((connection_degree[a] == connection_degree[b])&&(n_of_features[a] == n_of_features[b])&&(problem->studentNumber[a] > problem->studentNumber[b])){
	return true;
      }
      else{
	return false;
      }
    }
  }
}


int main( int argc, char** argv) {

  // CREATE THE CONTROL OBJECT
  // PARSING THE COMMAND LINE
  //
  Control control(argc, argv);

  // CREATE THE PROBLEM OBJECT
  // LOADING DATA FROM THE INSTANCE FILE PASSED AT THE COMMAND LINE
  //
  problem = new Problem(control.getInputStream());



  // SET THE PARAMETERS DEPENDING ON THE PROBLEM INSTANCE
  // easy   : <= 200 eventsdouble TAU0;
  // medium : > 200 events && <= 300 students
  // hard   : > 200 events && > 300 students

  if (problem->n_of_events <= 200) {

    // SETTINGS FOR EASY INSTANCE
    //
    std::cerr << "Warning: Settings for the easy instance loaded" << std::endl;
    TAU0 = 0.5;
    ALPHA = 0.1;
    RHO = 0.1;
    BETA = 3.0;
    GAMMA = 2.0;
    Q0 = 0.0;
    COLONYSIZE = 15;
    FACTOR = 100000.0;
    FPITER = 1;
    FPSTEPS = 5000;
    SPSTEPS = 2000;
  }
  else {
    if (problem->n_of_students <= 300){

      // SETTINGS FOR MEDIUM INSTANCE
      //
      std::cerr << "Warning: Settings for the medium instance loaded" << std::endl;
      TAU0 = 10.0;
      ALPHA = 0.1;
      RHO = 0.1;
      BETA = 3.0;
      GAMMA = 2.0;
      Q0 = 0.0;
      COLONYSIZE = 15;
      FACTOR = 10000000000.0;
      FPITER = 10;
      FPSTEPS = 50000;
      SPSTEPS = 10000;
    }
    else {

      // SETTINGS FOR HARD INSTANCE
      //
      std::cerr << "Warning: Settings for the hard instance loaded" << std::endl;
      TAU0 = 10.0;
      ALPHA = 0.1;
      RHO = 0.1;
      BETA = 3.0;
      GAMMA = 2.0;
      Q0 = 0.0;
      COLONYSIZE = 10;
      FACTOR = 10000000000.0;
      FPITER = 20;
      FPSTEPS = 150000;
      SPSTEPS = 100000;
    }
  }



  // CREATE THE RANDOM NUMBERS GENERATOR OBJECT
  // seed 1 for debugging purpose only
  rnd = new Random((unsigned) control.getSeed());

  // PRECALCULATE THE LIST OF EVENTS
  //
  for (int i=0; i < problem->n_of_events; i++){
    connection_degree.push_back(0);
    n_of_features.push_back(0);
  }
  // CALCULATE CONNECTION DEGREE AMONGST EVENTS
  //
  for (int i=0; i < problem->n_of_events; i++){
    int sum = 0;
    for (int j=0; j < problem->n_of_events; j++){
      if (problem->eventCorrelations[i][j]){
	sum++;
      }
    }
    connection_degree[i] = sum;
  }
  // CALCULATE NUMBER OF FEATURES REQUIRED BY EACH EVENT
  //
  for (int i=0; i < problem->n_of_events; i++){
    int sum = 0;
    for (int j=0; j < problem->n_of_features; j++){
      if (problem->event_features[i][j]){
	sum++;
      }
    }
    n_of_features[i] = sum;
  }
  // CALL THE SORTING FUNCTION ON ALL THE ELEMENTS
  //
  std::vector<int>::iterator pos;
  for (int i=0; i < problem->n_of_events; i++){
    ordered_events.push_back(i);
  }
  for (pos = ordered_events.begin(); pos != ordered_events.end(); ++pos){
    stable_sort(ordered_events.begin(), ordered_events.end(), eventsort);
  }

  //  std::cout << "Ordered Events:";
  //  for (int i=0; i < problem->n_of_events; i++){
  //    std::cout << ordered_events[i] << " ";
  //  }
  //  std::cout << std::endl;

  // CREATE THE PHEROMONE MATRIX
  // double** pheromoneMatrix[timeslots][events]
  //
  pheromoneMatrix = DoubleMatrixAlloc(45, problem->n_of_events);
  totalMatrix = DoubleMatrixAlloc(45, problem->n_of_events);

  // CREATE THE ANT COLONY
  //
  for(int i=0; i < COLONYSIZE; i++){
    colony.push_back(new Ant());
  }

  // CREATE SOLUTION FOR LOCAL SEARCH
  //
  Solution* perturbedSolution = new Solution(problem, rnd);


  // RUN A NUMBER OF TRIES, CONTROL TAKE CARE OF THAT
  //
  while( control.triesLeft()){

    control.beginTry();


    // ******************************
    // ** 1 ** INITIALIZATION PHASE
    // ******************************

    // - bestSolution
    //
    bestSolution = new Solution(problem, rnd);
    bestSolution->feasible = false;
    bestSolution->scv = INT_MAX;
    bestSolution->hcv = INT_MAX;

    // - pheromoneMatrix[45][n_of_events] = TAU0
    // - colony[COLONYSIZE]
    //
    for (int i=0; i < 45; i++){
      for (int j=0; j < problem->n_of_events; j++){
	pheromoneMatrix[i][j] = TAU0;
      }
    }
    //    std::cout << "PHEROMONE MATRIX INITIALIZED" << std::endl;
    //    print_pheromoneMatrix();

    int iteration_counter = 0;

    while (control.timeLeft()){

      iteration_counter++;

      for(int i=0; i < COLONYSIZE; i++){
	(colony[i]->current_event) = ordered_events[0];
	delete (colony[i]->currentSolution);
	(colony[i]->currentSolution) = new Solution(problem, rnd);
	for (int j=0; j < 45; j++){
	  for (int k=0; k < problem->n_of_students; k++){
	    colony[i]->student_eventsMatrix[j][k] = 0;
	  }
	}
	for (int j=0; j < 5; j++){
	  for (int k=0; k < problem->n_of_students; k++){
	    colony[i]->student_dayMatrix[j][k] = 0;
	  }
	}
      }

      // **********************************
      // ** 2 ** BUILDING SOLUTIONS PHASE
      // **********************************

      // Ants build the solution in parallel
      //

      for(int i=0; i < COLONYSIZE; i++){
	for(int step=0; step < problem->n_of_events; step++){

	  colony[i]->solutionStep(step);

	  // Ants applies the local update rule
	  // tau[i][j] = ((1 - rho) * tau[i][j]) + (rho * delta_tau[i][j]))
	  // Simple ACS set delta_tau[i][j] = tau0
	  // Ant-Q set delta_tau[i][j] = (gamma * max(tau[k][l]))
	  // where tau[k][l] are the events still to insert in the solution
	  //
#ifdef DEBUG
//	  std::cout << std::endl;
//	  std::cout << "Before Local Update Pheromone Matrix [" << (colony[i]->currentSolution)->sln[ordered_events[step]].first << "][" << ordered_events[step] << "]:" << pheromoneMatrix[(colony[i]->currentSolution)->sln[ordered_events[step]].first][ordered_events[step]] << std::endl;
#endif
	  pheromoneMatrix[(colony[i]->currentSolution)->sln[ordered_events[step]].first][ordered_events[step]] = (((1.0 - RHO) * pheromoneMatrix[(colony[i]->currentSolution)->sln[ordered_events[step]].first][ordered_events[step]]) + (RHO * TAU0));
#ifdef DEBUG
//	  std::cout << "After  Local Update Pheromone Matrix [" << ((colony[i]->currentSolution)->sln[ordered_events[step]]).first << "][" << ordered_events[step] << "]:" << pheromoneMatrix[(colony[i]->currentSolution)->sln[ordered_events[step]].first][ordered_events[step]] << std::endl;
//	  std::cout << std::endl;
#endif
	}
      }

#ifdef DEBUG
      //std::cout << "PHEROMONE MATRIX AFTER LOCAL UPDATE RULE" << std::endl;
      //      print_pheromoneMatrix();
#endif

      // Then a matching algorithm based on a deterministic network flow algorithm assigns rooms to events in each non-empty timeslot
      //
      for(int i=0; i< COLONYSIZE; i++){
	for (int j=0; j < 45; j++){
	  if((int)(colony[i]->currentSolution)->timeslot_events[j].size()){
	    (colony[i]->currentSolution)->assignRooms(j);
	  }
	}
      }

#ifdef DEBUG
      for (int i=0; i < COLONYSIZE; i++){
	(colony[i]->currentSolution)->feasible = (colony[i]->currentSolution)->computeFeasibility();
	(colony[i]->currentSolution)->hcv = (colony[i]->currentSolution)->computeHcv();
	(colony[i]->currentSolution)->scv = (colony[i]->currentSolution)->computeScv();
	colony[i]->fitness = ((((colony[i]->currentSolution)->hcv) * 1000) + ((colony[i]->currentSolution)->scv));
	std::cout << "Before local search Ant:" << i << " has build a solution with hcv: " << (colony[i]->currentSolution)->hcv << " and scv:" << (colony[i]->currentSolution)->scv << " and a fitness of " << colony[i]->fitness << std::endl;
      }
#endif

      // APPLY THE LOCAL SEARCH ROUTINE
      // Two phase strategy
      //
      if (iteration_counter < FPITER){
	mySteps = FPSTEPS;
      }
      else {
	mySteps = SPSTEPS;
      }
      for(int i=0; i < COLONYSIZE; i++){
	perturbedSolution->copy(colony[i]->currentSolution);
	perturbedSolution->localSearch(mySteps);
	colony[i]->currentSolution->copy(perturbedSolution);
      }


      // *******************************
      // ** 3 ** GLOBAL UPDATING PHASE
      // *******************************

      // Compute the quality of each solution
      //
      for (int i=0; i < COLONYSIZE; i++){
	(colony[i]->currentSolution)->feasible = (colony[i]->currentSolution)->computeFeasibility();
	(colony[i]->currentSolution)->hcv = (colony[i]->currentSolution)->computeHcv();
	(colony[i]->currentSolution)->scv = (colony[i]->currentSolution)->computeScv();
	colony[i]->fitness = ((((colony[i]->currentSolution)->hcv) * 1000) + ((colony[i]->currentSolution)->scv));
#ifdef DEBUG
	std::cout << "Ant:" << i << " has build a solution that has feasibility: " << (colony[i]->currentSolution)->feasible << " and a fitness of " << colony[i]->fitness << std::endl;
#endif
	// Compare each solution with bestSolution
	// and if better update bestSolution
	//

	// BOTH UNFEASIBLE
	// CHECK HCV
	//
	if (( (colony[i]->currentSolution)->feasible == false) &&  (bestSolution->feasible == false)){
	  if ( (colony[i]->currentSolution)->hcv < bestSolution->hcv ){
	    bestSolution->copy(colony[i]->currentSolution);
	    bestquality = colony[i]->currentSolution->hcv + colony[i]->currentSolution->scv;
	    bestfitness = colony[i]->fitness;
	  }
	}
	// NEW ONE FEASIBLE AND BEST UNFEASIBLE
	//
	else {
	  if(( (colony[i]->currentSolution)->feasible == true) && (bestSolution->feasible == false )){
	    bestSolution->copy(colony[i]->currentSolution);
	    bestquality = colony[i]->currentSolution->hcv + colony[i]->currentSolution->scv;
	    bestfitness = colony[i]->fitness;
	  }
	  // BOTH FEASIBLE
	  // CHECK SCV
	  //
	  else {
	    if(( (colony[i]->currentSolution)->feasible == true) && (bestSolution->feasible == true)){
	      if( (colony[i]->currentSolution)->scv < bestSolution->scv ){
		bestSolution->copy(colony[i]->currentSolution);
		bestquality = colony[i]->currentSolution->hcv + colony[i]->currentSolution->scv;
		bestfitness = colony[i]->fitness;
	      }
	    }
	  }
	}
      }

#ifdef DEBUG
      std::cout << "Iteration: " << iteration_counter << " feasibility: " << bestSolution->feasible << " fitness: " << bestfitness << std::endl;
#endif

      control.setCurrentCost(bestSolution);
      // UPDATE THE PHEROMONE MATRIX
      //
      for (int i=0; i < 45; i++){
	for (int j=0; j < problem->n_of_events; j++){
	  //#ifdef DEBUG
	  //	std::cout << "Before Global update Evaporation Pheromone Matrix [" << i << "][" << j << "]:" << pheromoneMatrix[i][j] << std::endl;
	  //#endif
	  pheromoneMatrix[i][j] = ((1.0 - ALPHA) * pheromoneMatrix[i][j]);
	  //#ifdef DEBUG
	  //	std::cout << "After  Global update Evaporation Pheromone Matrix [" << i << "][" << j << "]:" << pheromoneMatrix[i][j] << std::endl;
	  //#endif
	}
      }
      delta_tau = (FACTOR / ((1.0 + (double)bestquality)));
      for (int i=0; i < problem->n_of_events; i++){
#ifdef DEBUG
//	std::cout << "Before Global update Reinforcement Pheromone Matrix [" << bestSolution->sln[i].first << "][" << i << "]:" << pheromoneMatrix[bestSolution->sln[i].first][i] << std::endl;
#endif
	pheromoneMatrix[bestSolution->sln[i].first][i] =  (pheromoneMatrix[bestSolution->sln[i].first][i] + (ALPHA * delta_tau));
#ifdef DEBUG
//	std::cout << "After  Global update Reinforcement Pheromone Matrix [" << bestSolution->sln[i].first << "][" << i << "]:" << pheromoneMatrix[bestSolution->sln[i].first][i] << std::endl;
#endif
      }

#ifdef DEBUG
      //std::cout << "PHEROMONE MATRIX AFTER GLOBAL UPDATE RULE" << std::endl;
      //      print_pheromoneMatrix();
#endif

    }
    control.endTry(bestSolution);
    delete bestSolution;
  }
  delete problem;
  delete perturbedSolution;
  //  delete &control;
  //  delete rnd;
}
