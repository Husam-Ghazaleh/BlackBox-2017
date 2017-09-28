#include <iostream>
#include <string>
#include <fstream>
#include<iostream>
#include<sstream>
#include <vector>
#include<stdlib.h>
#include<cmath>
std::ifstream ifstream_;
const int MINUTES_PER_DAY = 24*60;
const int MINUTES_PER_HOUR = 60;
const int ONE_DAY_IN_MINUTES =1440;
const char BGL_KEYWORD[]="bgl";
const char EXERCISE_KEYWORD[]="Starting";
const char DIET_KEYWORD[]="Adding";

struct BGL{
double time ;
double value;
};

struct Activity{
std::string desc;
double bgl;
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
  Event (std::vector<Activity> & v, int t);
  Event();
  int getID();
  int getTime ();
  std::vector<Activity> getActivities();
  friend std:: ostream& operator<< (std::ostream &strm,  Event &e);
private:
    int ID;
    int time;
    std::vector<Activity> eventActivities;
};

void bGLParser( const char * file);
int toMinutes( char * timestamp);
void baselBGL(std::vector<BGL> & v);
std::string toTimeStamp (int  time);
bool isEmpty(std::vector<Activity> &v);
bool isMinimum(std::vector<int> &  bglBuffer);
bool isMaximum(std::vector<int> & bglBuffer);
void shiftToLeft(std::vector<int> & bglBuffer);
