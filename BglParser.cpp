#include "BgLParser.h"


int main (int argc, char * argv[]){

 if (argc != 2)
    std::cout << "error" << std::endl;

 bGLParser(argv[1]);
  return 0;
}

void bGLParser( const char * file){


  char * str_ =NULL;
  char* activityToken =NULL;
  char* timestamp = NULL;


  double currentBglValue = 0;
  int roundedCurrentBGLValue =0;
  double previouseBglValue =999999;

  int lastMin = 9999999, lastMax, max, min;
  int lastMinBglTime, lastMaxBglTime;
  int flag =0; // 0 means min and 1 means max

  int minBglTimeSoFar;
  int maxBglTimeSofar;

  int nextThreeLinesCounter;
  bool lock = false;
  bool Print =true;

  std::vector<int> potentialBglBuffer ={0};
  std::vector<BGL> bglVector;
  std::vector<Activity> activityVector;
  std::vector<Event> eventsVector;
  std::vector<int> bglBuffer ={0};
  std::vector<double> bglTimeBuffer ={0};


// open two files: simulator log file and bgl.txt file.
// parse the simulator log file to create events.
// write to bgl.txt file the time and the measurments of bgls.

   std::ofstream outfile;
   outfile.open("bgl.txt");
   ifstream_.open(file);
   if(outfile.fail()){
	 std::cerr << " outfile file opening failed:\n";
	 exit(1);
 	}
  // parse the simulator log file:

  std::string line;
  int  minBglIndexFrom =0;
  int  maxBglIndexFrom =0;
  if (ifstream_.is_open()){
    while (getline (ifstream_,line)){
      str_ = new char [line.length() + 1];
      strcpy(str_,line.c_str());
      char* tok_ = strtok(str_," ");
      timestamp = new char[line.length()+1];
      strcpy(timestamp,tok_);
      int minutes = toMinutes(timestamp);
      tok_ = strtok(NULL," ");
      activityToken = new char[10];
      strcpy(activityToken,tok_);


      if (strcmp(activityToken,BGL_KEYWORD)== 0){
           tok_ = strtok(NULL," ");
           currentBglValue = atof(tok_);
           roundedCurrentBGLValue = int((currentBglValue + 5) / 10) * 10;
           bglBuffer.push_back(roundedCurrentBGLValue);
           bglTimeBuffer.push_back(minutes);
           if(bglBuffer.size()>=7){
                potentialBglBuffer.clear();
                for(int i =7; i>=1; i--) {
                    potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-i]);
                    //std :: cout << bglBuffer.size() << "_" << bglBuffer[bglBuffer.size()-i] << "_" << bglBuffer.size()-i << " ";
                }
            // std::cout << "\n";
            //  std:cout <<  potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-7]) <<"/n";
            //  std:cout << potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-6]);
            //  std:cout << potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-5]);
            //  std:cout << potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-4]);
            //  std:cout <<  potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-3]);
            //  std:cout <<  potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-2]);
            //  std:cout << potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-1]);


          //    if((previouseBglValue < roundedCurrentBGLValue) and !lock ){
           //
          //           potentialBglBuffer.clear();
          //           potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-5]);
          //           potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-4]);
          //           potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-3]);
          //           potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-2]);
           //
           //
          //           minBglTimeSoFar = minutes-1;
          //           lock = true;
          //           nextThreeLinesCounter = 0;
           //
          //  }
           //
          //   else if(previouseBglValue != 999999 && !lock && previouseBglValue > roundedCurrentBGLValue){
           //
          //     potentialBglBuffer.clear();
          //     potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-5]);
          //     potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-4]);
          //     potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-3]);
          //     potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-2]);
           //
          //     maxBglTimeSofar = minutes-1;
           //
          //     lock = true;
          //     nextThreeLinesCounter =0;
          //   }

            // if( lock ){
              // if(nextThreeLinesCounter >= 3 ){
                //  lock =false;
                  if(isMinimum(potentialBglBuffer)){
                      if(flag ==1){
                         // Confirmed last MAX
                         if(min == max -10 || min == lastMax -10){
                             //std::cout   << max << "_" << min << "_"<< lastMax <<"\n";
                             std::cout <<std::max(max,lastMax) <<"\n";
                             Print =false;

                          }
                          else
                          Print =true;

                          max = lastMax;
                          int index = findFirstOccurance(minBglIndexFrom, max, bglBuffer);
                          std:: cout << toTimeStamp(bglTimeBuffer[index]) << "  " << bglBuffer[index] << "  " << "Max1" <<"\n" ;
                          lastMin = potentialBglBuffer[3];
                          lastMinBglTime = bglTimeBuffer[bglTimeBuffer.size()-4];
                          minBglIndexFrom = bglBuffer.size() -4;
                          flag =0;


                          if(Print)
                          std:: cout << toTimeStamp(lastMaxBglTime)<<"  "<< max<<"  "<<"Max" <<"\n";


                       }else if (flag == 0){
                        // check if the current is less than prev min
                          if (lastMin > potentialBglBuffer[3]){
                              lastMin = potentialBglBuffer[3];
                              lastMinBglTime = bglTimeBuffer[bglTimeBuffer.size()-4];
                              minBglIndexFrom = bglBuffer.size() -4;
                            }
                        }
                  }else if(isMaximum(potentialBglBuffer)){
                      if(flag ==0 ){
                        // confirmed the last min

                         if(max == min +10 || max == lastMin +10){

                             //std::cout << min << "_"<< max << "_" << lastMin <<"\n";
                             std::cout <<std::min(min,lastMin) <<"\n";
                             Print =false;
                          }
                          else
                              Print =true;

                          min= lastMin;
                          int index = findFirstOccurance(maxBglIndexFrom, min, bglBuffer);
                          std:: cout << toTimeStamp(bglTimeBuffer[index]) << "  " << bglBuffer[index]<< " " <<"Min1" << "\n" ;
                          lastMax = potentialBglBuffer[3];
                          lastMaxBglTime= bglTimeBuffer[bglTimeBuffer.size()-4];
                          maxBglIndexFrom = bglBuffer.size()-4;
                          flag = 1;

                          if(Print)
                          std:: cout << toTimeStamp(lastMinBglTime) << "  " <<min << "  "<<"Min" <<"\n";
                          if (!isEmpty(activityVector)){
                                Event event(activityVector, activityVector[0].time);
                                //std::cout << event << std::endl;
                                //std::cout  << '\n';
                                activityVector.clear();
                          }
                      }else if (flag ==1){
                        // check if the current is greater than the prev max
                          if(lastMax < potentialBglBuffer[3]){
                              lastMaxBglTime= bglTimeBuffer[bglTimeBuffer.size()-4];
                              lastMax = potentialBglBuffer[3];
                              maxBglIndexFrom = bglBuffer.size()-4;
                            }
                        }
                  }//max
              // }else{
              //     potentialBglBuffer.push_back(roundedCurrentBGLValue);
              //     nextThreeLinesCounter++;
              //   }
          }

        //previouseBglValue = roundedCurrentBGLValue;
        outfile<< toTimeStamp(minutes);
        outfile << " " << roundedCurrentBGLValue << std::endl;
        BGL bgl;
        bgl.time = minutes;
        bgl.value = currentBglValue;
        bglVector.push_back(bgl);

      } else if (strcmp(activityToken,EXERCISE_KEYWORD)== 0){

          tok_ = strtok(NULL," ");
          tok_ = strtok(NULL," ");
          std::string descTemp = tok_ ;
          tok_ = strtok(NULL," ");
          tok_ = strtok(NULL," ");
          double duration = atof(tok_);
          tok_ = strtok(NULL," ");

        // Create an exercise activity:
          Activity activity;
          activity.desc = descTemp;
          activity.bgl = roundedCurrentBGLValue;
          activity.time = minutes;
          activity.howmuch = duration;
          activityVector.push_back(activity);

      } else if (strcmp(activityToken,DIET_KEYWORD)== 0){

          tok_ = strtok(NULL," ");
          double servingSizeTemp = atof(tok_);
          tok_ = strtok(NULL," ");
          tok_ = strtok(NULL," ");
          tok_ = strtok(NULL," ");
          std::string descTemp = tok_ ;
            // Create a food activity:
          Activity activity;
          activity.desc = descTemp;
          activity.bgl = roundedCurrentBGLValue;
          activity.time = minutes;
          activity.howmuch = servingSizeTemp;
          activityVector.push_back(activity);
      }

    }
    ifstream_.close();
    //baselBGL(bglVector);
  }
}

int toMinutes( char * timestamp){
  char * saveptr;

  char* tok = strtok_r(timestamp, ":", &saveptr);
  int day = (unsigned)atoi(tok); // day
  tok = strtok_r(NULL, ":",&saveptr);
  int hour = (unsigned)atoi(tok);
  tok = strtok_r(NULL, ":",&saveptr);
  int minutes = (unsigned)atoi(tok);
  int returnedValue = day* MINUTES_PER_DAY + hour* MINUTES_PER_HOUR + minutes;

  return returnedValue;
}

std::string toTimeStamp (int  time){
  double day = time / MINUTES_PER_DAY;
  double hour=  (time % MINUTES_PER_DAY)/MINUTES_PER_HOUR;
  double minutes = (time % MINUTES_PER_DAY)%MINUTES_PER_HOUR;
  std::string timeStamp = std::to_string((int)day) + ":" +std::to_string((int)hour)+ ":"+ std::to_string((int)minutes);
  return timeStamp;

}
void baselBGL(std::vector<BGL> & v){

  double minsofar = v[0].value;
  double minimum =10000;
  for ( int i =1; i < v.size()-1; i++){
      if (v[i].value < minsofar){
          minsofar =v[i].value;
          minimum = minsofar;
        }
  }
}


bool isEmpty(std::vector<Activity> &v){

    if (v.size() == 0)
       return true;
    else return false;
}

bool isMinimum(std::vector<int> &  bglBuffer){

// Goyal: In order to decide whether a BGL at time 'x' constitute a minimum: compare it against rounded BGLs at times
// 'x-3','x-2','x-1','x+1','x+2' and 'x+3'. BGL at time 'x' is a minimum only if
// a) if is less than or equal to BGLs at times 'x-3','x-2','x-1'
// AND b) if it is less than BGLs at times 'x+1','x+2' and 'x+3'.

if (bglBuffer[3] <= bglBuffer[0] && bglBuffer[3] <=  bglBuffer[1]&& bglBuffer[3] <=  bglBuffer[2])

    if( bglBuffer[3] < bglBuffer[4] && bglBuffer[3] <  bglBuffer[5]&& bglBuffer[3] <  bglBuffer[6]){

        //std::cout << " Trace " << bglBuffer[0] << bglBuffer[1] << bglBuffer[2] << bglBuffer[3]<< bglBuffer[4]<< bglBuffer[5]<< bglBuffer[6] << std::endl;
          return true;
    }


return false;

}

bool isMaximum(std::vector<int> &  bglBuffer){

//Goyal:
//In order to decide whether a BGL at time 'x' constitute a maximum:
//compare it against rounded BGLs at times 'x-3','x-2','x-1','x+1','x+2' and 'x+3'.
//BGL at time 'x' is a maximum only if
//a) if is greater than or equal to BGLs at times 'x-3','x-2','x-1'
//AND b) if it is greater than BGLs at times 'x+1','x+2' and 'x+3

if (bglBuffer[3] >= bglBuffer[0] && bglBuffer[3] >=  bglBuffer[1]&& bglBuffer[3] >=  bglBuffer[2])

    if( bglBuffer[3] > bglBuffer[4] && bglBuffer[3] >  bglBuffer[5]&& bglBuffer[3] >  bglBuffer[6]){
      //std::cout << " Trace " << bglBuffer[0] << bglBuffer[1] << bglBuffer[2] << bglBuffer[3]<< bglBuffer[4]<< bglBuffer[5]<< bglBuffer[6] << std::endl;
          return true;
      }


return false;

}

int findFirstOccurance(int from, int value ,std::vector<int> &v){

for (int i = from; i < v.size(); i++)
    if(v[i] == value)
      return i;

return -1;
}

Event::Event(){
  // Nothing to do..
}

static int counter =0;
Event::Event (std::vector<Activity> & v, int t){
  counter = counter +1;
  ID =counter;
  time = t;
  eventActivities = v;
}

int Event::getID (){
 return ID;
}
int Event::getTime (){
 return this->time;
}



std::vector<Activity> Event:: getActivities(){

      return this->eventActivities;
}

std::ostream& operator<< (std::ostream &strm,  Event &e){

    for (auto i: e.getActivities())
       strm << toTimeStamp(i.time) << " " << i.bgl<< "  "<< i.desc << std:: endl;
    return strm;
}

// if (!isEmpty(activityVector) && makeevent){  // if there is an activity there, create an event
//
//       Event event(activityVector, activityVector[0].time);
//       //std::cout <<"\nEvent " <<event.getID() << " is created and consists of the following acts:\n" ;
//       //std::cout << toTimeStamp(event.getTime());
//       //std::cout << " has the following activities: ";
//       std::cout << event << std::endl;
//       std::cout  << '\n';
//       eventsVector.push_back(event);
//       activityVector.clear();
//   } else {
//    //  do smething .....example: the first starts of the log file, till the first activity.
//  }
//
//   }
//  else{
//    lock = false;
//  }
