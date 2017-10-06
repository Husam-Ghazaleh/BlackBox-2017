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
  int roundedBGLValue =0;
  int roundedCurrentBGlTime ;
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

  int start=0;
  std::vector<int> minMaxBuffer;
  std::vector<int>minMaxBufferTime;
  std::vector<int> min1_Max1_;
  std::vector<int> min1_Max1_Time;



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
           // Filter 1: Round all BGLs to nearest 10
           roundedBGLValue = int((currentBglValue + 5) / 10) * 10;
           bglBuffer.push_back(roundedBGLValue);
           roundedCurrentBGlTime = minutes;
           bglTimeBuffer.push_back(minutes);
           if(bglBuffer.size()>=7){
                potentialBglBuffer.clear();
                for(int i =7; i>=1; i--) {
                    potentialBglBuffer.push_back(bglBuffer[bglBuffer.size()-i]);
                    //std :: cout << bglBuffer.size() << "_" << bglBuffer[bglBuffer.size()-i] << "_" << bglBuffer.size()-i << " ";
                }
                  if(isMinimum(potentialBglBuffer)){
                      if(flag ==1){
                         // Confirmed last MAX
                          max = lastMax;
                          minMaxBuffer.push_back(max);
                          minMaxBufferTime.push_back(lastMaxBglTime);


                          int index = findFirstOccurance(minBglIndexFrom, max, bglBuffer);
                          //std:: cout << toTimeStamp(bglTimeBuffer[index]) << "  " << bglBuffer[index] << "  " << "Max1" <<"\n" ;
                           min1_Max1_.push_back(bglBuffer[index]);
                           min1_Max1_Time.push_back (bglTimeBuffer[index]);

                          lastMin = potentialBglBuffer[3];
                          lastMinBglTime = bglTimeBuffer[bglTimeBuffer.size()-4];
                          minBglIndexFrom = bglBuffer.size() -4;
                          flag =0;
                          //std:: cout << toTimeStamp(lastMaxBglTime)<<"  "<< max<<"  "<<"Max" <<"\n";

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
                          min= lastMin;
                          minMaxBuffer.push_back(min);
                          minMaxBufferTime.push_back(lastMinBglTime);
                          int index = findFirstOccurance(maxBglIndexFrom, min, bglBuffer);
                          //std:: cout << toTimeStamp(bglTimeBuffer[index]) << "  " << bglBuffer[index]<< " " <<"Min1" << "\n" ;

                          min1_Max1_.push_back(bglBuffer[index]);

                          min1_Max1_Time.push_back(bglTimeBuffer[index]);



                          lastMax = potentialBglBuffer[3];
                          lastMaxBglTime= bglTimeBuffer[bglTimeBuffer.size()-4];
                          maxBglIndexFrom = bglBuffer.size()-4;
                          flag = 1;
                          //std:: cout << toTimeStamp(lastMinBglTime) << "  " <<min << "  "<<"Min" <<"\n";
                          if (!isEmpty(activityVector)){
                                Event event(activityVector, activityVector[0].time);
                                //std::cout << event << std::endl;
                                //std::cout  << '\n';
                                activityVector.clear();
                          }
                          // if(minMaxBuffer.size()>= 3){

                             //start =
                             //std::cout <<"time now: " << toTimeStamp(minutes) <<"_" <<start<<"\n";

                          // }

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
              //     potentialBglBuffer.push_back(roundedBGLValue);
              //     nextThreeLinesCounter++;
              //   }
          }
        // outfile<< toTimeStamp(minutes);
        // outfile << " " << roundedBGLValue << std::endl;
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
          activity.bgl = roundedBGLValue;
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
          outfile << toTimeStamp(roundedCurrentBGlTime);
          outfile << " " << roundedBGLValue;
          outfile << " " << descTemp << "\n" ;
            // Create a food activity:
          Activity activity;
          activity.desc = descTemp;
          activity.bgl = roundedBGLValue;
          activity.time = minutes;
          activity.howmuch = servingSizeTemp;
          activityVector.push_back(activity);
      }

    }
    ifstream_.close();
    //baselBGL(bglVector);
  }
  filters(minMaxBuffer,minMaxBufferTime,min1_Max1_ ,min1_Max1_Time,start);
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

//filters(minMaxBuffer,minMaxBufferTime,min1_Max1_ ,min1_Max1_Time,start)
void filters (std::vector<int> &minMaxBuffer ,std::vector<int> &minMaxBufferTime,std::vector<int> &min1_Max1_,std::vector<int> &min1_Max1_Time,int start ){

      //std::cout <<minmaxtime.size() << "_" << start << "\n";

  while(start < minMaxBuffer.size() ){
      if(start % 2 ==0){

           int min1 = minMaxBuffer[start];
           int max = minMaxBuffer[start+1];
           int min2= minMaxBuffer[start+2];

           if(max == min1 + 10 || max == min2 +10){
                  //std::cout << toTimeStamp(minmaxtime[start]) << " " << minmax[start] << "  Min1"<<"\n";
                  //std::cout <<toTimeStamp(minmaxtime[start+1]) <<" " <<minmax[start +1]<<"  Max1" <<"\n";
                  //std::cout <<toTimeStamp(minmaxtime[start+2] )<<" " <<minmax[start +2]<< " Min1"<<"\n";
                  if(min1<= min2){
                      std::cout << toTimeStamp(min1_Max1_Time[start]) << " " << min1_Max1_[start] << "  Min1_"<<"\n";
                      std::cout << toTimeStamp(minMaxBufferTime[start]) << " " << min1  <<" " <<"Min" << '\n';

                  }
                  else{
                      std::cout <<toTimeStamp(min1_Max1_Time[start+2] )<<" " <<min1_Max1_[start +2]<< " Min1_"<<"\n";
                      std::cout << toTimeStamp(minMaxBufferTime[start+2]) << " " << min2  <<" " <<"Min" << '\n';

                  }
                 //std::cout <<std::min(min1,min2) <<"\n";
                 start = start+3;
            }
           else {
                   std::cout << toTimeStamp(min1_Max1_Time[start]) <<"  "<< min1_Max1_[start] << " Min1_ "<< "\n";
                   std::cout << toTimeStamp(minMaxBufferTime[start]) <<"  "<< min1 << " Min "<< "\n";
                   //std::cout << max <<" Max "<< "\n";
                   //std::cout << min2 << " Min2 "<<"\n";
                   start =start+1;

                }
        }

        else {
            int max1 =minMaxBuffer[start];
            int min =minMaxBuffer[start+1];
            int max2 =minMaxBuffer[start+2];
            if(min == max1-10 || min == max2 -10){
            //    std::cout <<toTimeStamp(minmaxtime[start]) << " " << minmax[start] << " Max1"<<"\n";
            //    std::cout <<toTimeStamp(minmaxtime[start+1]) <<" " <<minmax[start +1]<<" Min1" <<"\n";
            //    std::cout <<toTimeStamp(minmaxtime[start+2]) <<" " <<minmax[start +2]<< " Max1"<<"\n";
            //  std::cout <<std::max(max1,max2) <<"\n";
            if(max1>=max2){
              std::cout << toTimeStamp(min1_Max1_Time[start]) << " " << min1_Max1_[start] << "  Max1_"<<"\n";
              std::cout << toTimeStamp(minMaxBufferTime[start]) << " " << max1  <<" " <<"Max" << '\n';



            }else{
              std::cout <<toTimeStamp(min1_Max1_Time[start+2] )<<" " <<min1_Max1_[start +2]<< " Max1_"<<"\n";
              std::cout << toTimeStamp(minMaxBufferTime[start+2]) << " " << max2  <<" " <<"Max" << '\n';


            }



                start =start+3 ;
            }
            else{

              std::cout << toTimeStamp(min1_Max1_Time[start]) <<"  "<< min1_Max1_[start] << " Max1_ "<< "\n";
              std::cout <<toTimeStamp(minMaxBufferTime[start]) << "  " <<max1  <<" Max "<<"\n";
              //std::cout << min <<" Min " <<"\n";
              //std::cout << max2  <<" Max2 "<<"\n";
               start =start+1;
            }
        }

  }

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
