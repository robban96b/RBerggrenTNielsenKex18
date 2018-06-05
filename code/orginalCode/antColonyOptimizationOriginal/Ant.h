/****************************************************************************
                          Ant.h - description
                             -------------
begin                    : Thursday March 14 14:12:00 CET 2002
copyright                : (C) 2002 by Max Manfrin
email                    : mmanfrin@ulb.ac.be
****************************************************************************/

/****************************************************************************
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 ***************************************************************************/
#ifndef ANT_H
#define ANT_H

#include "Solution.h"
#include "Random.h"

#include <limits.h>
#include <math.h>
#include <vector>

extern Problem* problem;
extern std::vector<int> ordered_events;
extern Random* rnd;
extern double Q0;
extern double** pheromoneMatrix;
extern double GAMMA;
extern double BETA;

class Ant{

 public:

  int current_event;
  Solution* currentSolution;
  std::vector<int> candidate_list; /* List of candidate timeslots, will exclude the ones with too many events inside */
  double* tauij;
  double* etaij;
  double* piij;
  double* roulette; /* Vector of elements pij */
  int fitness;
  int** student_eventsMatrix;
  int** student_dayMatrix;

  Ant();
  ~Ant();
  void solutionStep(const int step);
  int chooseTimeslot(const int event);
};

#endif
