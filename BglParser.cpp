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
  int roundedBGlTime ;

  int lastMin = 9999999, lastMax, max, min;
  int lastMinBglTime, lastMaxBglTime;
  int flag =0; // 0 means min and 1 means max

  std::vector<int> potentialBglVector ={0};
  std::vector<Activity> activityVector;
  std::vector<int> bglVector ={0};
  std::vector<double> bglTimeVector ={0};

  int start=0;
  std::vector<int> minMaxVector;
  std::vector<int>minMaxVectorTime;
  std::vector<int> min1_Max1_;
  std::vector<int> min1_Max1_Time;

  std::string line;
  int  minBglIndexFrom =0;
  int  maxBglIndexFrom =0;


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
           bglVector.push_back(roundedBGLValue);
           roundedBGlTime = minutes;
           bglTimeVector.push_back(minutes);
           if(bglVector.size()>=7){
                potentialBglVector.clear();
                for(int i =7; i>=1; i--) {
                    potentialBglVector.push_back(bglVector[bglVector.size()-i]);
                    //std :: cout << bglVector.size() << "_" << bglVector[bglVector.size()-i] << "_" << bglVector.size()-i << " ";
                }
                  if(isMinimum(potentialBglVector)){
                      if(flag ==1){
                         // Confirmed last MAX
                          max = lastMax;
                          minMaxVector.push_back(max);
                          minMaxVectorTime.push_back(lastMaxBglTime);


                          int index = findFirstOccurance(minBglIndexFrom, max, bglVector);
                          //std:: cout << toTimeStamp(bglTimeVector[index]) << "  " << bglVector[index] << "  " << "Max1" <<"\n" ;
                           min1_Max1_.push_back(bglVector[index]);
                           min1_Max1_Time.push_back (bglTimeVector[index]);

                          lastMin = potentialBglVector[3];
                          lastMinBglTime = bglTimeVector[bglTimeVector.size()-4];
                          minBglIndexFrom = bglVector.size() -4;
                          flag =0;
                          //std:: cout << toTimeStamp(lastMaxBglTime)<<"  "<< max<<"  "<<"Max" <<"\n";

                       }else if (flag == 0){
                        // check if the current is less than prev min
                          if (lastMin > potentialBglVector[3]){
                              lastMin = potentialBglVector[3];
                              lastMinBglTime = bglTimeVector[bglTimeVector.size()-4];
                              minBglIndexFrom = bglVector.size() -4;
                            }
                        }
                  }else if(isMaximum(potentialBglVector)){
                      if(flag ==0 ){
                        // confirmed the last min
                          min= lastMin;
                          minMaxVector.push_back(min);
                          minMaxVectorTime.push_back(lastMinBglTime);
                          int index = findFirstOccurance(maxBglIndexFrom, min, bglVector);
                          //std:: cout << toTimeStamp(bglTimeVector[index]) << "  " << bglVector[index]<< " " <<"Min1" << "\n" ;

                          min1_Max1_.push_back(bglVector[index]);

                          min1_Max1_Time.push_back(bglTimeVector[index]);
                          lastMax = potentialBglVector[3];
                          lastMaxBglTime= bglTimeVector[bglTimeVector.size()-4];
                          maxBglIndexFrom = bglVector.size()-4;
                          flag = 1;
                          //std:: cout << toTimeStamp(lastMinBglTime) << "  " <<min << "  "<<"Min" <<"\n";
                          // if (!isEmpty(activityVector)){
                          //       Event event(activityVector, activityVector[0].time);
                          //       activityVector.clear();
                          // }

                      }else if (flag ==1){
                        // check if the current is greater than the prev max
                          if(lastMax < potentialBglVector[3]){
                              lastMaxBglTime= bglTimeVector[bglTimeVector.size()-4];
                              lastMax = potentialBglVector[3];
                              maxBglIndexFrom = bglVector.size()-4;
                            }
                        }
                  }//max
          }
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
          outfile << toTimeStamp(roundedBGlTime);
          outfile << " " << roundedBGLValue;
          outfile << " " << descTemp << "\n" ;
            // Create a food activity:
          Activity activity;
          activity.desc = descTemp;
          activity.time = minutes;
          activity.howmuch = servingSizeTemp;
          activityVector.push_back(activity);
      }

    }
    ifstream_.close();
    //baselBGL(bglVector);
  }
  filters(minMaxVector,minMaxVectorTime,min1_Max1_ ,min1_Max1_Time,activityVector,start);
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
bool isMinimum(std::vector<int> &  bglVector){

// Applying rule 5
  if (bglVector[3] <= bglVector[0] && bglVector[3] <=  bglVector[1]&& bglVector[3] <=  bglVector[2])

      if( bglVector[3] < bglVector[4] && bglVector[3] <  bglVector[5]&& bglVector[3] <  bglVector[6]){

        //std::cout << " Trace " << bglVector[0] << bglVector[1] << bglVector[2] << bglVector[3]<< bglVector[4]<< bglVector[5]<< bglVector[6] << std::endl;
            return true;
    }
  return false;
}

bool isMaximum(std::vector<int> &  bglVector){

 //Applying rule 6
  if (bglVector[3] >= bglVector[0] && bglVector[3] >=  bglVector[1]&& bglVector[3] >=  bglVector[2])

      if( bglVector[3] > bglVector[4] && bglVector[3] >  bglVector[5]&& bglVector[3] >  bglVector[6]){
      //std::cout << " Trace " << bglVector[0] << bglVector[1] << bglVector[2] << bglVector[3]<< bglVector[4]<< bglVector[5]<< bglVector[6] << std::endl;
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
Event::Event (std::vector<Activity> & v, int min1,int max1_,int max, int min2_, int min2, int min1Time,int max1_Time,int maxTime,int min2_Time, int min2Time){
  counter = counter +1;
  ID =counter;
  eventActivities = v;
   this->min1 = min1;
   this->max1_ = max1_;
   this->max = max;
   this->min2_ = min2_;
   this->min2 = min2;
   this->min1Time = min1Time;
   this->max1_Time = max1_Time;
   this->maxTime = maxTime;
   this->min2_Time = min2_Time;
   this->min2Time = min2Time;
}

int Event::getID (){
 return ID;
}

std::vector<Activity> Event:: getActivities(){

      return this->eventActivities;
}

std::ostream& operator<< (std::ostream &strm,  Event &e){
    int previousetime =0;
    std:: cout  <<toTimeStamp(e.getMin1Time()) <<" " << e.getMin1() << " Min1"<<"\n";
    for (auto i: e.getActivities()){
      if( previousetime <= e.getMax1_Time() &&  e.getMax1_Time() <= i.time )
            std::cout  <<  toTimeStamp(e.getMax1_Time()) << " "<< e.getMax1_()<<" Max1_ ""\n";

      if( previousetime <= e.getMaxTime() &&  e.getMaxTime() <= i.time )
            std::cout << toTimeStamp(e.getMaxTime()) <<" " << e.getMax() << " Max " << "\n";

      if( previousetime <= e.getMin2_Time() &&  e.getMin2_Time() <= i.time )
            std::cout  <<  toTimeStamp(e.getMin2_Time()) <<" "<< e.getMin2_()<<" Min2_"<< "\n";

       strm <<toTimeStamp(i.time) << "  "<< i.desc <<" " <<i.howmuch <<std:: endl;

       previousetime = i.time;

    }

    //std::cout << "Min2_mmmmm  " <<  toTimeStamp(e.getMin2_Time())<< " " << e.getMin2_() <<"\n";
    std:: cout <<toTimeStamp(e.getMin2Time()) << " "<< e.getMin2() << " Min2"  << "\n";
    return strm;
}
void filters (std::vector<int> &minMaxVector ,std::vector<int> &minMaxVectorTime,std::vector<int> &min1_Max1_,std::vector<int> &min1_Max1_Time,std::vector<Activity> &act,int start ){

  std::vector<int> finalminMaxVector;
  std::vector<int> finalminMaxTimeVector;
  std::vector<int> finalmin1_Max1_Vector;
  std::vector<int> finalmin1_Max1_VectorTime;
  // Applying the rules 7,8,9,10, and 11.
  while(start < minMaxVector.size() ){
      if(start % 2 ==0){
           int min1 = minMaxVector[start];
           int max = minMaxVector[start+1];
           int min2= minMaxVector[start+2];
           if(max == min1 + 10 || max == min2 +10){
                  if(min1<= min2){
                      int time =minMaxVectorTime[start];
                      std::cout << toTimeStamp(min1_Max1_Time[start]) << " " << min1_Max1_[start] << "  Min1_"<<"\n";
                      std::cout << toTimeStamp(time) << " " << min1  <<" " <<"Min" << '\n';
                      finalminMaxVector.push_back(min1);
                      finalminMaxTimeVector.push_back(time);
                      finalmin1_Max1_Vector.push_back(min1_Max1_[start]);
                      finalmin1_Max1_VectorTime.push_back(min1_Max1_Time[start]);


                  }
                  else{
                      int time =minMaxVectorTime[start +2];
                      std::cout <<toTimeStamp(min1_Max1_Time[start+2] )<<" " <<min1_Max1_[start +2]<< " Min1_"<<"\n";
                      std::cout << toTimeStamp(time) << " " << min2  <<" " <<"Min" << '\n';
                      finalminMaxVector.push_back(min2);
                      finalminMaxTimeVector.push_back(time);
                      finalmin1_Max1_Vector.push_back(min1_Max1_[start+2]);
                      finalmin1_Max1_VectorTime.push_back(min1_Max1_Time[start+2]);


                  }
                 start = start+3;
            }
           else {
                   int time =minMaxVectorTime[start];
                   std::cout << toTimeStamp(min1_Max1_Time[start]) <<"  "<< min1_Max1_[start] << " Min1_ "<< "\n";
                   std::cout << toTimeStamp(minMaxVectorTime[start]) <<"  "<< min1 << " Min "<< "\n";
                   finalminMaxVector.push_back(min1);
                   finalminMaxTimeVector.push_back(time);
                   finalmin1_Max1_Vector.push_back(min1_Max1_[start]);
                   finalmin1_Max1_VectorTime.push_back(min1_Max1_Time[start]);
                   

                   start =start+1;
                }
      }else {
          int max1 =minMaxVector[start];
          int min =minMaxVector[start+1];
          int max2 =minMaxVector[start+2];
          if(min == max1-10 || min == max2 -10){
              if(max1>=max2){
                  int time =minMaxVectorTime[start];
                  std::cout << toTimeStamp(min1_Max1_Time[start]) << " " << min1_Max1_[start] << "  Max1_"<<"\n";
                  std::cout << toTimeStamp(time) << " " << max1  <<" " <<"Max" << '\n';
                  finalminMaxVector.push_back(max1);
                  finalminMaxTimeVector.push_back(time);
                  finalmin1_Max1_Vector.push_back(min1_Max1_[start]);
                  finalmin1_Max1_VectorTime.push_back(min1_Max1_Time[start]);



               }else{
                 int time =minMaxVectorTime[start +2];
                  std::cout <<toTimeStamp(min1_Max1_Time[start+2] )<<" " <<min1_Max1_[start +2]<< " Max1_"<<"\n";
                  std::cout << toTimeStamp(time) << " " << max2  <<" " <<"Max" << '\n';
                  finalminMaxVector.push_back(max2);
                  finalminMaxTimeVector.push_back(time);
                  finalmin1_Max1_Vector.push_back(min1_Max1_[start+2]);
                  finalmin1_Max1_VectorTime.push_back(min1_Max1_Time[start+2]);

               }
              start =start+3 ;
          }else{
              int time = minMaxVectorTime[start];
              std::cout << toTimeStamp(min1_Max1_Time[start]) <<"  "<< min1_Max1_[start] << " Max1_ "<< "\n";
              std::cout <<toTimeStamp(time) << "  " << max1  <<" Max "<<"\n";
              finalminMaxVector.push_back(max1);
              finalminMaxTimeVector.push_back(time);
              finalmin1_Max1_Vector.push_back(min1_Max1_[start]);
              finalmin1_Max1_VectorTime.push_back(min1_Max1_Time[start]);

              start =start+1;
            }
        }
    }

    createEvents(finalminMaxVector,finalminMaxTimeVector,finalmin1_Max1_Vector,finalmin1_Max1_VectorTime,act);
}

int createEvents(std::vector<int> &minMaxVector ,std::vector<int> &minMaxVectorTime,std::vector<int> &min1_Max1_,std::vector<int> &min1_Max1_Time,std::vector<Activity> &act){

  int numberOfEvent =0;
  int i =0;
  int start =0;
  std::vector<Activity> actTemp;
  int min1;
  int max1_;
  int max;
  int min2_;
  int min2;

  int min1Time;
  int max1_Time;
  int maxTime;
  int min2_Time;
  int min2Time;

  if(act.size() ==0)
   return 0;

   while( i < minMaxVector.size()){

          if(start == act.size())
             return numberOfEvent;
//std::cout << "act[start].desc "<<"\n";
          while(start < act.size()){

              //std::cout <<"start_" <<start <<"\n";
              //std::cout <<"i_" <<i <<"\n";

              if (act[start].time >= minMaxVectorTime[i] &&  act[start].time < minMaxVectorTime[i+2]){
                 //std::cout << "act[start].desc "<<"\n";

                 actTemp.push_back(act[start]);
                 start++;
              }
              else
                 break;
           }

          // need code here to prevent empty events

          min1  = minMaxVector[i];
          max1_ = min1_Max1_[i+1];
          max   =  minMaxVector[i+1];
          min2_ = min1_Max1_[i+2];
          min2 = minMaxVector[i+2];
          min1Time = minMaxVectorTime[i];
          max1_Time = min1_Max1_Time[i+1];
          maxTime = minMaxVectorTime[i+1];
          min2_Time = min1_Max1_Time[i+2];
          min2Time = minMaxVectorTime[i+2];
          Event event(actTemp, min1,max1_,max,min2_,min2,min1Time,max1_Time,maxTime,min2_Time,min2Time);

          actTemp.push_back(act[start]);
          std::cout <<"Event "<<event.getID() << "\n";
          std::cout << event <<"\n";
          numberOfEvent++;
          actTemp.clear();
          i = i+2;

   }
return numberOfEvent;
}
