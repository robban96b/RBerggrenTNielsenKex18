/* -*- mode: c++; mode: font-lock -*- */
#include "Solution.h"

double TIMEINTERVALPRINT = 10.0;

void Solution::simulatedAnnealing_LS(std::ostream &outfile, double time_limit, double alfa, int nrep,
				     double iniTempFactorHcv, double iniTempFactorScv,
				     double rejectedLimHcv, double rejectedLimScv,
				     double prob1, double prob2, double prob3)
{
#ifdef HARDDEBUG
  int ngv,cgv;
  std::cout << "HARDDEBUG ACTIVE"<<std::endl;
#endif

  // perform local search with given time limit and probabilities for each type of move
  bool stillTime=1;
  int delta=0;
  int rejected=0;
  int tempIterCount=0;
  int countAlfa=0;

  Solution *bestSolution = new Solution( data, rg );
  bestSolution->copy(this);
  int bestHcv = computeHcv();
  int bestScv = computeScv();
  int currentTotHcv=bestHcv;
  int currentTotScv=bestScv;
#ifdef METAOUTPUT
  double time;
  std::ostream *mos;
  //if ()
   mos = &outfile;//new std::ofstream( getStringParameter( "-o" ).c_str() );
   //  else {
   //std::cerr << "Warning: No output file given for new best solution and CPU time, writing to stderr" << std::endl;
   //mos = &std::cerr;
  //}
#endif

#ifdef SOFTDEBUG
  int highTemp=0;
  std::cout<<"SOFTDEBUG ACTIVE\n";
#endif
  timer.resetTime(); // reset time counter for the local search

  //Setting the temperature dpending on the distance from random sampling
  //--------------------------------------------------
  int recordHcv=0;
  int recordScv=0;
  int numSamp=100;
  for (int samp=0; samp<numSamp; samp++)
    {
      Solution *neighbourSolution = new Solution( data, rg );
      neighbourSolution->copy(this);
      neighbourSolution->randomMove();
      recordHcv+=abs(neighbourSolution->computeHcv()-bestHcv);
      recordScv+=abs(neighbourSolution->computeScv()-bestScv);
      delete neighbourSolution;
    }
  double iniTempHcv = iniTempFactorHcv *(double)recordHcv/numSamp;
  double iniTempScv = iniTempFactorScv *(double)recordScv/numSamp;

#ifdef DEBUG
  std::cout<<"Initialization of temperature for Hcv: "<<iniTempHcv<<"\n";
  std::cout<<"Initialization of temperature for Scv: "<<iniTempScv<<"\n";
  std::cout << "Max num. of rejected moves acceptable: "<<rejectedLimHcv<<std::endl;
  std::cout << "Max num. of rejected moves acceptable: "<<rejectedLimScv<<std::endl;
  //--------------------------------------------------

  std::cout<<"DEBUG ACTIVE\n";
  std::ofstream *tempFile = new std::ofstream("sar/temp.dat");
  std::ofstream *currHcvFile = new std::ofstream("sar/currHcv.dat");
  std::ofstream *currScvFile = new std::ofstream("sar/currScv.dat");
  std::ofstream *ratioFile = new std::ofstream("sar/ratio.dat");
  (*tempFile) <<"Temp  stepCount" <<"\n";
  (*currHcvFile) << "Hcv  Scv  stepCount  delta"<<"\n";
  (*currScvFile) << "Scv  bestScv  stepCount  delta"<<"\n";
  (*ratioFile) <<"R T  Ratio  stepCount" << std::endl;
  int MAXITER=992434800;
  std::cout <<"Max number of steps: "<<MAXITER<<"\n";
  std::cout<<"Simulated Annealing in Local Search...."<<std::endl;
#endif
  double Temp = iniTempHcv;

 int eventList[data->n_of_events]; // keep a list of events to go through
 /*    for(int i = 0; i < data->n_of_events; i++)
	  eventList[i] = i;
	for(int i = 0; i < data->n_of_events; i++){ // scramble the list of events to obtain a random order
	  int j = (int)(rg->next()*data->n_of_events);
	  int h = eventList[i];
	  eventList[i] = eventList[j];
	  eventList[j] = h;
	  }*/

  /*std::cout <<"event list" <<std::endl;
    for(int i = 0 ; i< data->n_of_events; i++)
    std::cout<< eventList[i] << " ";
    std::cout << std::endl;*/

  int neighbourAffectedHcv = 0; // partial evaluation of neighbour solution hcv
  int neighbourScv = 0; // partial evaluation of neighbour solution scv
  int evCount = 0;     // counter of events considered
  int stepCount = 0; // set step counter to zero
  int moveperformed = false;
  // MAIN TIME LOOP ------------------------------------------------------------
  computeFeasibility();

    //UNFEASIBLE REGION LOOP --------------------------------------------------
  while (stillTime) {

        if (moveperformed == false)
      {

	for(int i = 0; i < data->n_of_events; i++)
	  eventList[i] = i;
	for(int i = 0; i < data->n_of_events; i++){ // scramble the list of events to obtain a random order
	  int j = (int)(rg->next()*data->n_of_events);
	  int h = eventList[i];
	  eventList[i] = eventList[j];
	  eventList[j] = h;
	}
	}
    moveperformed = false;


    if (currentTotHcv==0)
      feasible=true;

    if(!feasible ){ // if the timetable is not feasible try to solve hcv
      for( int i = 0; evCount < data->n_of_events; i = (i+1)% data->n_of_events){
	//std::cout<<stepCount<<"\n";
	if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	  {
	    stillTime=0;
	    break;
	  }
	int currentHcv = eventHcv(eventList[i]);
	if(currentHcv == 0 ){ // if the event on the list does not cause any hcv
	  evCount++; // increase the counter
	  continue; // go to the next event
	}


#ifdef DEBUG
	if (stepCount>MAXITER)
	  {
	    stillTime=0;
	    break;
	  }
	(*tempFile) << Temp << "  " << stepCount <<"\n";
	(*currHcvFile) << computeHcv() << "  0" <<  "  " <<stepCount<<"  "<<delta<<"\n";
	(*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
	//if (tempIterCount>0)
	//  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif
	if (countAlfa>nrep)
	  {
	    Temp = alfa * Temp;


	    countAlfa=0;
#ifdef DEBUG
	    if (tempIterCount>=(3*nrep))
	      (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

	  }
	    if ((tempIterCount>=(3*nrep)) && ((double)(rejected)/tempIterCount)>=rejectedLimHcv)
	      {

		Temp = Temp + iniTempHcv;
#ifdef SOFTDEBUG
		highTemp++;
#endif
		 rejected=0;
		 tempIterCount=0;
	      }

	// otherwise if the event in consideration caused hcv
	int currentAffectedHcv;
	int t_start = (int)(rg->next()*45); // try moves of type 1
	int t_orig = sln[eventList[i]].first;
	for(int h = 0, t = t_start; h < 45; t= (t+1)%45, h++){
	  if (currentTotHcv == 0)
	    {
	      //bestSolution->copy(this);
	      //bestHcv = currentTotHcv;
	      break;
	    }
	  if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	    {
	      stillTime=0;
	      break;
	    }



	  if(rg->next() < prob1){ // with given probability
	    stepCount++; countAlfa++;
	    //std::cout<<stepCount<<" 1 \n";
	    Solution *neighbourSolution = new Solution( data, rg );
	    neighbourSolution->copy( this );
	    //std::cout<< "event " << eventList[i] << " timeslot " << t << std::endl;
	    neighbourSolution->Move1(eventList[i],t);
	    neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->affectedRoomInTimeslotHcv(t_orig);
	    currentAffectedHcv = eventAffectedHcv(eventList[i]) + affectedRoomInTimeslotHcv(t);
	    delta = neighbourAffectedHcv - currentAffectedHcv;
	    if(delta<0){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotHcv = currentTotHcv + delta;
#ifdef METAOUTPUT
	      if (currentTotHcv<bestHcv)
		{
		  bestHcv=currentTotHcv;
		  time=timer.elapsedTime(Timer::VIRTUAL);
		  (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
		}
#endif
	      evCount = 0;
	      moveperformed = true;
	      tempIterCount++;
	      break;
	    }
	    else if ((rg->next()) < (exp((double)(-delta)/Temp))){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotHcv=currentTotHcv+delta;
	      evCount = 0;
	      moveperformed = true;
	      //EHI
	      if (delta==0)
		rejected++;
	     tempIterCount++;
	      break;
	    }
	    else
	      {
		rejected++;
		delete neighbourSolution;
		tempIterCount++;
	      }
	  }
	}
	if(moveperformed){
	  //rejected=0;
	  moveperformed = false;
	  continue;
	}
	if(prob2 != 0){
	  for(int j= (i+1)%data->n_of_events; j != i ;j = (j+1)%data->n_of_events){ // try moves of type 2
	    if (currentTotHcv == 0)
	      {
		//bestSolution->copy(this);
		//bestHcv = currentTotHcv;
		break;
	      }

	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }




	    if(rg->next() < prob2){ // with given probability
	      stepCount++; countAlfa++;
	      //std::cout<<stepCount<<" 2 \n";
	      Solution *neighbourSolution = new Solution( data, rg );
	      neighbourSolution->copy( this );
	      neighbourSolution->Move2(eventList[i],eventList[j]);
	      //std::cout<< "event " << eventList[i] << " second event " << eventList[j] << std::endl;
	      neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i])+neighbourSolution->eventAffectedHcv(eventList[j]);
	      currentAffectedHcv = eventAffectedHcv(eventList[i]) + eventAffectedHcv(eventList[j]);
	      delta=neighbourAffectedHcv - currentAffectedHcv;
	      if(delta<0 ){
		copy( neighbourSolution );
		delete neighbourSolution;
		currentTotHcv=currentTotHcv+delta;

#ifdef METAOUTPUT
		if (currentTotHcv<bestHcv)
		  {
		    bestHcv=currentTotHcv;
		    time=timer.elapsedTime(Timer::VIRTUAL);
		    (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
		  }
#endif
		evCount = 0;
		moveperformed = true;
		tempIterCount++;
		break;
	      }
	      else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		copy( neighbourSolution );
		delete neighbourSolution;
		currentTotHcv=currentTotHcv+delta;
		  //EHI
	      if (delta==0)
		rejected++;
		evCount = 0;
		moveperformed = true;
		tempIterCount++;
		break;
	      }
	      else
		{
		  rejected++;
		  tempIterCount++;
		  delete neighbourSolution;
		}
	    }
	  }
	  if(moveperformed){
	    //rejected=0;
	    moveperformed = false;
	    continue;
	  }
	}
	if(prob3 != 0){
	  for(int j= (i+1)%data->n_of_events; j != i; j = (j+1)%data->n_of_events){ // try moves of type 3
	    if (currentTotHcv == 0)
	      {
		//bestSolution->copy(this);
		//bestHcv = currentTotHcv;
		break;
	      }

	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }




	    for(int k= (j+1)%data->n_of_events; k != i ; k = (k+1)%data->n_of_events){
	      if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
		{
		  stillTime=0;
		  break;
		}




	      if(rg->next() < prob3){ // with given probability
		stepCount++; countAlfa++;
		//std::cout<<stepCount<<"  3\n";
		currentAffectedHcv = eventAffectedHcv(eventList[i]) + eventAffectedHcv(eventList[j]) + eventAffectedHcv(eventList[k]);
		Solution *neighbourSolution = new Solution( data, rg );
		neighbourSolution->copy( this );
		neighbourSolution->Move3(eventList[i],eventList[j], eventList[k]); //try one of the to possible 3-cycle
		//std::cout<< "event " << eventList[i] << " second event " << eventList[j] << " third event "<< eventList[k] << std::endl;
		neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i])+ neighbourSolution->eventAffectedHcv(eventList[j])
		  + neighbourSolution->eventAffectedHcv(eventList[k]);
		delta = neighbourAffectedHcv - currentAffectedHcv;
		if( delta<0 ){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		      mos->flags( std::ios::fixed );
		      (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
		    }
#endif
		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
  //EHI
	      if (delta==0)
		rejected++;
		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else
		  {
		    rejected++;
		    tempIterCount++;
		    delete neighbourSolution;
		  }
	      }
	      if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
		{
		  stillTime=0;
		  break;
		}



	      if(rg->next() < prob3){  // with given probability
		stepCount++; countAlfa++;
		//std::cout<<stepCount<<" 3b \n";
		currentAffectedHcv = eventAffectedHcv(eventList[i]) + eventAffectedHcv(eventList[k]) + eventAffectedHcv(eventList[j]);
		Solution *neighbourSolution = new Solution( data, rg );
		neighbourSolution->copy( this );
		neighbourSolution->Move3(eventList[i],eventList[k], eventList[j]); //try one of the to possible 3-cycle
		//std::cout<< "event " << eventList[i] << " second event " << eventList[j] << " third event "<< eventList[k] << std::endl;
		neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i])+ neighbourSolution->eventAffectedHcv(eventList[k])
		  + neighbourSolution->eventAffectedHcv(eventList[j]);
		delta=neighbourAffectedHcv - currentAffectedHcv;
		if( delta<0 ){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		    (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
		    }
#endif
		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
		    //EHI
	      if (delta==0)
		rejected++;
		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else{
		  rejected++;
		  tempIterCount++;
		  delete neighbourSolution;
		}
	      }
	    }
	    if(moveperformed)
	      {
		//rejected=0;
		break;
	      }
	  }
	  if(moveperformed){
	    moveperformed = false;
	    continue;
	  }
	}
	evCount++;
      }
    }
    else
      break;
  }

  //FEASIBLE REGION LOOP --------------------------------------------------
  rejected=0;
  countAlfa=0;
  tempIterCount=0;
  moveperformed = true;
  Temp = iniTempScv;
  currentTotScv=computeScv();
  while (stillTime) {
         if (moveperformed == false)
      {
	for(int i = 0; i < data->n_of_events; i++)
	  eventList[i] = i;
	for(int i = 0; i < data->n_of_events; i++){ // scramble the list of events to obtain a random order
	  int j = (int)(rg->next()*data->n_of_events);
	  int h = eventList[i];
	  eventList[i] = eventList[j];
	  eventList[j] = h;
	}
	}
      moveperformed = false;
    //computeFeasibility();
    // if(feasible){ // if the timetable is feasible
    evCount = 0;
    int neighbourHcv;
    for( int i = 0; evCount < data->n_of_events; i = (i+1)% data->n_of_events){ //go through the events in the list
      if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	{
	  stillTime=0;
	  break;
	}
      int currentScv = eventScv(eventList[i]);
      //std::cout << "event " << eventList[i] << " cost " << currentScv<<std::endl;
      if(currentScv == 0 ){ // if there are no scv
	evCount++; // increase counter
	continue;  //go to the next event
      }

#ifdef DEBUG
      if (stepCount>MAXITER)
	{
	  stillTime=0;
	  break;
	}
      (*tempFile) << Temp << "  " << stepCount <<"\n";
      (*currHcvFile) << computeHcv() << "   0"  << "  " <<stepCount<<"  "<<delta<<"\n";
      (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
      //  if (tempIterCount>0)
      //(*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

      if (countAlfa>nrep)
	{
	  Temp = alfa * Temp;


	  countAlfa=0;
	}
#ifdef DEBUG
	  if (tempIterCount>=(3*nrep))
	    (*ratioFile) <<"0 0 "<<((double)(rejected))/tempIterCount<<" "<< stepCount << std::endl;
#endif
	  if ((tempIterCount>=(3*nrep)) && ((double)(rejected)/tempIterCount)>=rejectedLimScv)
	    {
	      Temp = Temp + iniTempScv;

#ifdef SOFTDEBUG
	      highTemp++;
#endif
	      rejected=0;
	      tempIterCount=0;
	    }


      // otherwise try all the possible moves
      int t_start = (int)(rg->next()*45); // try moves of type 1
      for(int h= 0, t = t_start; h < 45; t= (t+1)%45, h++){
	if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	  {
	    stillTime=0;
	    break;
	  }
	if(rg->next() < prob1){ // each with given propability
	  stepCount++; 	  countAlfa++;
	  Solution *neighbourSolution = new Solution( data, rg );
	  neighbourSolution->copy( this );
	  neighbourSolution->Move1(eventList[i],t);
	  //std::cout<< "event " << eventList[i] << " timeslot " << t << std::endl;
	  neighbourHcv =  neighbourSolution->eventAffectedHcv(eventList[i]); //count possible hcv introduced by move
	  if(neighbourHcv == 0){ // consider the move only if no hcv are introduced
	    neighbourScv = neighbourSolution->eventScv(eventList[i])  // respectively Scv involving event e
	      + singleClassesScv(eventList[i]) // + single classes introduced in day of original timeslot
	      - neighbourSolution->singleClassesScv(eventList[i]); // - single classes "solved" in new day
	    //std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
	    delta = neighbourScv - currentScv;

#ifdef HARDDEBUG
	    std::cout << "Move 1: " << eventList[i]<<","<<t
		 <<" neighbourScv = "<<neighbourSolution->eventScv(eventList[i])<<"+"<<singleClassesScv(eventList[i])
		 <<"-"<< neighbourSolution->singleClassesScv(eventList[i])<<"="<<neighbourScv<<" ";
	    std::cout << "| currentScv = "<<currentScv<<" \n ";
	    ngv=neighbourSolution->computeScv();
	    cgv=computeScv();
	    std::cout << "Real neighScv  = "<<ngv<<" Real currScv = "<<cgv<<"\n ";
	    std::cout <<"Delta = "<<delta<<" Real = " <<ngv-cgv<<"\n";
	    if (delta!=(ngv-cgv))
	      {
		exit(EXIT_FAILURE);
	      }
#endif

	    if(delta<0){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotScv = currentTotScv + delta;//computeScv();//currentTotScv+delta;
	      if (currentTotScv < bestScv)
		{
		  bestSolution->copy(this);
		  bestScv = currentTotScv;
#ifdef METAOUTPUT
		  time=timer.elapsedTime(Timer::VIRTUAL);
		  (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
		}

	      evCount = 0;
	      moveperformed = true;
	      tempIterCount++;
	      break;
	    }
	    else if ((rg->next()) < (exp((double)(-delta)/Temp))){
	      //std::cout << " "<<delta<<" " <<Temp<<" "<<exp((double)(-delta)/Temp)<<" "<< rg->next()<<" \n" ;
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotScv = currentTotScv+delta;
	        //EHI
	      if (delta==0)
		  rejected++;
	      evCount = 0;
	      moveperformed = true;
	      tempIterCount++;
	      break;
	    }
	    else
	    {
	      tempIterCount++;
	      rejected++;
	    }
	  }

	  delete neighbourSolution;
	}
      }
      if(moveperformed){
	//rejected=0;
	moveperformed = false;
	continue;
      }
      if(prob2 != 0){
	for(int j= (i+1)%data->n_of_events; j != i ;j = (j+1)%data->n_of_events){ //try moves of type 2
	  if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	    {
	      stillTime=0;
	      break;
	    }
	  if(rg->next() < prob2){ // with the given probability
	    stepCount++; countAlfa++;
	    Solution *neighbourSolution = new Solution( data, rg );
	    neighbourSolution->copy( this );
	    //std::cout<< "event " << eventList[i] << " second event " << eventList[j] << std::endl;
	    neighbourSolution->Move2(eventList[i],eventList[j]);
	    //count possible hcv introduced with the move
	    neighbourHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->eventAffectedHcv(eventList[j]);
	    if( neighbourHcv == 0){ // only if no hcv are introduced by the move
	      // compute alterations on scv for neighbour solution
	      neighbourScv =  neighbourSolution->eventScv(eventList[i]) + singleClassesScv(eventList[i]) - neighbourSolution->singleClassesScv(eventList[i])
		+ neighbourSolution->eventScv(eventList[j]) + singleClassesScv(eventList[j]) - neighbourSolution->singleClassesScv(eventList[j]);
	      // std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
	      delta=neighbourScv - (currentScv + eventScv(eventList[j]));

#ifdef HARDDEBUG
	      std::cout << "Move 2: " << eventList[i]<<","<<eventList[j]
		   <<" neighbourScv = "<<neighbourSolution->eventScv(eventList[i])<<"+"<<singleClassesScv(eventList[i])
		   <<"-"<< neighbourSolution->singleClassesScv(eventList[i])<<"="<<neighbourScv<<" ";
	      std::cout << "| currentScv = "<<currentScv<<"  + "<<eventScv(eventList[j])<<"="<<currentScv + eventScv(eventList[j])<<"\n";
	      ngv=neighbourSolution->computeScv();
	      cgv=computeScv();
	      std::cout << "Real neighScv  = "<<ngv<<" Real currScv = "<<cgv<<"\n ";
	      std::cout <<"Delta = "<<delta<<" Real = " <<ngv-cgv<<"\n";
	      if (delta!=(ngv-cgv))
		{
		  exit(EXIT_FAILURE);
		}
#endif


	      if(delta<0){ // if scv are reduced
		copy( neighbourSolution ); // do the move
		delete neighbourSolution;
		currentTotScv = currentTotScv + delta;
		if (currentTotScv < bestScv)
		  {
		    bestSolution->copy(this);
		    bestScv = currentTotScv;
#ifdef METAOUTPUT
		    time=timer.elapsedTime(Timer::VIRTUAL);
		    (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;

#endif
		  }

		evCount = 0;
		moveperformed = true;
		tempIterCount++;
		break;
	      }
	      else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		copy( neighbourSolution );
		delete neighbourSolution;
		currentTotScv = currentTotScv+delta;
		  //EHI
	      if (delta==0)
		rejected++;
		evCount = 0;
		moveperformed = true;
		tempIterCount++;
		break;
	      }
	      else
		{
		  tempIterCount++;
		  rejected++;
		}
	    }

	    delete neighbourSolution;
	  }
	}
	if(moveperformed){
	  //rejected=0;
	  moveperformed = false;
	  continue;
	}
      }
      if(prob3 != 0){
	for(int j= (i+1)%data->n_of_events; j != i; j = (j+1)%data->n_of_events){ //try moves of type 3
	  if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	    {
	      stillTime=0;
	      break;
	    }
	  for(int k= (j+1)%data->n_of_events; k != i ; k = (k+1)%data->n_of_events){
	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }
	    if(rg->next() < prob3){ // with given probability try one of the 2 possibles 3-cycles
	      stepCount++; countAlfa++;
	      Solution *neighbourSolution = new Solution( data, rg );
	      neighbourSolution->copy( this );
	      neighbourSolution->Move3(eventList[i],eventList[j], eventList[k]);
	      // std::cout<< "event " << eventList[i] << " second event " << eventList[j] << " third event "<< eventList[k] << std::endl;
	      // compute the possible hcv introduced by the move
	      neighbourHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->eventAffectedHcv(eventList[j])
		+ neighbourSolution->eventAffectedHcv(eventList[k]);
	      if(neighbourHcv == 0){ // consider the move only if hcv are not introduced
		// compute alterations on scv for neighbour solution
		neighbourScv = neighbourSolution->eventScv(eventList[i]) + singleClassesScv(eventList[i]) - neighbourSolution->singleClassesScv(eventList[i])
		  + neighbourSolution->eventScv(eventList[j]) + singleClassesScv(eventList[j]) - neighbourSolution->singleClassesScv(eventList[j])
		  + neighbourSolution->eventScv(eventList[k]) + singleClassesScv(eventList[k]) - neighbourSolution->singleClassesScv(eventList[k]);
		// std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
		delta =neighbourScv - (currentScv+eventScv(eventList[j])+eventScv(eventList[k]));
		if(delta<0){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv = currentTotScv + delta;
		  if (currentTotScv < bestScv)
		    {
		      bestSolution->copy(this);
		      bestScv = currentTotScv;
#ifdef METAOUTPUT
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
		    }

		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv = currentTotScv+delta;
		   //EHI
	      if (delta==0)
		rejected++;
		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else
		{
		  tempIterCount++;
		  rejected++;
		}
	      }

	      delete neighbourSolution;
	    }
	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }
	    if(rg->next() < prob3){ // with the same probability try the other possible 3-cycle for the same 3 events
	      stepCount++; countAlfa++;
	      Solution *neighbourSolution = new Solution( data, rg );
	      neighbourSolution->copy( this );
	      neighbourSolution->Move3(eventList[i],eventList[k], eventList[j]);
	      // std::cout<< "event " << eventList[i] << " second event " << eventList[k] << " third event "<< eventList[j] << std::endl;
	      // compute the possible hcv introduced by the move
	      neighbourHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->eventAffectedHcv(eventList[k])
		+ neighbourSolution->eventAffectedHcv(eventList[j]);
	      if(neighbourHcv == 0){ // consider the move only if hcv are not introduced
		// compute alterations on scv for neighbour solution
		neighbourScv = neighbourSolution->eventScv(eventList[i]) + singleClassesScv(eventList[i]) - neighbourSolution->singleClassesScv(eventList[i])
		  + neighbourSolution->eventScv(eventList[k]) + singleClassesScv(eventList[k]) - neighbourSolution->singleClassesScv(eventList[k])
		  + neighbourSolution->eventScv(eventList[j]) + singleClassesScv(eventList[j]) - neighbourSolution->singleClassesScv(eventList[j]);
		// std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
		delta =neighbourScv - (currentScv+eventScv(eventList[k])+eventScv(eventList[j]));
		if(delta <0){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv= currentTotScv+delta;
		  if (currentTotScv < bestScv)
		    {
		      bestSolution->copy(this);
		      bestScv = currentTotScv;
#ifdef METAOUTPUT
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
		    }

		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv = currentTotScv+delta;
		   //EHI
	      if (delta==0)
		rejected++;

		  evCount = 0;
		  moveperformed = true;
		  tempIterCount++;
		  break;
		}
		else
		{
		  tempIterCount++;
		  rejected++;
		}
	      }

	      delete neighbourSolution;
	    }
	  }
	  if(moveperformed)
	    {
	      //rejected=0;
	      break;
	    }
	}
	if(moveperformed){
	  //rejected=0;
	  moveperformed = false;
	  continue;
	}
      }
      evCount++;
    }
  }
#ifdef DEBUG
  (*tempFile) << Temp << "  " << stepCount <<"\n";
  (*currHcvFile) << computeHcv() << "  " << bestHcv << "  " <<stepCount<<"  "<<delta<<"\n";
  (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
#endif
  copy(bestSolution);
#ifdef SOFTDEBUG
  //std::cout<<"Best: " <<computeScv() << " iterations: " <<stepCount<<"\n";
  std::cout <<computeHcv()<<" "<< computeScv() << " " << stepCount << " " << highTemp <<std::endl;
#endif
}


void Solution::simulatedAnnealing_LS_Tfixed(std::ostream &outfile, double time_limit,
					    double TempFactorHcv, double TempFactorScv,
					    double prob1, double prob2, double prob3)
{
  // perform local search with given time limit and probabilities for each type of move
  bool stillTime=1;
  int delta=0;
#ifdef METAOUTPUT
  double time;
  std::ostream *mos;
  //  //  if ()
  //  //      *mos = new std::ofstream( getStringParameter( "-o" ).c_str() );
  //  //    else {
  //std::cerr << "Warning: No output file given for new best solution and CPU time, writing to stderr" << std::endl;
  mos = &outfile;
  //}
#endif
  Solution *bestSolution = new Solution( data, rg );
  bestSolution->copy(this);
  int bestHcv = computeHcv();
  int bestScv = computeScv();
  int currentTotHcv=bestHcv;
  int currentTotScv=bestScv;
#ifdef SOFTDEBUG
  std::cout<<"SOFTDEBUG ACTIVE\n";
#endif

  timer.resetTime(); // reset time counter for the local search

  //Setting the temperature dpending on the distance from random sampling
  //--------------------------------------------------
  int recordHcv=0;
  int recordScv=0;
  int numSamp=100;
  for (int samp=0; samp<numSamp; samp++)
    {
      Solution *neighbourSolution = new Solution( data, rg );
      neighbourSolution->copy(this);
      neighbourSolution->randomMove();
      recordHcv+=abs(neighbourSolution->computeHcv()-bestHcv);
      recordScv+=abs(neighbourSolution->computeScv()-bestScv);
      delete neighbourSolution;
    }
  double TempHcv = TempFactorHcv *(double)recordHcv/numSamp;
  double TempScv = TempFactorScv *(double)recordScv/numSamp;

#ifdef DEBUG
  std::cout<<"Temperature for Hcv: "<<TempHcv<<"\n";
  std::cout<<"Temperature for Scv: "<<TempScv<<"\n";
  //--------------------------------------------------

  std::cout<<"DEBUG ACTIVE\n";
  std::ofstream *tempFile = new std::ofstream("sar/temp.dat");
  std::ofstream *currHcvFile = new std::ofstream("sar/currHcv.dat");
  std::ofstream *currScvFile = new std::ofstream("sar/currScv.dat");
  std::ofstream *ratioFile = new std::ofstream("sar/ratio.dat");
  (*tempFile) <<"Temp  stepCount" <<"\n";
  (*currHcvFile) << "Hcv  Scv  stepCount  delta"<<"\n";
  (*currScvFile) << "Scv  bestScv  stepCount  delta"<<"\n";
  (*ratioFile) <<"R T  Ratio  stepCount" << std::endl;
  int MAXITER=992434800;
  std::cout <<"Max number of steps: "<<MAXITER<<"\n";
  std::cout<<"Simulated Annealing in Local Search with Fixed Temperature...."<<std::endl;
#endif
  double Temp = TempHcv;



  int eventList[data->n_of_events]; // keep a list of events to go through

  /*std::cout <<"event list" <<std::endl;
  for(int i = 0 ; i< data->n_of_events; i++)
    std::cout<< eventList[i] << " ";
    std::cout << std::endl;*/

  int neighbourAffectedHcv = 0; // partial evaluation of neighbour solution hcv
  int neighbourScv = 0; // partial evaluation of neighbour solution scv
  int evCount = 0;     // counter of events considered
  int stepCount = 0; // set step counter to zero
  int moveperformed = false;
  // MAIN TIME LOOP ------------------------------------------------------------
  computeFeasibility();
  while (stillTime) {
    if (moveperformed == false)
    {
      for(int i = 0; i < data->n_of_events; i++)
	eventList[i] = i;
      for(int i = 0; i < data->n_of_events; i++){ // scramble the list of events to obtain a random order
	int j = (int)(rg->next()*data->n_of_events);
	int h = eventList[i];
	eventList[i] = eventList[j];
	eventList[j] = h;
      }
    }
    moveperformed = false;

    //UNFEASIBLE REGION LOOP --------------------------------------------------
    if (currentTotHcv==0)
      feasible=true;

    if(!feasible ){ // if the timetable is not feasible try to solve hcv
      for( int i = 0; evCount < data->n_of_events; i = (i+1)% data->n_of_events){
	//std::cout<<stepCount<<"\n";
	if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	  {
	    stillTime=0;
	    break;
	  }
	int currentHcv = eventHcv(eventList[i]);
	if(currentHcv == 0 ){ // if the event on the list does not cause any hcv
	  evCount++; // increase the counter
	  continue; // go to the next event
	}


#ifdef DEBUG
	if (stepCount>MAXITER)
	  {
	    stillTime=0;
	    break;
	  }
	(*tempFile) << Temp << "  " << stepCount <<"\n";
	(*currHcvFile) << computeHcv() << "  0" <<  "  " <<stepCount<<"  "<<delta<<"\n";
	(*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
	(*ratioFile) <<"0 0 0 "<< stepCount << std::endl;
#endif

	// otherwise if the event in consideration caused hcv
	int currentAffectedHcv;
	int t_start = (int)(rg->next()*45); // try moves of type 1
	int t_orig = sln[eventList[i]].first;
	for(int h = 0, t = t_start; h < 45; t= (t+1)%45, h++){
	  if (currentTotHcv == 0)
	    {
	      //bestSolution->copy(this);
	      //bestHcv = currentTotHcv;
	      break;
	    }
	  if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	    {
	      stillTime=0;
	      break;
	    }



	  if(rg->next() < prob1){ // with given probability
	    stepCount++;
	    Solution *neighbourSolution = new Solution( data, rg );
	    neighbourSolution->copy( this );
	    //std::cout<< "event " << eventList[i] << " timeslot " << t << std::endl;
	    neighbourSolution->Move1(eventList[i],t);
	    neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->affectedRoomInTimeslotHcv(t_orig);
	    currentAffectedHcv = eventAffectedHcv(eventList[i]) + affectedRoomInTimeslotHcv(t);
	    delta = neighbourAffectedHcv - currentAffectedHcv;
	    if(delta<0){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotHcv = currentTotHcv + delta;
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
		    }
#endif
		  evCount = 0;
	      moveperformed = true;
	      break;
	    }
	    else if ((rg->next()) < (exp((double)(-delta)/Temp))){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotHcv=currentTotHcv+delta;
	      evCount = 0;
	      moveperformed = true;
	      break;
	    }
	    delete neighbourSolution;
	  }
	}
	if(moveperformed){
	  //rejected=0;
	  moveperformed = false;
	  continue;
	}
	if(prob2 != 0){
	  for(int j= (i+1)%data->n_of_events; j != i ;j = (j+1)%data->n_of_events){ // try moves of type 2
	    if (currentTotHcv == 0)
	      {
		//bestSolution->copy(this);
		//bestHcv = currentTotHcv;
		break;
	      }

	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }




	    if(rg->next() < prob2){ // with given probability
	      stepCount++;
	      Solution *neighbourSolution = new Solution( data, rg );
	      neighbourSolution->copy( this );
	      neighbourSolution->Move2(eventList[i],eventList[j]);
	      //std::cout<< "event " << eventList[i] << " second event " << eventList[j] << std::endl;
	      neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i])+neighbourSolution->eventAffectedHcv(eventList[j]);
	      currentAffectedHcv = eventAffectedHcv(eventList[i]) + eventAffectedHcv(eventList[j]);
	      delta=neighbourAffectedHcv - currentAffectedHcv;
	      if(delta<0 ){
		copy( neighbourSolution );
		delete neighbourSolution;
		currentTotHcv=currentTotHcv+delta;
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
		    }
#endif

		evCount = 0;
		moveperformed = true;
		break;
	      }
	      else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		copy( neighbourSolution );
		delete neighbourSolution;
		currentTotHcv=currentTotHcv+delta;

		evCount = 0;
		moveperformed = true;
		break;
	      }
	      else
		delete neighbourSolution;
	    }
	  }
	  if(moveperformed){
	    //rejected=0;
	    moveperformed = false;
	    continue;
	  }
	}
	if(prob3 != 0){
	  for(int j= (i+1)%data->n_of_events; j != i; j = (j+1)%data->n_of_events){ // try moves of type 3
	    if (currentTotHcv == 0)
	      {
		//bestSolution->copy(this);
		//bestHcv = currentTotHcv;
		break;
	      }

	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }




	    for(int k= (j+1)%data->n_of_events; k != i ; k = (k+1)%data->n_of_events){
	      if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
		{
		  stillTime=0;
		  break;
		}




	      if(rg->next() < prob3){ // with given probability
		stepCount++;
		currentAffectedHcv = eventAffectedHcv(eventList[i]) + eventAffectedHcv(eventList[j]) + eventAffectedHcv(eventList[k]);
		Solution *neighbourSolution = new Solution( data, rg );
		neighbourSolution->copy( this );
		neighbourSolution->Move3(eventList[i],eventList[j], eventList[k]); //try one of the to possible 3-cycle
		//std::cout<< "event " << eventList[i] << " second event " << eventList[j] << " third event "<< eventList[k] << std::endl;
		neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i])+ neighbourSolution->eventAffectedHcv(eventList[j])
		  + neighbourSolution->eventAffectedHcv(eventList[k]);
		delta = neighbourAffectedHcv - currentAffectedHcv;
		if( delta<0 ){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		     (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;

		    }
#endif
		  evCount = 0;
		  moveperformed = true;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
		  evCount = 0;
		  moveperformed = true;
		  break;
		}
		else
		  {

		    delete neighbourSolution;
		  }
	      }
	      if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
		{
		  stillTime=0;
		  break;
		}



	      if(rg->next() < prob3){  // with given probability
		stepCount++;
		currentAffectedHcv = eventAffectedHcv(eventList[i]) + eventAffectedHcv(eventList[k]) + eventAffectedHcv(eventList[j]);
		Solution *neighbourSolution = new Solution( data, rg );
		neighbourSolution->copy( this );
		neighbourSolution->Move3(eventList[i],eventList[k], eventList[j]); //try one of the to possible 3-cycle
		//std::cout<< "event " << eventList[i] << " second event " << eventList[j] << " third event "<< eventList[k] << std::endl;
		neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(eventList[i])+ neighbourSolution->eventAffectedHcv(eventList[k])
		  + neighbourSolution->eventAffectedHcv(eventList[j]);
		delta=neighbourAffectedHcv - currentAffectedHcv;
		if( delta<0 ){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		     (*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
		  mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;

		    }
#endif
		  evCount = 0;
		  moveperformed = true;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotHcv=currentTotHcv+delta;
		  evCount = 0;
		  moveperformed = true;
		  break;
		}
		else
		  delete neighbourSolution;
	      }
	    }
	    if(moveperformed)
	      break;
	  }
	  if(moveperformed){
	    moveperformed = false;
	    continue;
	  }
	}
	evCount++;
      }
    }
    else
      break;
  }

  //FEASIBLE REGION LOOP --------------------------------------------------
 moveperformed = true;
  Temp = TempScv;
  currentTotScv=computeScv();
  while (stillTime) {
     if (moveperformed == false)
       {
	 for(int i = 0; i < data->n_of_events; i++)
	   eventList[i] = i;
	 for(int i = 0; i < data->n_of_events; i++){ // scramble the list of events to obtain a random order
	   int j = (int)(rg->next()*data->n_of_events);
	   int h = eventList[i];
	   eventList[i] = eventList[j];
	   eventList[j] = h;
	 }
       }
     moveperformed = false;
    //computeFeasibility();
    // if(feasible){ // if the timetable is feasible
    evCount = 0;
    int neighbourHcv;
    for( int i = 0; evCount < data->n_of_events; i = (i+1)% data->n_of_events){ //go through the events in the list
      if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	{
	  stillTime=0;
	  break;
	}
      int currentScv = eventScv(eventList[i]);
      //std::cout << "event " << eventList[i] << " cost " << currentScv<<std::endl;
      if(currentScv == 0 ){ // if there are no scv
	evCount++; // increase counter
	continue;  //go to the next event
      }

#ifdef DEBUG
      if (stepCount>MAXITER)
	{
	  stillTime=0;
	  break;
	}
      (*tempFile) << Temp << "  " << stepCount <<"\n";
      (*currHcvFile) << computeHcv() << "   0"  << "  " <<stepCount<<"  "<<delta<<"\n";
      (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
      (*ratioFile) <<"0 0 0 "<< stepCount << std::endl;
#endif

      // otherwise try all the possible moves
      int t_start = (int)(rg->next()*45); // try moves of type 1
      for(int h= 0, t = t_start; h < 45; t= (t+1)%45, h++){
	if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	  {
	    stillTime=0;
	    break;
	  }
	if(rg->next() < prob1){ // each with given propability
	  stepCount++;
	  Solution *neighbourSolution = new Solution( data, rg );
	  neighbourSolution->copy( this );
	  neighbourSolution->Move1(eventList[i],t);
	  //std::cout<< "event " << eventList[i] << " timeslot " << t << std::endl;
	  neighbourHcv =  neighbourSolution->eventAffectedHcv(eventList[i]); //count possible hcv introduced by move
	  if(neighbourHcv == 0){ // consider the move only if no hcv are introduced
	    neighbourScv = neighbourSolution->eventScv(eventList[i])  // respectively Scv involving event e
	      + singleClassesScv(eventList[i]) // + single classes introduced in day of original timeslot
	      - neighbourSolution->singleClassesScv(eventList[i]); // - single classes "solved" in new day
	    //std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
	    delta = neighbourScv - currentScv;
	    if(delta<0){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotScv = currentTotScv+delta;
	      if (currentTotScv < bestScv)
		{
		  bestSolution->copy(this);
		  bestScv = currentTotScv;
#ifdef METAOUTPUT
		  time=timer.elapsedTime(Timer::VIRTUAL);
		  (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
		}

	      evCount = 0;
	      moveperformed = true;
	      break;
	    }
	    else if ((rg->next()) < (exp((double)(-delta)/Temp))){
	      copy( neighbourSolution );
	      delete neighbourSolution;
	      currentTotScv = currentTotScv+delta;

	      evCount = 0;
	      moveperformed = true;
	      break;
	    }
	  }
	  delete neighbourSolution;
	}
      }
      if(moveperformed){
	//rejected=0;
	moveperformed = false;
	continue;
      }
      if(prob2 != 0){
	for(int j= (i+1)%data->n_of_events; j != i ;j = (j+1)%data->n_of_events){ //try moves of type 2
	  if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	    {
	      stillTime=0;
	      break;
	    }
	  if(rg->next() < prob2){ // with the given probability
	    stepCount++;
	    Solution *neighbourSolution = new Solution( data, rg );
	    neighbourSolution->copy( this );
	    //std::cout<< "event " << eventList[i] << " second event " << eventList[j] << std::endl;
	    neighbourSolution->Move2(eventList[i],eventList[j]);
	    //count possible hcv introduced with the move
	    neighbourHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->eventAffectedHcv(eventList[j]);
	    if( neighbourHcv == 0){ // only if no hcv are introduced by the move
	      // compute alterations on scv for neighbour solution
	      neighbourScv =  neighbourSolution->eventScv(eventList[i]) + singleClassesScv(eventList[i]) - neighbourSolution->singleClassesScv(eventList[i])
		+ neighbourSolution->eventScv(eventList[j]) + singleClassesScv(eventList[j]) - neighbourSolution->singleClassesScv(eventList[j]);
	      // std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
	      delta=neighbourScv - (currentScv + eventScv(eventList[j]));
	      if(delta<0){ // if scv are reduced
		copy( neighbourSolution ); // do the move
		delete neighbourSolution;
		currentTotScv = currentTotScv + delta;
		if (currentTotScv < bestScv)
		  {
		    bestSolution->copy(this);
		    bestScv = currentTotScv;
#ifdef METAOUTPUT
		    time=timer.elapsedTime(Timer::VIRTUAL);
		     (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;

#endif
		  }

		evCount = 0;
		moveperformed = true;
		break;
	      }
	      else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		copy( neighbourSolution );
		delete neighbourSolution;
		currentTotScv = currentTotScv+delta;

		evCount = 0;
		moveperformed = true;
		break;
	      }
	    }
	    delete neighbourSolution;
	  }
	}
	if(moveperformed){
	  //rejected=0;
	  moveperformed = false;
	  continue;
	}
      }
      if(prob3 != 0){
	for(int j= (i+1)%data->n_of_events; j != i; j = (j+1)%data->n_of_events){ //try moves of type 3
	  if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	    {
	      stillTime=0;
	      break;
	    }
	  for(int k= (j+1)%data->n_of_events; k != i ; k = (k+1)%data->n_of_events){
	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }
	    if(rg->next() < prob3){ // with given probability try one of the 2 possibles 3-cycles
	      stepCount++;
	      Solution *neighbourSolution = new Solution( data, rg );
	      neighbourSolution->copy( this );
	      neighbourSolution->Move3(eventList[i],eventList[j], eventList[k]);
	      // std::cout<< "event " << eventList[i] << " second event " << eventList[j] << " third event "<< eventList[k] << std::endl;
	      // compute the possible hcv introduced by the move
	      neighbourHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->eventAffectedHcv(eventList[j])
		+ neighbourSolution->eventAffectedHcv(eventList[k]);
	      if(neighbourHcv == 0){ // consider the move only if hcv are not introduced
		// compute alterations on scv for neighbour solution
		neighbourScv = neighbourSolution->eventScv(eventList[i]) + singleClassesScv(eventList[i]) - neighbourSolution->singleClassesScv(eventList[i])
		  + neighbourSolution->eventScv(eventList[j]) + singleClassesScv(eventList[j]) - neighbourSolution->singleClassesScv(eventList[j])
		  + neighbourSolution->eventScv(eventList[k]) + singleClassesScv(eventList[k]) - neighbourSolution->singleClassesScv(eventList[k]);
		// std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
		delta =neighbourScv - (currentScv+eventScv(eventList[j])+eventScv(eventList[k]));
		if(delta<0){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv = currentTotScv + delta;
		  if (currentTotScv < bestScv)
		    {
		      bestSolution->copy(this);
		      bestScv = currentTotScv;
#ifdef METAOUTPUT
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
		    }

		  evCount = 0;
		  moveperformed = true;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv = currentTotScv+delta;

		  evCount = 0;
		  moveperformed = true;
		  break;
		}
	      }
	      delete neighbourSolution;
	    }
	    if (timer.elapsedTime(Timer::VIRTUAL) > time_limit)
	      {
		stillTime=0;
		break;
	      }
	    if(rg->next() < prob3){ // with the same probability try the other possible 3-cycle for the same 3 events
	      stepCount++;
	      Solution *neighbourSolution = new Solution( data, rg );
	      neighbourSolution->copy( this );
	      neighbourSolution->Move3(eventList[i],eventList[k], eventList[j]);
	      // std::cout<< "event " << eventList[i] << " second event " << eventList[k] << " third event "<< eventList[j] << std::endl;
	      // compute the possible hcv introduced by the move
	      neighbourHcv = neighbourSolution->eventAffectedHcv(eventList[i]) + neighbourSolution->eventAffectedHcv(eventList[k])
		+ neighbourSolution->eventAffectedHcv(eventList[j]);
	      if(neighbourHcv == 0){ // consider the move only if hcv are not introduced
		// compute alterations on scv for neighbour solution
		neighbourScv = neighbourSolution->eventScv(eventList[i]) + singleClassesScv(eventList[i]) - neighbourSolution->singleClassesScv(eventList[i])
		  + neighbourSolution->eventScv(eventList[k]) + singleClassesScv(eventList[k]) - neighbourSolution->singleClassesScv(eventList[k])
		  + neighbourSolution->eventScv(eventList[j]) + singleClassesScv(eventList[j]) - neighbourSolution->singleClassesScv(eventList[j]);
		// std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
		delta =neighbourScv - (currentScv+eventScv(eventList[k])+eventScv(eventList[j]));
		if(delta <0){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv= currentTotScv+delta;
		  if (currentTotScv < bestScv)
		    {
		      bestSolution->copy(this);
		      bestScv = currentTotScv;
#ifdef METAOUTPUT
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
		    }

		  evCount = 0;
		  moveperformed = true;
		  break;
		}
		else if ((rg->next()) < (exp((double)(-delta)/Temp))){
		  copy( neighbourSolution );
		  delete neighbourSolution;
		  currentTotScv = currentTotScv+delta;


		  evCount = 0;
		  moveperformed = true;
		  break;
		}
	      }
	      delete neighbourSolution;
	    }
	  }
	  if(moveperformed)
	    {
	      //rejected=0;
	      break;
	    }
	}
	if(moveperformed){
	  //rejected=0;
	  moveperformed = false;
	  continue;
	}
      }
      evCount++;
    }
  }
#ifdef DEBUG
  (*tempFile) << Temp << "  " << stepCount <<"\n";
  (*currHcvFile) << computeHcv() << "  " << bestHcv << "  " <<stepCount<<"  "<<delta<<"\n";
  (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
#endif
  copy(bestSolution);
#ifdef SOFTDEBUG
  //std::cout<<"Best: " <<computeScv() << " iterations: " <<stepCount<<"\n";
  std::cout <<computeHcv()<<" "<< computeScv() << " " << stepCount << " 0" <<std::endl;
#endif
}






void Solution::simulatedAnnealing_Random(std::ostream &outfile, double time_limit, double alfa, int nrep,
					 double iniTempFactorHcv, double iniTempFactorScv,
					 double rejectedLimHcv, double rejectedLimScv,
					 double prob1, double prob2, double prob3)
{
  // perform local search with given time limit and probabilities for each type of move

  int moveType,e1;
  int delta=0;
  int neighbourHcv = 0; // partial evaluation of neighbour solution hcv
  int neighbourScv = 0; // partial evaluation of neighbour solution scv
  int rejected=0;
  int tempIterCount=0;
  int countAlfa=0;

  double Temp;
  int stepCount=0;
  computeFeasibility();
  int currentTotHcv=computeHcv();
  int currentTotScv=computeScv();

  int neighbourAffectedHcv=0;
  int currentAffectedHcv=0;

#ifdef METAOUTPUT
  std::ostream *mos;
  //  //  if ()
  //  //      *mos = new std::ofstream( getStringParameter( "-o" ).c_str() );
  //  //    else {
  //std::cerr << "Warning: No output file given for new best solution and CPU time, writing to stderr" << std::endl;
  mos = &outfile;
  //}
  double time;
  int bestHcv=currentTotHcv;
#endif

  timer.resetTime(); // reset time counter for the local search

  //int bestScv=currentScv;
  //Setting the temperature dpending on the distance from random sampling
  //--------------------------------------------------
  int recordHcv=0;
  int recordScv=0;
  int numSamp=100;
  for (int samp=0; samp<numSamp; samp++)
    {
      Solution *neighbourSolution = new Solution( data,rg );
      neighbourSolution->copy(this);
      neighbourSolution->randomMove();
      recordHcv+=abs(neighbourSolution->computeHcv()-currentTotHcv);
      recordScv+=abs(neighbourSolution->computeScv()-currentTotScv);
      delete neighbourSolution;
    }
  double iniTempHcv = iniTempFactorHcv *(double)recordHcv/numSamp;
  double iniTempScv = iniTempFactorScv *(double)recordScv/numSamp;
#ifdef DEBUG
  std::cout<<"Initialization of temperature for Hcv: "<<iniTempHcv<<"\n";
  std::cout<<"Initialization of temperature for Scv: "<<iniTempScv<<"\n";

  std::cout << "Max num. of rejected moves in Hcv loop acceptable: "<<rejectedLimHcv<<std::endl;
  std::cout << "Max num. of rejected moves in Scv loop acceptable: "<<rejectedLimScv<<std::endl;
  std::cout<<"--------------------------------------------------\n";
  std::cout<<"Simulated Annealing Random...."<<std::endl;
  //--------------------------------------------------


  std::cout<<"DEBUG ACTIVE\n";
  std::ofstream *tempFile = new std::ofstream("sar/temp.dat");
  std::ofstream *currHcvFile = new std::ofstream("sar/currHcv.dat");
  std::ofstream *currScvFile = new std::ofstream("sar/currScv.dat");
  std::ofstream *ratioFile = new std::ofstream("sar/ratio.dat");
  (*tempFile) <<"Temp  stepCount" <<"\n";
  (*currHcvFile) << "Hcv  Scv  stepCount  delta"<<"\n";
  (*currScvFile) << "Scv  bestScv  stepCount  delta"<<"\n";
  (*ratioFile) <<"R T  Ratio  stepCount" << std::endl;
  int MAXITER=45000;

#endif

#ifdef SOFTDEBUG

  int highTemp=0;
  std::cout<<"SOFTDEBUG ACTIVE\n";
#endif

  Temp = iniTempHcv;
  timer.resetTime(); // reset time counter for the local search

  //UNFEASIBLE REGION LOOP --------------------------------------------------

  // if the timetable is not feasible try to solve hcv

  while(!feasible
	&& timer.elapsedTime(Timer::VIRTUAL) < time_limit){


#ifdef DEBUG
    if (stepCount>MAXITER)
      {
	time_limit=0;
	break;
      }
    (*tempFile) << Temp << "  " << stepCount <<"\n";
    (*currHcvFile) << computeHcv() << "  0" <<  "  " <<stepCount<<"  "<<delta<<"\n";
    (*currScvFile) << computeScv() << "  0" << "  " <<stepCount<<"  "<<delta<<"\n";
    //if (tempIterCount>0)
    //  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif
    if (countAlfa>nrep)
      {
	Temp = alfa * Temp;

	countAlfa=0;
      }
#ifdef DEBUG
	if (tempIterCount>=(3*nrep))
	  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

	if ((tempIterCount>=(3*nrep)) && ((double)(rejected)/tempIterCount)>=rejectedLimHcv)
	  {


	    Temp = Temp + iniTempHcv;
#ifdef SOFTDEBUG
	    highTemp++;
#endif
	    rejected=0;
	    tempIterCount=0;
	  }
    Solution *neighbourSolution = new Solution( data,rg );
    neighbourSolution->copy( this );
    //std::cout<< "event " << eventList[i] << " timeslot " << t << std::endl;
    stepCount++; countAlfa++;
    moveType = (int)(rg->next()*2) + 1;
    e1 = (int)(rg->next()*(data->n_of_events));
    if(moveType == 1){  // perform move of type 1
      int t = (int)(rg->next()*45);
      neighbourSolution->Move1( e1, t);
      //std::cout<< "event " << e1 << " in timeslot " << t << std::endl;
      int t_orig = sln[e1].first;
	neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(e1)
	  + neighbourSolution->affectedRoomInTimeslotHcv(t_orig);
	currentAffectedHcv = eventAffectedHcv(e1)
	  + affectedRoomInTimeslotHcv(t);
    }
    else if(moveType == 2){ // perform move of type 2
      int e2 = (int)(rg->next()*(data->n_of_events));
      while(e2 == e1) // take care of not swapping one event with itself
	e2 = (int)(rg->next()*(data->n_of_events));
      neighbourSolution->Move2( e1, e2);
      // std::cout << "e1 "<< e1 << " e2 " << e2 << std::endl;
      neighbourAffectedHcv = neighbourSolution->eventAffectedHcv(e1)
	  +neighbourSolution->eventAffectedHcv(e2);
	currentAffectedHcv = eventAffectedHcv(e1)
	  + eventAffectedHcv(e2);
    }

    //neighbourHcv = neighbourSolution->computeHcv();
    delta = neighbourAffectedHcv-currentAffectedHcv;
    if( delta<0 ){
      copy( neighbourSolution );
      delete neighbourSolution;
      currentTotHcv = currentTotHcv + delta;//computeHcv();
#ifdef METAOUTPUT
		  if (currentTotHcv<bestHcv)
		    {
		      bestHcv=currentTotHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
(*mos) << "best " << (bestHcv * 1000000) + computeScv() << " time ";
mos->flags( std::ios::fixed );
		  (*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
}
#endif
      if (currentTotHcv == 0)
	{
	  feasible=true;
	}
      tempIterCount++;
    }
    else if (SAcriteria(delta,Temp)){
      copy( neighbourSolution );
      currentTotHcv=currentTotHcv+delta;
          //EHI
	      if (delta==0)
		rejected++;
	      tempIterCount++;
      delete neighbourSolution;
    }
    else
      {
	rejected++;
	tempIterCount++;
	delete neighbourSolution;
      }

  }


  //FEASIBLE REGION LOOP --------------------------------------------------
  rejected=0;
  countAlfa=0;
  tempIterCount=0;
  Solution *bestSolution = new Solution( data,rg );
  bestSolution->copy(this);
#ifdef DEBUG
  bestHcv = computeHcv();
#endif

computeScv();

  Temp = iniTempScv;


  currentTotScv = computeScv();
  int bestScv = currentTotScv;
  int currentScv;

  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  //              MAIN LOOP
  //---------------------------------------------------------------------
  //---------------------------------------------------------------------
  //---------------------------------------------------------------------

  double printTime = 0.0;
  while (timer.elapsedTime(Timer::VIRTUAL) < time_limit) {

    double currentTime = timer.elapsedTime(Timer::VIRTUAL);
    if (currentTime - printTime > TIMEINTERVALPRINT) {
      printTime = currentTime;
      std::cout << "time left: " << time_limit - currentTime << std::endl;
    }
    //std::cout << "event " << eventList[i] << " cost " << currentScv<<std::endl;

#ifdef DEBUG
    if (stepCount>MAXITER)
      {
	time_limit=0;
	break;
      }
    (*tempFile) << Temp << "  " << stepCount <<"\n";
    (*currHcvFile) << computeHcv() << "   0"  << "  " <<stepCount<<"  "<<delta<<"\n";
    (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
    //  if (tempIterCount>0)
    //(*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

    if (countAlfa>nrep)
      {
	Temp = alfa * Temp;


	countAlfa=0;
      }

#ifdef DEBUG
	if (tempIterCount>=(3*nrep))
	  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

	if (tempIterCount>=(3*nrep) && ((double)(rejected)/tempIterCount)>=rejectedLimScv)
	  {


	    Temp = Temp + iniTempScv;

#ifdef SOFTDEBUG
	    highTemp++;
#endif
	    rejected=0;
	    tempIterCount=0;
	  }



    Solution *neighbourSolution = new Solution( data,rg );
    do{
      neighbourSolution->copy( this );
      stepCount++; countAlfa++;
      moveType = (int)(rg->next()*2) + 1;
      e1 = (int)(rg->next()*(data->n_of_events));
      if(moveType == 1){  // perform move of type 1
	int t = (int)(rg->next()*45);
	neighbourSolution->Move1( e1, t);
	//std::cout<< "event " << e1 << " in timeslot " << t << std::endl;
	 neighbourHcv =  neighbourSolution->eventAffectedHcv(e1);
	 if (neighbourHcv==0)
	   {
	     neighbourScv = neighbourSolution->eventScv(e1)  // respectively Scv involving event e
	       + singleClassesScv(e1) // + single classes introduced in day of original timeslot
	       - neighbourSolution->singleClassesScv(e1);
	     currentScv = eventScv(e1);
	     break;
	   }
          //neighbourScv = neighbourSolution->computeScv();
	     // std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
      }
      else if(moveType == 2){ // perform move of type 2
	int e2 = (int)(rg->next()*(data->n_of_events));
	while(e2 == e1) // take care of not swapping one event with itself
	  e2 = (int)(rg->next()*(data->n_of_events));
	neighbourSolution->Move2( e1, e2);
	neighbourHcv = neighbourSolution->eventAffectedHcv(e1)
	  + neighbourSolution->eventAffectedHcv(e2);
	if (neighbourHcv==0)
	  {
	    neighbourScv =  neighbourSolution->eventScv(e1) + singleClassesScv(e1)
	      - neighbourSolution->singleClassesScv(e1)
	      + neighbourSolution->eventScv(e2)
	      + singleClassesScv(e2) - neighbourSolution->singleClassesScv(e2);
	    // std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
	    currentScv=eventScv(e1)+eventScv(e2);
	    break;
	  }
      }
      // std::cout << "e1 "<< e1 << " e2 " << e2 << std::endl;
      //count possible hcv introduced
      //neighbourHcv = neighbourSolution->computeHcv();
    } while (true);//neighbourHcv!=0);
  	delta=neighbourScv - currentScv;
      if( delta<0 ) {
      copy( neighbourSolution );
      delete neighbourSolution;
      //currentScv = computeScv();
      currentTotScv = currentTotScv+delta;
      if (currentTotScv < bestScv)
	{
	  bestSolution->copy(this);
	  bestScv = currentTotScv;
#ifdef METAOUTPUT
	  time=timer.elapsedTime(Timer::VIRTUAL);
	  (*mos) << "best " << bestScv << " time ";
			mos->flags( std::ios::fixed );
			(*mos) << ( time < 0 ? 0.0 : time ) << std::endl;
#endif
	}
      tempIterCount++;
    }
    else if (SAcriteria(delta,Temp)){
      copy( neighbourSolution );
      currentTotScv = currentTotScv+delta;
      //EHI
      if (delta==0)
	rejected++;
      tempIterCount++;
      delete neighbourSolution;
    }
    else
      {
	rejected++;
	tempIterCount++;
	delete neighbourSolution;
      }

  }
#ifdef DEBUG
  (*tempFile) << Temp << "  " << stepCount <<"\n";
  (*currHcvFile) << computeHcv() << "  " << bestHcv << "  " <<stepCount<<"  "<<delta<<"\n";
  (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
#endif
  copy(bestSolution);
#ifdef SOFTDEBUG
  //std::cout<<"Best: " <<computeScv() << " iterations: " <<iter<<"\n";
  std::cout <<computeHcv()<<" "<<computeScv() << " " << stepCount << " " << highTemp <<std::endl;
#endif
}


inline bool Solution::SAcriteria(int delta, double Temp)
{
  return (rg->next() < (exp((double)(-delta)/Temp)));
}

/*
void Solution::simulatedAnnealing_Random(double time_limit, double alfa, int nrep,
					 double iniTempFactorHcv, double iniTempFactorScv,
					 double rejectedLimHcv, double rejectedLimScv,
					 double prob1 = 1.0, double prob2 = 1.0, double prob3 = 0.0)
{
  // perform local search with given time limit and probabilities for each type of move
  int moveType,e1,e2;
  int delta=0;
  int neighbourHcv = 0; // partial evaluation of neighbour solution hcv
  int neighbourScv = 0; // partial evaluation of neighbour solution scv
  int rejected=0;
  int tempIterCount=0;

  double Temp;
  int stepCount=0;
  computeFeasibility();
  int currentHcv=computeHcv();
  int currentScv=computeScv();
#ifdef METAOUTPUT
  std::ostream *mos;
  //  //  if ()
  //  //      *mos = new std::ofstream( getStringParameter( "-o" ).c_str() );
  //  //    else {
  std::cerr << "Warning: No output file given for new best solution and CPU time, writing to stderr" << std::endl;
  mos = &std::cerr;
  //}
  double time;
  int bestHcv=currentHcv;
#endif

  timer.resetTime(); // reset time counter for the local search

  //int bestScv=currentScv;
  //Setting the temperature dpending on the distance from random sampling
  //--------------------------------------------------
  int recordHcv=0;
  int recordScv=0;
  int numSamp=100;
  for (int samp=0; samp<numSamp; samp++)
    {
      Solution *neighbourSolution = new Solution( data,rg );
      neighbourSolution->copy(this);
      neighbourSolution->randomMove();
      recordHcv+=abs(neighbourSolution->computeHcv()-currentHcv);
      recordScv+=abs(neighbourSolution->computeScv()-currentScv);
      delete neighbourSolution;
    }
  double iniTempHcv = iniTempFactorHcv *(double)recordHcv/numSamp;
  double iniTempScv = iniTempFactorScv *(double)recordScv/numSamp;
#ifdef DEBUG
  std::cout<<"Initialization of temperature for Hcv: "<<iniTempHcv<<"\n";
  std::cout<<"Initialization of temperature for Scv: "<<iniTempScv<<"\n";

  std::cout << "Max num. of rejected moves in Hcv loop acceptable: "<<rejectedLimHcv<<std::endl;
  std::cout << "Max num. of rejected moves in Scv loop acceptable: "<<rejectedLimScv<<std::endl;
  std::cout<<"--------------------------------------------------\n";
  std::cout<<"Simulated Annealing Random...."<<std::endl;
  //--------------------------------------------------


  std::cout<<"DEBUG ACTIVE\n";
  std::ofstream *tempFile = new std::ofstream("sar/temp.dat");
  std::ofstream *currHcvFile = new std::ofstream("sar/currHcv.dat");
  std::ofstream *currScvFile = new std::ofstream("sar/currScv.dat");
  std::ofstream *ratioFile = new std::ofstream("sar/ratio.dat");
  int MAXITER=45000;

#endif

#ifdef SOFTDEBUG

  int highTemp=0;
  std::cout<<"SOFTDEBUG ACTIVE\n";
#endif

  Temp = iniTempHcv;
  timer.resetTime(); // reset time counter for the local search

  //UNFEASIBLE REGION LOOP --------------------------------------------------

  // if the timetable is not feasible try to solve hcv

  while(!feasible
	&& timer.elapsedTime(Timer::VIRTUAL) < time_limit){


#ifdef DEBUG
    if (stepCount>MAXITER)
      {
	time_limit=0;
	break;
      }
    (*tempFile) << Temp << "  " << stepCount <<"\n";
    (*currHcvFile) << computeHcv() << "  0" <<  "  " <<stepCount<<"  "<<delta<<"\n";
    (*currScvFile) << computeScv() << "  0" << "  " <<stepCount<<"  "<<delta<<"\n";
    //if (tempIterCount>0)
    //  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif
    if (tempIterCount>nrep)
      {
	Temp = alfa * Temp;
#ifdef DEBUG
	if (tempIterCount>0)
	  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif
	if (((double)(rejected)/tempIterCount)>rejectedLimHcv)
	  {

	    Temp = Temp + iniTempHcv;
#ifdef SOFTDEBUG
	    highTemp++;
#endif
	  }
	rejected=0;
	tempIterCount=0;
      }

    currentHcv=computeHcv();
    Solution *neighbourSolution = new Solution( data,rg );
    neighbourSolution->copy( this );
    //std::cout<< "event " << eventList[i] << " timeslot " << t << std::endl;
    stepCount++; tempIterCount++;
    moveType = (int)(rg->next()*2) + 1;
    e1 = (int)(rg->next()*(data->n_of_events));
    if(moveType == 1){  // perform move of type 1
      int t = (int)(rg->next()*45);
      neighbourSolution->Move1( e1, t);
      //std::cout<< "event " << e1 << " in timeslot " << t << std::endl;
    }
    else if(moveType == 2){ // perform move of type 2
      e2 = (int)(rg->next()*(data->n_of_events));
      while(e2 == e1) // take care of not swapping one event with itself
	e2 = (int)(rg->next()*(data->n_of_events));
      neighbourSolution->Move2( e1, e2);
      // std::cout << "e1 "<< e1 << " e2 " << e2 << std::endl;
    }

    neighbourHcv = neighbourSolution->computeHcv();
    delta = neighbourHcv-currentHcv;
    if( delta<0 ){
      copy( neighbourSolution );
      delete neighbourSolution;
      currentHcv = computeHcv();
#ifdef METAOUTPUT
		  if (currentHcv<bestHcv)
		    {
		      bestHcv=currentHcv;
		      time=timer.elapsedTime(Timer::VIRTUAL);
		      (*mos)<<bestHcv<<" "<<"- "<< ( time < 0 ? 0.0 : time ) << std::endl;
		    }
#endif
      if (currentHcv == 0)
	{
	  feasible=true;
	}
    }
    else if (SAcriteria(delta,Temp)){
      copy( neighbourSolution );
          //EHI
	      if (delta==0)
		rejected++;
      delete neighbourSolution;
    }
    else
      {
	rejected++;
	delete neighbourSolution;
      }

  }


  //FEASIBLE REGION LOOP --------------------------------------------------
  Solution *bestSolution = new Solution( data,rg );
  bestSolution->copy(this);
#ifdef DEBUG
  bestHcv = computeHcv();
#endif

computeScv();

  Temp = iniTempScv;
  tempIterCount=0;


  currentScv = computeScv();
  int bestScv = currentScv;

  while (timer.elapsedTime(Timer::VIRTUAL) < time_limit) {

    //std::cout << "event " << eventList[i] << " cost " << currentScv<<std::endl;

#ifdef DEBUG
    if (stepCount>MAXITER)
      {
	time_limit=0;
	break;
      }
    (*tempFile) << Temp << "  " << stepCount <<"\n";
    (*currHcvFile) << computeHcv() << "   0"  << "  " <<stepCount<<"  "<<delta<<"\n";
    (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"  "<<delta<<"\n";
    //  if (tempIterCount>0)
    //(*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

    if (tempIterCount>nrep)
      {
	Temp = alfa * Temp;
#ifdef DEBUG
	if (tempIterCount>0)
	  (*ratioFile) <<"0 0 "<<(double)(rejected)/tempIterCount<<" "<< stepCount << std::endl;
#endif

	if (((double)(rejected)/tempIterCount)>rejectedLimScv)
	  {
	    Temp = Temp + iniTempScv;

#ifdef SOFTDEBUG
	    highTemp++;
#endif

	  }
	rejected=0;
	tempIterCount=0;
      }


    Solution *neighbourSolution = new Solution( data,rg );
    do{
      neighbourSolution->copy( this );
      stepCount++; tempIterCount++;
      moveType = (int)(rg->next()*2) + 1;
      e1 = (int)(rg->next()*(data->n_of_events));
      if(moveType == 1){  // perform move of type 1
	int t = (int)(rg->next()*45);
	neighbourSolution->Move1( e1, t);
	//std::cout<< "event " << e1 << " in timeslot " << t << std::endl;
      }
      else if(moveType == 2){ // perform move of type 2
	e2 = (int)(rg->next()*(data->n_of_events));
	while(e2 == e1) // take care of not swapping one event with itself
	  e2 = (int)(rg->next()*(data->n_of_events));
	neighbourSolution->Move2( e1, e2);
      }
      // std::cout << "e1 "<< e1 << " e2 " << e2 << std::endl;
      //count possible hcv introduced
      neighbourHcv = neighbourSolution->computeHcv();
    } while (neighbourHcv!=0);
    neighbourScv = neighbourSolution->computeScv();
    // std::cout<< "neighbour cost " << neighbourScv<<" " << neighbourHcv<< std::endl;
    delta = neighbourScv - currentScv;
    if( delta<0 ) {
      copy( neighbourSolution );
      delete neighbourSolution;
      currentScv = computeScv();
      if (currentScv < bestScv)
	{
	  bestSolution->copy(this);
	  bestScv = currentScv;
#ifdef METAOUTPUT
	  time=timer.elapsedTime(Timer::VIRTUAL);
	  (*mos)<<bestHcv<<" "<<bestScv<<" "<< ( time < 0 ? 0.0 : time ) << std::endl;
#endif
	}
    }
    else if (SAcriteria(delta,Temp)){
      copy( neighbourSolution );
      //EHI
      if (delta==0)
	rejected++;
      delete neighbourSolution;
    }
    else
      {
	rejected++;
	delete neighbourSolution;
      }

  }
#ifdef DEBUG
  (*tempFile) << Temp << "  " << stepCount <<"\n";
  (*currHcvFile) << computeHcv() << "  " << bestHcv << "  " <<stepCount<<"\n";
  (*currScvFile) << computeScv() << "  " << bestScv << "  " <<stepCount<<"\n";
#endif
  copy(bestSolution);
#ifdef SOFTDEBUG
  //std::cout<<"Best: " <<computeScv() << " iterations: " <<iter<<"\n";
  std::cout <<computeHcv()<<" "<<computeScv() << " " << stepCount << " " << highTemp <<std::endl;
#endif
}
*/
