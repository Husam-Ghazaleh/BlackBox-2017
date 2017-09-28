
#include<iostream>
#include<string>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
int main(int argc, const char * argv[]) {

	srand(time(0));
	ofstream outfile;
	int minutes[6];
	string howmuch[4] ={ "200", "80", "250", "160"};
	for(int i =0; i < 6; i++)
		minutes[i]= i * 10;
	outfile.open("Eventv1.txt");

	if(outfile.fail()){
	 cerr << " outfile file opening failed:\n";
	 exit(1);
 	}

	for ( int day =0 ; day < 90; day++) {

		int bk_time = rand()%(8-7 + 1 )+7;
    //cout <<"Test " << 1+ (rand()% 19) << endl;
		//int Lunch_time = rand()%(15-13 + 1 )+12;
		//int Dinner_time = rand()%(21-19 + 1)+18;
		int Minutes = rand()% 5;
		enum type {food,excer};

		 outfile << day << ":" <<  bk_time << ":" << minutes[Minutes] << " " << food  << " " <<  1+ (rand()% 19) << " "<< howmuch[rand()% 4]  << endl;
		 outfile << day << ":" << (bk_time + 2)<< ":" << minutes[Minutes] << " " << excer << " " <<  1 + (rand() % 2) <<" "  << 60 << endl;
		 outfile << day << ":" << (bk_time + 3) << ":" << minutes[Minutes] << " " << food << " " << 1 + (rand()% 19 ) << " "  << howmuch[rand()% 4]<< endl;
		 outfile << day << ":" <<  bk_time + 6 << ":" << minutes[Minutes] << " " << food << " " <<  1 + (rand()% 19) << " " << howmuch[rand()% 4] << endl;
		 outfile << day << ":" << (bk_time + 9) << ":" << minutes[Minutes] << " " << food << " " <<  1 + (rand()% 19) << " "  << howmuch[rand()% 4] << endl;
		 outfile << day << ":" <<  bk_time + 13 << ":" << minutes[Minutes] << " " << food << " " <<  1+ (rand()% 19)  << " "  << howmuch[rand()% 4]  << endl;
	}
	outfile << "90" << ":" <<  "0" << ":" << "0" << " " << "2" <<" " <<"0" << " " << "0";

}
