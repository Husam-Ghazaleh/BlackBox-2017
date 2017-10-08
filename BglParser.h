#include <iostream>
#include <string>
#include <fstream>
#include<iostream>
#include<sstream>
#include <vector>
#include<stdlib.h>
#include <algorithm>
#include<cmath>
std::ifstream ifstream_;
const int MINUTES_PER_DAY = 24*60;
const int MINUTES_PER_HOUR = 60;
const int ONE_DAY_IN_MINUTES =1440;
const char BGL_KEYWORD[]="bgl";
const char EXERCISE_KEYWORD[]="Starting";
const char DIET_KEYWORD[]="Adding";

struct Activity{
std::string desc;
double time;
double howmuch;

};

struct Period{

int lenght;
double initBgl;
double peakBgl;
double peakTime;//time when the peak occurs since the period's start
double endBgl;
};


class Event{
public:
  Event (std::vector<Activity> & v, int min1,int max1_,int max,int min2_,int min2,int min1Time,int max1_Time,int maxTime,int min2_Time, int min2Time);
  Event();
  int getID();
  int getMin1();
  int getMax1_();
  int getMax();
  int getMin2_();
  int getMin2();

  int getMin1Time();
  int getMax1_Time();
  int getMaxTime();
  int getMin2_Time();
  int getMin2Time();

  std::vector<Activity> getActivities();
  friend std:: ostream& operator<< (std::ostream &strm,  Event &e);
private:
    int ID;
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
    std::vector<Activity> eventActivities;
};

inline int Event::getMin1(){
 return min1;
}
inline int Event::getMax1_(){
  return max1_;
}
inline int Event::getMax(){
   return  max;
}
inline int Event::getMin2_(){
  return min2_;
}
inline int Event::getMin2(){
  return min2;
}
inline int Event::getMin1Time(){
  return min1Time;
}
inline int Event::getMax1_Time(){
  return max1_Time;
}
inline int Event::getMaxTime(){
   return maxTime;
}
inline int Event::getMin2_Time(){
   return min2_Time;
}
inline int Event::getMin2Time(){
  return min2Time;
}


void bGLParser( const char * file);
int toMinutes( char * timestamp);
std::string toTimeStamp (int  time);
bool isMinimum(std::vector<int> &  bglBuffer);
bool isMaximum(std::vector<int> & bglBuffer);
int findFirstOccurance(int from, int value ,std::vector<int> &v);
void filters (std::vector<int> &v ,std::vector<int> &vt,std::vector<int> &v1,std::vector<int> &v1t,std::vector<Activity>&act,int start);
int createEvents(std::vector<int> &minMaxVector ,std::vector<int> &minMaxVectorTime,std::vector<int> &min1_Max1_,std::vector<int> &min1_Max1_Time,std::vector<Activity> &act);
