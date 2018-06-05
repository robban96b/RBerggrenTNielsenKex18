/* -*- mode: c++; mode: font-lock -*- */
#include "Control.h"
#include <string.h>

Control::Control( int argc, char** argv ) {

	// parse the command line options to set all vars

	if( ( argc % 2 == 0 ) || ( argc == 1 ) ) {
		std::cerr << "Parse error: Number of command line parameters incorrect\n";
		std::cerr << "Usage:" << std::endl;
		std::cerr << argv[ 0 ] << " -i InputFile [-o OutputFile] [-n NumberOfTries] [-s RandomSeed] [-t TimeLimit] [-p ProblemType]" << std::endl;
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
		std::cerr << "Error: No input file given, exiting" << std::endl;
		std::cerr << "Usage:" << std::endl;
		std::cerr << argv[ 0 ] << " -i InputFile [-o OutputFile] [-n NumberOfTries] [-s RandomSeed] [-t TimeLimit] [-p ProblemType]" << std::endl;
		exit(1);
	}

	// check for ouput parameter

	if( parameterExists( "-o" ) ) {
		os = new std::ofstream( getStringParameter( "-o" ).c_str() );
	} else {
		std::cerr << "Warning: No output file given, writing to stdout" << std::endl;
		os = &std::cout;
	}

	// check for number of tries parameter

	if( parameterExists( "-n" ) ) {
		maxTry = getIntParameter( "-n" );
		std::cout << "Max number of tries " << maxTry << std::endl;
	} else {
		std::cerr << "Warning: Number of tries is set to default (10)" << std::endl;
		maxTry = 10; // default number of tries
	}

	// check for time limit parameter

	if( parameterExists( "-t" ) ) {
		timeLimit = getDoubleParameter( "-t" );
		std::cout <<"Time limit " << timeLimit << std::endl;
	} else {
		std::cerr << "Warning: Time limit is set to default (90 sec)" << std::endl;
		timeLimit = 90; // default time limit
	}

        // check for problem instance type parameter for the local search

	if( parameterExists( "-p" ) ) {
		problemType = getIntParameter( "-p" );
		std::cout <<"Problem instance type " << problemType << std::endl;
	} else {
	   std::cerr << "WARNING: The problem instance type flag is missing. This input data is necessary for setting the right configuration!" << std::endl;
	   exit(EXIT_FAILURE);
	   //std::cerr << "Warning: The problem instance type is set by default to 1 (easy)" << std::endl;
		problemType = 1; // default problem type
	}


        // check for maximum steps parameter for the local search

	if( parameterExists( "-m" ) ) {
		maxSteps = getIntParameter( "-m" );
		std::cout <<"Max number of steps in the local search " << maxSteps << std::endl;
	} else {
	  //std::cerr << "Warning: The maximum number of steps for the local search is set by default to 100" << std::endl;
		maxSteps = 100; // default max steps
	}

        // check for time limit parameter for the local search

	if( parameterExists( "-l" ) ) {
		LS_limit = getDoubleParameter( "-l" );
		std::cout <<"Local search time limit " << LS_limit << std::endl;
	} else {
		std::cerr << "Warning: The local search time limit is set to default (99999 sec)" << std::endl;
		LS_limit = 99999; // default local search time limit
	}

        // check for probability parameter for each move in the local search

	if( parameterExists( "-p1" ) ) {
		prob1 = getDoubleParameter( "-p1" );
		std::cout << "LS move 1 probability " << prob1 <<std::endl;
	} else {
		std::cerr << "Warning: The local search move 1 probability is set to default 1.0" << std::endl;
		prob1 = 1.0; // default local search probability for each move of type 1 to be performed
	}

	if( parameterExists( "-p2" ) ) {
		prob2 = getDoubleParameter( "-p2" );
		std::cout <<"LS move 2 probability " << prob2 << std::endl;
	} else {
		std::cerr << "Warning: The local search move 2 probability is set to default 1.0" << std::endl;
		prob2 = 1.0; // default local search probability for each move to be performed
	}

	if( parameterExists( "-p3" ) ) {
		prob3 = getDoubleParameter( "-p3" );
		std::cout <<"LS move 3 probability " << prob3 <<  std::endl;
	} else {
		std::cerr << "Warning: The local search move 3 probability is set to default 0.0" << std::endl;
		prob3 = 0.0; // default local search probability for each move to be performed
	}

	// check for random seed

	if( parameterExists( "-s" ) ) {
		seed = getIntParameter( "-s" );
		srand( seed );
	} else {
		seed = time( NULL );
		std::cerr << "Warning: " << seed << " used as default random seed" << std::endl;
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
  if(bestSolution->feasible){
    (*os) << "feasible: evaluation function = " << bestSolution->scv <<std::endl;
    for(int i = 0; i < (*bestSolution).data->n_of_events; i++)
      (*os) << bestSolution->sln[i].first << " " ;
    (*os) << std::endl;
    for(int i = 0; i < (*bestSolution).data->n_of_events; i++)
      (*os) << bestSolution->sln[i].second << " " ;
    (*os) << std::endl;
  }
  else{
    (*os) << "unfeasible: evaluation function = " << (bestSolution->computeHcv() * 1000000) + bestSolution->computeScv() <<std::endl;
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
	  if( currentSolution->feasible && currentScv < bestScv ) {
	    bestScv = currentScv;
	    bestEvaluation = currentScv;
	    double time = getTime();
	    (*os) << "best " << bestScv << " time ";
			os->flags( std::ios::fixed );
			(*os) << ( time < 0 ? 0.0 : time ) << std::endl;
	  }
	  else if(!currentSolution->feasible){
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

void Control::setSAParameters(const int neighbourhoodSize, int &method, double &rateNeighbourhood, double &alfa, int &nrep, double &iniTempFactorHcv, double &iniTempFactorScv, double &rejectedLimHcv, double &rejectedLimScv)
{

 //------------------------------------------------------------
  //MAURO, cambia il valore di config qui sotto!

  if (problemType==1)
    CONFIG = 47;  //<-- EASY INSTANCE
  else if (problemType==2)
    CONFIG = 50;  //<-- MEDIUM INSTANCE
  else if (problemType==3)
    CONFIG = 50; //<-- HARD INSTANCE
  else
    {
      std::cerr<<"Errors occuring in setting the right configuration. -p flag must range from 1 (easy instance) to 3 (hard instance)"<<std::endl;
      exit(EXIT_FAILURE);
    }
  //------------------------------------------------------------


//Setting of Simulated Annealing Parameters
//--------------------------------------------------

  div_t r;
  int alfaInd,TempHcvInd,TempScvInd,rateNeighInd;

  //config=control.getConfiguration();

  //---------- SIMULATED ANNEALING IN LOCAL SEARCH -------------------------------------
  if (CONFIG>0 && CONFIG<=36)
    {
      method=1;
      int val=CONFIG;
      r = div(val,2);
      rateNeighInd = r.rem + 1;
      switch (rateNeighInd) {
      case 1:
	rateNeighbourhood = 0.1;
	break;
      case 2:
	rateNeighbourhood = 0.2;
	break;
      default:
	std::cerr<<"Error in neigh. configuration!"<<std::endl;
      }

      r = div(val,3);
      alfaInd = r.rem + 1;
      switch (alfaInd) {
      case 1:
	alfa = 0.8;
	break;
      case 2:
	alfa = 0.9;
	break;
      case 3:
	alfa = 0.97;
	break;
      default:
	std::cerr<<"Error in alfa configuration!"<<std::endl;
      }

      r = div(r.quot,2);
      TempHcvInd = r.rem + 1;
      switch (TempHcvInd) {
      case 1:
	iniTempFactorHcv = 0.1;
	break;
      case 2:
  	iniTempFactorHcv = 0.2;
  	break;
      default:
  	std::cerr<<"Error in TempHcvInd configuration!"<<std::endl;
      }

      r = div(r.quot,3);
      TempScvInd = r.rem + 1;
      switch (TempScvInd) {
      case 1:
	iniTempFactorScv = 0.3;
	break;
      case 2:
	iniTempFactorScv = 0.62;
	break;
      case 3:
	iniTempFactorScv = 0.7;
	break;
      default:
	std::cerr<<"Error in TempScvInd configuration!"<<std::endl;
      }

      rejectedLimScv = 0.97;
      nrep = (int)(rateNeighbourhood * neighbourhoodSize);
      rejectedLimHcv = 0.98;//(int)(6.0e-05 * neighbourhoodSize);
    }
  //---------- SIMULATED ANNEALING WITH RANDOM MOVES ------------------------------

  else if (CONFIG>36 && CONFIG<=46)
    {
      method=2;
      int val=CONFIG-36;

      r = div(val,5);
      TempScvInd = r.rem + 1;
      switch (TempScvInd) {
      case 1:
	iniTempFactorScv = 0.05;
	break;
      case 2:
	iniTempFactorScv = 0.1;
	break;
      case 3:
	iniTempFactorScv = 0.15;
	break;
      case 4:
	iniTempFactorScv = 0.2;
	break;
      case 5:
	iniTempFactorScv = 0.3;
	break;
      default:
	std::cerr<<"Error in TempScvInd configuration!"<<std::endl;
      }

      r = div(r.quot,2);
      TempHcvInd = r.rem + 1;
      switch (TempHcvInd) {
      case 1:
	iniTempFactorHcv = 0.1;
	break;
      case 2:
	iniTempFactorHcv = 0.2;
	break;
      default:
	std::cerr<<"Error in TempHcvInd configuration!"<<std::endl;
      }

     alfa=0;//cooling rate
     rateNeighbourhood=0;
     nrep=0;
     rejectedLimHcv=0;
     rejectedLimScv=0;

    }

  //---------- SIMULATED ANNEALING WITH RANDOM MOVES ------------------------------

  else if (CONFIG>46 && CONFIG<=70)
    {
      method=3;
      int val=CONFIG-46;

      r = div(val,2);
      rateNeighInd = r.rem + 1;
      switch (rateNeighInd) {
      case 1:
	rateNeighbourhood = 0.1;
	break;
      case 2:
	rateNeighbourhood = 0.2;
      	break;
      default:
	std::cerr<<"Error in alfa configuration!"<<std::endl;
      }

      r = div(val,3);
      alfaInd = r.rem + 1;
      switch (alfaInd) {
      case 1:
	alfa = 0.8;
	break;
      case 2:
	alfa = 0.9;
	break;
      case 3:
	alfa = 0.95;
	break;
      default:
	std::cerr<<"Error in alfa configuration!"<<std::endl;
      }


      r = div(r.quot,2);
      TempHcvInd = r.rem + 1;
      switch (TempHcvInd) {
      case 1:
	iniTempFactorHcv = 0.1;
	break;
      case 2:
	iniTempFactorHcv = 0.2;
	break;
      default:
	std::cerr<<"Error in TempHcvInd configuration!"<<std::endl;
      }


      r = div(r.quot,2);
      TempScvInd = r.rem + 1;
      switch (TempScvInd) {
      case 1:
	iniTempFactorScv = 0.3;
	break;
      case 2:
	iniTempFactorScv = 0.62;
	break;
      default:
	std::cerr<<"Error in TempScvInd configuration!"<<std::endl;
      }


      rejectedLimScv = 0.97;
      nrep = (int)(rateNeighbourhood * neighbourhoodSize);
      rejectedLimHcv = 0.98;//(int)(6.0e-05 * neighbourhoodSize);
    }
  else
    std::cerr<<"Errors in configuration"<<std::endl;
}


void Control::informationsReport(int method, double rateNeighbourhood, double alfa, double iniTempFactorHcv, double iniTempFactorScv, double rejectedLimHcv, double rejectedLimScv, double initialization)
{
  std::ifstream info;
  char *line;
  char *trailer;
  /*
  (*os) << "Current Machine: \n";

  info.open("/proc/cpuinfo", std::ios::in);
  if (info.is_open())
   {
   while (!info.eof())
    {
      do
	{
	  line = readline(info);
	  if (info.eof())
	    break;
	} while ((line==NULL));
      if (line==NULL)
	break;
      else if ((trailer = match (line, "processor")))
	(*os) <<line<<"\n";
      else if ((trailer = match (line, "vendor_id")))
	(*os) << line<<"\n";
      else if ((trailer = match (line, "model name")))
	(*os) << line<<"\n";
      else if ((trailer = match (line, "cpu MHz")))
	(*os) << line<<"\n";
      else if ((trailer = match (line, "cache size")))
	(*os) << line<<"\n";
      else if ((trailer = match (line, "bogomips")))
	(*os) << line<<"\n";
      else if ((trailer = match (line, "EOF"))) break;
      else if ((trailer = match (line, " "))) {}
      else
	continue;
    }
   info.close();
   }
  else {
    (*os)<<"Couldn`t find file /proc/cpuinfo for current machine type data\n";
  }
   (*os) <<"\n";
   (*os) << "Compiler g++ V2.95.2 Flags "; //-Wall -ansi -O3\n";
   //(*os)<< exec("g++ -v");

     info.open("Makefile", std::ios::in);
  if (info.is_open())
   {
   while (!info.eof())
    {
      do
	{
	  line = readline(info);
	  if (info.eof())
	    break;
	} while ((line==NULL));
      if (line==NULL)
	break;
      else if ((trailer = match (line, "CXXFLAGS =")))
	(*os) <<trailer<<"\n";
      else if ((trailer = match (line, "EOF"))) break;
      else if ((trailer = match (line, " "))) {}
      else
	continue;
    }
   info.close();
   }
  else {
    (*os)<<"Couldn`t find Makefile \n";
  }
  */
    (*os) <<"\n";
    (*os) << "OS Suse Linux 7.1\n";
   (*os) <<"\n";
   (*os) << "Algorithm Simulated Annealing for Timetabling, V2.2.6\n";
   (*os) <<"\n";
   (*os) << "Parameter-settings" << std::endl;
   (*os) <<"\n";
   (*os) <<"problemType: "<<problemType<<"\n";
   (*os) <<"maxTry: "<<maxTry<<"\n";
   (*os) <<"timeLimit: "<<timeLimit<<"\n";
   (*os) <<"prob1: "<<prob1<<"\n";
   (*os) <<"prob2: "<<prob2<<"\n";
   (*os) <<"prob3: "<<prob3<<"\n";
   (*os) <<"seed: "<<seed<<"\n";
   (*os) <<"method: "<<method <<"\n";
   (*os) <<"rateNeighbourhood: "<<rateNeighbourhood <<"\n";
   (*os) <<"alfa: " << alfa <<"\n";
   (*os) <<"iniTempFactorHcv: " << iniTempFactorHcv <<"\n";
   (*os) <<"iniTempFactorScv: " << iniTempFactorScv <<"\n";
   (*os) <<"rejectedLimHcv: "<<rejectedLimHcv<<"\n";
   (*os) <<"rejectedLimScv: "<<rejectedLimScv <<"\n";
   (*os) <<"\n";
   (*os) <<"Initialization Time: "<<( initialization < 0 ? 0.0 : initialization )<< "\n";
   (*os) <<"\n";
   (*os) <<"Begin problem "<<getStringParameter( "-i" )<<"\n";
   (*os)<<std::endl;

}

char * Control::readline (std::istream &infile) {
  size_t length;
  size_t i;
  //char *result = f
  //std::cout << "entra/n";
  if (infile.getline(buffer,250))
    {
      length = strlen (buffer);
      //if (length > 0 && buffer[length-1] != 0)
      //std::cerr<<"Buffer size in getline() isn't large enough\n"<<buffer;
      //std::cout << buffer << std::endl;
      for (i=0; i<length; ++i)
	if (buffer[i]!=' ' || buffer[i]!='\t') return buffer;
    }
  else
    return 0;
  return readline(infile);
}


char * Control::match (char *source, char *prefix) {

  size_t prefixLength = strlen (prefix);

  if (strncmp(source, prefix, prefixLength) != 0) return 0;
  source += prefixLength;
  while ((*source)==' ' || (*source)=='\t' ) source++;
  if (*source == ':' || *source == '=') source++;
  while ((*source)==' ' || (*source)=='\t' ) source++;

  return source;
}
