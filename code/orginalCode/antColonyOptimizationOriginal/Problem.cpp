#include "Problem.h"

Problem::Problem( std::istream& ifs ) {

  // read in a problem instance from file with extension file format .tim

  ifs >> n_of_events;
#ifdef DEBUG
  std::cout << "n_of_events: " << n_of_events << std::endl;
#endif
  ifs >> n_of_rooms;
#ifdef DEBUG
  std::cout << "n_of_rooms: " << n_of_rooms << std::endl;
#endif
  ifs >> n_of_features;
#ifdef DEBUG
  std::cout << "n_of_features: " << n_of_features << std::endl;
#endif
  ifs >> n_of_students;
#ifdef DEBUG
  std::cout << "n_of_students: " << n_of_students << std::endl;
#endif

  // read room sizes
  int size;
  for( int i = 0; i < n_of_rooms; i++ ) {
    ifs >> size;
    roomSize.push_back(size);
#ifdef DEBUG
    std::cout << "Room " << i << ", size: " << roomSize[i];
#endif
  }
#ifdef DEBUG
  std::cout << std::endl;
#endif

  // read student attendance and keep it in a matrix
  student_events = IntMatrixAlloc(n_of_students,n_of_events);
  for (int i = 0; i < n_of_students; i++) {
#ifdef DEBUG
    std::cout << "Student " << i << " follows the events:" << std::endl;
#endif
    for (int j = 0; j < n_of_events; j++) {
      ifs >> student_events[i][j];
#ifdef DEBUG
      if (student_events[i][j]){
	std::cout << j << " ";
      }
#endif
    }
#ifdef DEBUG
    std::cout << std::endl;
#endif
  }
#ifdef DEBUG
  std::cout << std::endl;
#endif

  // calculate the number of students for each event and store it in the studentNumber vector
  for (int i = 0; i < n_of_events; i++) {
    int sum = 0;
#ifdef DEBUG
    std::cout << "Event " << i << " is followed by the students:" << std::endl;
#endif
    for (int j = 0; j < n_of_students; j++) {
      sum = sum + student_events[j][i];
#ifdef DEBUG
      if (student_events[j][i]){
	std::cout << j << " ";
      }
#endif
    }
#ifdef DEBUG
    std::cout << std::endl;
#endif
    studentNumber.push_back( sum);
  }
#ifdef DEBUG
  std::cout << std::endl;

  for (int i = 0; i < n_of_events; i++) {
    std::cout << "Event " << i << ", nr_of_stud: " << studentNumber[i] << std::endl;
  }
#endif

  // calculate event correlations in terms of students in common and store them in the eventCorrelations matrix
  eventCorrelations = IntMatrixAlloc(n_of_events,n_of_events);
  for (int i = 0; i < n_of_events; i++) {
    for (int j = 0; j < n_of_events; j++) {
      eventCorrelations[i][j] = 0;
    }
  }
  for (int i = 0; i < n_of_events; i++) {
    for (int j = 0; j < n_of_events; j++) {
      for (int k = 0; k < n_of_students; k++) {
	if ((student_events[k][i] == 1) && (student_events[k][j] == 1)) {
	   eventCorrelations[i][j] = 1;
	   break;
	}
      }
    }
  }

  // read features satisfied by each room and store them in the room_features matrix
  room_features = IntMatrixAlloc(n_of_rooms,n_of_features);
  for (int i = 0; i < n_of_rooms; i++) {
    for (int j = 0; j < n_of_features; j++) {
      ifs >> room_features[i][j];
    }
  }

  // read features required by each event and store them in the event_features matrix
  event_features = IntMatrixAlloc(n_of_events,n_of_features);
  for (int i = 0; i < n_of_events; i++) {
    for (int j = 0; j < n_of_features; j++) {
      ifs >> event_features[i][j];
    }
  }

#ifdef DEBUG
  for (int i = 0; i < n_of_rooms; i++) {
    std::cout << "Room " << i << " features " ;
    for (int j = 0; j < n_of_features; j++) {
      if( room_features[i][j] == 1)
	std::cout << j << " ";
    }
    std::cout << std::endl;
  }

  for (int i = 0; i < n_of_events; i++) {
    std::cout << "Event " << i << " features " ;
    for (int j = 0; j < n_of_features; j++) {
      if( event_features[i][j] == 1)
	std::cout << j << " ";
    }
    std::cout << std::endl;
  }
#endif

  // pre-process which rooms are suitable for each event
 possibleRooms = IntMatrixAlloc(n_of_events,n_of_rooms);
 for (int i = 0; i < n_of_events; i++) {
   for (int j = 0; j < n_of_rooms; j++) {
     possibleRooms[i][j] = 0;
   }
 }
 int k = 0;
 for (int i = 0; i < n_of_events; i++) {
   for (int j = 0; j < n_of_rooms; j++){
     if((roomSize[j] >= studentNumber[i])){
       for(k = 0; k < n_of_features; k++){
	 if(event_features[i][k] == 1 && room_features[j][k] == 0)
	   break;
       }
       if(k == n_of_features)
	 possibleRooms[i][j] = 1;
     }
   }
 }

#ifdef DEBUG
 for (int i = 0; i < n_of_events; i++) {
   std::cout << "Event " << i << " possible rooms " ;
   for (int j = 0; j < n_of_rooms; j++) {
     if( possibleRooms[i][j] == 1)
       std::cout << j << " ";
   }
   std::cout << std::endl;
 }
#endif
}

Problem::~Problem()
{
  free(eventCorrelations);
  free(room_features);
  free(event_features);
  free(possibleRooms);
}
