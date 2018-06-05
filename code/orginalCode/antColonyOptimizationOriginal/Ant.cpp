#include "Ant.h"

Ant::Ant() {

  current_event = ordered_events[0];
  currentSolution = NULL;
  student_eventsMatrix = IntMatrixAlloc(45, problem->n_of_students);
  for (int i=0; i < 45; i++){
    for (int j=0; j < problem->n_of_students; j++){
      student_eventsMatrix[i][j] = 0;
    }
  }
  student_dayMatrix = IntMatrixAlloc(5,problem->n_of_students);
  for (int i=0; i < 5; i++){
    for (int j=0; j < problem->n_of_students; j++){
      student_dayMatrix[i][j] = 0;
    }
  }
  tauij = new double[45];
  etaij = new double[45];
  piij = new double[45];
  for (int i=0; i<45; i++){
    tauij[i] = 0.0;
    etaij[i] = 0.0;
    piij[i]  = 0.0;
  }
  roulette = new double[45];
  fitness = INT_MAX;
}

Ant::~Ant() {

  delete currentSolution;
}

void Ant::solutionStep(int step){

  int day;

  current_event = ordered_events[step];
  int t = chooseTimeslot(current_event);
  currentSolution->timeslot_events[t].push_back(current_event);
  currentSolution->sln[current_event].first = t;

  for (int i=0; i < problem->n_of_students; i++){
    if (problem->student_events[i][current_event] == 1){
      student_eventsMatrix[t][i] = 1;
    }
  }
  day = t / 9;
  for (int i=0; i < problem->n_of_students; i++){
    student_dayMatrix[day][i] = student_dayMatrix[day][i] + 1;
  }
}

int Ant::chooseTimeslot(const int event){

  double dividend, divisor;
  double eVHC   = 0.0;
  double pij    = 0.0;
  double terVSC = 0.0;
  double q = rnd->next();
  double q1 = rnd->next();

  // Calculate values for timeslots in Monday
  //
  for (int j=0; j < 9; j++){

    // Calculate Tauij for fixed event i and all timeslots j
    //
    tauij[j] = pheromoneMatrix[j][event];

    // Calculate Etaij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR HARD CONSTRAINTS
    //
    eVHC = 0.0;
    for (int i = 0; i < (int)currentSolution->timeslot_events[j].size(); i++){
      if (problem->eventCorrelations[event][currentSolution->timeslot_events[j][i]] == 1){
	eVHC = eVHC + 1.0;
      }
    }
    etaij[j] = pow(1.0/(1.0+eVHC),BETA);
    //    cout << "Event:" << event<< " Timeslot:" << j << " etaij:" << etaij[j] << endl
;
    // Calculate Piij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR SOFT CONSTRAINTS
    //
    terVSC = 0.0;
    for (int i=0; i < problem->n_of_students; i++){
      if (j < 7){
	terVSC = terVSC + (double)(problem->student_events[i][event] && student_eventsMatrix[j+1][i] && student_eventsMatrix[j+2][i]);
      }
      if (j > 0 && j < 8){
	terVSC = terVSC + (double)(student_eventsMatrix[j-1][i] && problem->student_events[i][event] && student_eventsMatrix[j+1][i]);
      }
      if ( j > 1){
	terVSC = terVSC + (double)(student_eventsMatrix[j-2][i] && student_eventsMatrix[j-1][i] && problem->student_events[i][event]);
      }
    }
    // Last timeslot of the day
    //
    if (j==8){
      terVSC = terVSC + (double)problem->studentNumber[event];
    }
    //only one event on a day
    //
    for (int i=0; i < problem->n_of_students; i++){
      if (problem->student_events[i][event] == 1){
	if (student_dayMatrix[0][i] == 0){
	  terVSC = terVSC + 1.0;
	}
      }
    }
    //cout << "terVSC:" << terVSC << endl;
    piij[j] = pow(1.0/(1.0+terVSC),GAMMA);
    //    cout << "Timeslot:" << j << " piij:" << piij[j] << endl;
  }

  // Calculate values for timeslots in Tuesday
  //
  for (int j=9; j < 18; j++){

    // Calculate Tauij for fixed event i and all timeslots j
    //
    tauij[j] = pheromoneMatrix[j][event];

    // Calculate Etaij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR HARD CONSTRAINTS
    //
    eVHC = 0.0;
    for (int i = 0; i < (int)currentSolution->timeslot_events[j].size(); i++){
      if (problem->eventCorrelations[event][currentSolution->timeslot_events[j][i]] == 1){
	eVHC = eVHC + 1.0;
      }
    }
    etaij[j] = pow(1.0/(1.0+eVHC),BETA);
    //    cout << "Event:" << event<< " Timeslot:" << j << " etaij:" << etaij[j] << endl
;
    // Calculate Piij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR SOFT CONSTRAINTS
    //
    terVSC = 0.0;
    for (int i=0; i < problem->n_of_students; i++){
      if (j < 16){
	terVSC = terVSC + (double)(problem->student_events[i][event] && student_eventsMatrix[j+1][i] && student_eventsMatrix[j+2][i]);
      }
      if (j > 9 && j < 17){
	terVSC = terVSC + (double)(student_eventsMatrix[j-1][i] && problem->student_events[i][event] && student_eventsMatrix[j+1][i]);
      }
      if ( j > 10){
	terVSC = terVSC + (double)(student_eventsMatrix[j-2][i] && student_eventsMatrix[j-1][i] && problem->student_events[i][event]);
      }
    }
    // Last timeslot of the day
    //
    if (j==17){
      terVSC = terVSC + (double)problem->studentNumber[event];
    }
    //only one event on a day
    //
    for (int i=0; i < problem->n_of_students; i++){
      if (problem->student_events[i][event] == 1){
	if (student_dayMatrix[1][i] == 0){
	  terVSC = terVSC + 1.0;
	}
      }
    }
    //cout << "terVSC:" << terVSC << endl;
    piij[j] = pow(1.0/(1.0+terVSC),GAMMA);
    //    cout << "Timeslot:" << j << " piij:" << piij[j] << endl;
  }

  // Calculate values for timeslots in Wendsday
  //
  for (int j=18; j < 27; j++){

    // Calculate Tauij for fixed event i and all timeslots j
    //
    tauij[j] = pheromoneMatrix[j][event];

    // Calculate Etaij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR HARD CONSTRAINTS
    //
    eVHC = 0.0;
    for (int i = 0; i < (int)currentSolution->timeslot_events[j].size(); i++){
      if (problem->eventCorrelations[event][currentSolution->timeslot_events[j][i]] == 1){
	eVHC = eVHC + 1.0;
      }
    }
    etaij[j] = pow(1.0/(1.0+eVHC),BETA);
    //    cout << "Event:" << event<< " Timeslot:" << j << " etaij:" << etaij[j] << endl
;
    // Calculate Piij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR SOFT CONSTRAINTS
    //
    terVSC = 0.0;
    for (int i=0; i < problem->n_of_students; i++){
      if (j < 25){
	terVSC = terVSC + (double)(problem->student_events[i][event] && student_eventsMatrix[j+1][i] && student_eventsMatrix[j+2][i]);
      }
      if (j > 18 && j < 26){
	terVSC = terVSC + (double)(student_eventsMatrix[j-1][i] && problem->student_events[i][event] && student_eventsMatrix[j+1][i]);
      }
      if ( j > 19){
	terVSC = terVSC + (double)(student_eventsMatrix[j-2][i] && student_eventsMatrix[j-1][i] && problem->student_events[i][event]);
      }
    }
    // Last timeslot of the day
    //
    if (j==26){
      terVSC = terVSC + (double)problem->studentNumber[event];
    }
    //only one event on a day
    //
    for (int i=0; i < problem->n_of_students; i++){
      if (problem->student_events[i][event] == 1){
	if (student_dayMatrix[2][i] == 0){
	  terVSC = terVSC + 1.0;
	}
      }
    }
    //cout << "terVSC:" << terVSC << endl;
    piij[j] = pow(1.0/(1.0+terVSC),GAMMA);
    //    cout << "Timeslot:" << j << " piij:" << piij[j] << endl;
  }

  // Calculate values for timeslots in Thursday
  //
  for (int j=27; j < 36; j++){

    // Calculate Tauij for fixed event i and all timeslots j
    //
    tauij[j] = pheromoneMatrix[j][event];

    // Calculate Etaij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR HARD CONSTRAINTS
    //
    eVHC = 0.0;
    for (int i = 0; i < (int)currentSolution->timeslot_events[j].size(); i++){
      if (problem->eventCorrelations[event][currentSolution->timeslot_events[j][i]] == 1){
	eVHC = eVHC + 1.0;
      }
    }
    etaij[j] = pow(1.0/(1.0+eVHC),BETA);
    //    cout << "Event:" << event<< " Timeslot:" << j << " etaij:" << etaij[j] << endl
;
    // Calculate Piij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR SOFT CONSTRAINTS
    //
    terVSC = 0.0;
    for (int i=0; i < problem->n_of_students; i++){
      if (j < 34){
	terVSC = terVSC + (double)(problem->student_events[i][event] && student_eventsMatrix[j+1][i] && student_eventsMatrix[j+2][i]);
      }
      if (j > 27 && j < 35){
	terVSC = terVSC + (double)(student_eventsMatrix[j-1][i] && problem->student_events[i][event] && student_eventsMatrix[j+1][i]);
      }
      if ( j > 28){
	terVSC = terVSC + (double)(student_eventsMatrix[j-2][i] && student_eventsMatrix[j-1][i] && problem->student_events[i][event]);
      }
    }
    // Last timeslot of the day
    //
    if (j==35){
      terVSC = terVSC + (double)problem->studentNumber[event];
    }
    //only one event on a day
    //
    for (int i=0; i < problem->n_of_students; i++){
      if (problem->student_events[i][event] == 1){
	if (student_dayMatrix[3][i] == 0){
	  terVSC = terVSC + 1.0;
	}
      }
    }
    //cout << "terVSC:" << terVSC << endl;
    piij[j] = pow(1.0/(1.0+terVSC),GAMMA);
    //    cout << "Timeslot:" << j << " piij:" << piij[j] << endl;
  }

  // Calculate values for timeslots in Friday
  //
  for (int j=36; j < 45; j++){

    // Calculate Tauij for fixed event i and all timeslots j
    //
    tauij[j] = pheromoneMatrix[j][event];

    // Calculate Etaij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR HARD CONSTRAINTS
    //
    eVHC = 0.0;
    for (int i = 0; i < (int)currentSolution->timeslot_events[j].size(); i++){
      if (problem->eventCorrelations[event][currentSolution->timeslot_events[j][i]] == 1){
	eVHC = eVHC + 1.0;
      }
    }
    etaij[j] = pow(1.0/(1.0+eVHC),BETA);
    //    cout << "Event:" << event<< " Timeslot:" << j << " etaij:" << etaij[j] << endl
;
    // Calculate Piij for fixed event i and all timeslots j
    // HEURISTIC INFORMATION FOR SOFT CONSTRAINTS
    //
    terVSC = 0.0;
    for (int i=0; i < problem->n_of_students; i++){
      if (j < 43){
	terVSC = terVSC + (double)(problem->student_events[i][event] && student_eventsMatrix[j+1][i] && student_eventsMatrix[j+2][i]);
      }
      if (j > 36 && j < 44){
	terVSC = terVSC + (double)(student_eventsMatrix[j-1][i] && problem->student_events[i][event] && student_eventsMatrix[j+1][i]);
      }
      if ( j > 37){
	terVSC = terVSC + (double)(student_eventsMatrix[j-2][i] && student_eventsMatrix[j-1][i] && problem->student_events[i][event]);
      }
    }
    // Last timeslot of the day
    //
    if (j==44){
      terVSC = terVSC + (double)problem->studentNumber[event];
    }
    //only one event on a day
    //
    for (int i=0; i < problem->n_of_students; i++){
      if (problem->student_events[i][event] == 1){
	if (student_dayMatrix[4][i] == 0){
	  terVSC = terVSC + 1.0;
	}
      }
    }
    //cout << "terVSC:" << terVSC << endl;
    piij[j] = pow(1.0/(1.0+terVSC),GAMMA);
    //    cout << "Timeslot:" << j << " piij:" << piij[j] << endl;
  }

  if (q > Q0){

    divisor = 0.0;
    for (int j=0; j < 45; j++){
      divisor = divisor + (tauij[j]*etaij[j]*piij[j]);
    }
    for (int i=0; i < 45; i++){
      dividend = tauij[i] * etaij[i] * piij[i];
      roulette[i] = (dividend / divisor);
    }

    // *********************************
#ifdef DEBUG
//    cout << "Roulette vector for ant" << this <<":" << endl;
//    cout << "---" << endl;
//        for (int i=0; i < 9; i++){
//	  cout << "[" << i << "]:" << roulette[i] << "    \t Tauij:" << tauij[i] << "\t Etaij:" << etaij[i] << "\t Piij:" << piij[i] << endl;
//        }
//        cout << "---" << endl;
//        for (int i=9; i < 18; i++){
//          cout << "[" << i << "]:" << roulette[i] << "    \t Tauij:" << tauij[i] << "\t Etaij:" << etaij[i] << "\t Piij:" << piij[i] << endl;
//        }
//        cout << "---" << endl;
//        for (int i=18; i < 27; i++){
//          cout << "[" << i << "]:" << roulette[i] << "    \t Tauij:" << tauij[i] << "\t Etaij:" << etaij[i] << "\t Piij:" << piij[i] << endl;
//        }
//        cout << "---" << endl;
//        for (int i=27; i < 36; i++){
//          cout << "[" << i << "]:" << roulette[i] << "    \t Tauij:" << tauij[i] << "\t Etaij:" << etaij[i] << "\t Piij:" << piij[i] << endl;
//        }
//        cout << "---" << endl;
//        for (int i=36; i < 45; i++){
//	  cout << "[" << i << "]:" << roulette[i] << "    \t Tauij:" << tauij[i] << "\t Etaij:" << etaij[i] << "\t Piij:" << piij[i] << endl;
//        }
//        cout << endl;
#endif
    // *********************************

    for (int i=0; i < 45; i++){
      pij = pij + roulette[i];
      if (q1 < pij){
#ifdef DEBUG
//	cout << "Timeslot choosen:" << i << " thanks to probability:" << q1 << endl;
#endif
	return (i);
      }
    }
  }
  else {
    std::vector<int> set_of_max;
    double argmax[45];
    double maxargmax = -1.0;

    for (int i=0; i < 45; i++){
      argmax[i] = tauij[i] * etaij[i] * piij[i];
      if (argmax[i] > maxargmax){
	set_of_max.clear();
	set_of_max.push_back(i);
	maxargmax = argmax[i];
      }
      else {
	if ( argmax[i] == maxargmax) {
	  set_of_max.push_back(i);
	}
      }
    }
    int max = ((int)set_of_max.size() - 1);
    double rand;
    rand = rnd->next();
    rand = rand * (double)(max + 1);
    if (rand == ((double)(max + 1))) {
      rand = rand - 0.1;
    }
    rand = floor(rand);
#ifdef DEBUG
//    cout << "Timeslot choosen:" << rand << " thanks to argmax:" << maxargmax << " amongst " << set_of_max.size() << " elements" << endl;
#endif
    return((int)rand);
  }
}
