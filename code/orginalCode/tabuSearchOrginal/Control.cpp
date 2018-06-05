#include "Control.h"

Control::Control( int argc, char** argv ) {

	// parse the command line options to set all vars

	if( ( argc % 2 == 0 ) || ( argc == 1 ) ) {
		std::cout << "Parse error: Number of command line parameters incorrect\n";
		std::cout << "Usage:" << std::endl;
		std::cout << argv[ 0 ] << " -i InputFile [-o OutputFile] [-n NumberOfTries] [-s RandomSeed] [-t TimeLimit] [-p ProblemType]" << std::endl;
		exit(1);
	}

	for( int i = 1; i < argc / 2 + 1; i++ ) {
		parameters[ argv[ i * 2 - 1 ] ] = argv[ i * 2 ];
	}

	nrTry = 0;

	// check for input parameter

	if( parameterExists( "-i") ) {
		is = new std::ifstream( getStringParameter( "-i" ).c_str() );
	} else {
		std::cout << "Error: No input file given, exiting" << std::endl;
		std::cout << "Usage:" << std::endl;
		std::cout << argv[ 0 ] << " -i InputFile [-o OutputFile] [-n NumberOfTries] [-s RandomSeed] [-t TimeLimit] [-p ProblemType]" << std::endl;
		exit(1);
	}

	// check for ouput parameter

	if( parameterExists( "-o" ) ) {
		os = new std::ofstream( getStringParameter( "-o" ).c_str() );
	} else {
		std::cout << "Warning: No output file given, writing to stdout" << std::endl;
		os = &std::cout;
	}

	// check for number of tries parameter

	if( parameterExists( "-n" ) ) {
		maxTry = getIntParameter( "-n" );
		std::cout << "Max number of tries " << maxTry << std::endl;
	} else {
		std::cout << "Warning: Number of tries is set to default (10)" << std::endl;
		maxTry = 10; // default number of tries
	}

	// check for time limit parameter

	if( parameterExists( "-t" ) ) {
		timeLimit = getDoubleParameter( "-t" );
		std::cout <<"Time limit " << timeLimit << std::endl;
	} else {
		std::cout << "Warning: Time limit is set to default (90 sec)" << std::endl;
		timeLimit = 90; // default time limit
	}

        // check for problem instance type parameter for the local search

	if( parameterExists( "-p" ) ) {
		problemType = getIntParameter( "-p" );
		std::cout <<"Problem instance type " << problemType << std::endl;
	} else {
	  //std::cout << "Warning: The problem instance type is set by default to 1 (easy)" << std::endl;
		problemType = 1; // default problem type
	}


        // check for maximum steps parameter for the local search

	if( parameterExists( "-m" ) ) {
		maxSteps = getIntParameter( "-m" );
		std::cout <<"Max number of steps in the local search " << maxSteps << std::endl;
	} else {
	  //std::cout << "Warning: The maximum number of steps for the local search is set by default to 100" << std::endl;
		maxSteps = 100; // default max steps
	}

        // check for time limit parameter for the local search

	if( parameterExists( "-l" ) ) {
		LS_limit = getDoubleParameter( "-l" );
		std::cout <<"Local search time limit " << LS_limit << std::endl;
	} else {
		std::cout << "Warning: The local search time limit is set to default (99999 sec)" << std::endl;
		LS_limit = 99999; // default local search time limit
	}

        // check for probability parameter for each move in the local search

	if( parameterExists( "-p1" ) ) {
		prob1 = getDoubleParameter( "-p1" );
		std::cout << "LS move 1 probability " << prob1 <<std::cout;
	} else {
     	prob1 = 0.1; // default local search probability for each move of type 1 to be performed
		std::cout << "Warning: The local search move 1 probability is set to default " << prob1 <<std::cout;
	}

	if( parameterExists( "-p2" ) ) {
		prob2 = getDoubleParameter( "-p2" );
		std::cout <<"LS move 2 probability " << prob2 << std::endl;
	} else {
		prob2 = 0.1; // default local search probability for each move to be performed
		std::cout << "Warning: The local search move 2 probability is set to default " << prob2<< std::endl;
	}


	if( parameterExists( "-alfa" ) ) {
		alfa = getDoubleParameter( "-alfa" );
		std::cout << "Tabu list length factor " << alfa <<std::cout;
	} else {
		std::cout << "Warning: The tabu list length factor is set to default 0.01" << std::endl;
		alfa = 0.01; // default local search probability for each move of type 1 to be performed
	}


	if( parameterExists( "-p3" ) ) {
		prob3 = getDoubleParameter( "-p3" );
		std::cout <<"LS move 3 probability " << prob3 <<  std::cout;
	} else {
		std::cout << "Warning: The local search move 3 probability is set to default 0.0" << std::endl;
		prob3 = 0.0; // default local search probability for each move to be performed
	}

	// check for random seed

	if( parameterExists( "-s" ) ) {
		seed = getIntParameter( "-s" );
		srand( seed );
	} else {
		seed = time( NULL );
		std::cout << "Warning: " << seed << " used as default random seed" << std::endl;
		srand( seed );
	}
}

Control::~Control() {
}

bool
Control::parameterExists( std::string paramName ) {
	for( std::map< std::string, std::string >::iterator i = parameters.begin(); i != parameters.end(); i++ ) {
		if( i-> first == paramName )
			return true;
	}
	return false;
}

int
Control::getIntParameter( std::string paramName ) {
	if( parameterExists( paramName ) )
		return atoi( parameters[paramName].c_str() );
	else {
		return 0;
	}
}

double
Control::getDoubleParameter( std::string paramName ) {
	if( parameterExists( paramName ) )
		return atof( parameters[paramName].c_str() );
	else {
		return 0;
	}
}

std::string
Control::getStringParameter( std::string paramName ) {
	if( parameterExists( paramName ) )
		return parameters[paramName];
	else {
		return 0;
	}
}

void
Control::resetTime() {
	timer.resetTime();
}

double
Control::getTime() {
	return timer.elapsedTime( Timer::VIRTUAL );
}

void
Control::beginTry() {
	srand( seed++ );
	(*os) << "begin try " << ++nrTry << std::endl;
	resetTime();
	feasible = false;
	bestScv = INT_MAX;
	bestEvaluation = INT_MAX;
}

void
Control::endTry( Solution *bestSolution) {
  (*os) << "begin solution " << nrTry << std::endl;
  if(bestSolution->hcv == 0){
    (*os) << "feasible: evaluation function = " << bestSolution->scv <<std::cout;
    for(int i = 0; i < (*bestSolution).data->n_of_events; i++)
      (*os) << bestSolution->sln[i].first << " " ;
    (*os) << std::endl;
    for(int i = 0; i < (*bestSolution).data->n_of_events; i++)
      (*os) << bestSolution->sln[i].second << " " ;
    (*os) << std::endl;
  }
  else{
    (*os) << "unfeasible: evaluation function = " << (bestSolution->computeHcv() * 1000000) + bestSolution->computeScv() <<std::cout;
  }
  (*os) << "end solution " << nrTry << std::endl;
  (*os) << "end try " << nrTry << std::endl;

  // The following output might be used if the ./checksln program wants to be used to check the validity of solutions.
  // Remember that the output file has then to have the .sln extension and has to have the same name as the .tim instance file
  /*for(int i = 0; i < (*bestSolution).data->n_of_events; i++){
    (*os) << bestSolution->sln[i].first << " "<< bestSolution->sln[i].second;
    (*os) << std::endl;
    }*/
}

void
Control::setCurrentCost(Solution *currentSolution ) {
  //if( timeLeft() ) {
	  int currentScv = currentSolution->scv;
	  if( currentSolution->hcv==0 && currentScv < bestScv ) {
	    bestScv = currentScv;
	    bestEvaluation = currentScv;
	    double time = getTime();
	    (*os) << "best " << bestScv << " time ";
			os->flags( std::ios::fixed );
			(*os) << ( time < 0 ? 0.0 : time ) << std::endl;
	  }
	  else if(currentSolution->hcv!=0){
	    int currentEvaluation = (currentSolution->computeHcv() * 1000000) + currentSolution->computeScv();
	    if(currentEvaluation < bestEvaluation){
	      bestEvaluation = currentEvaluation;
	      double time = getTime();
	    (*os) << "best " << bestEvaluation << " time ";
			os->flags( std::ios::fixed );
			(*os) << ( time < 0 ? 0.0 : time ) << std::endl;
	    }
	  }
	  //}
}
