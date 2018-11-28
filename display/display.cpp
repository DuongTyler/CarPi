
#define DEBUG
//#define PI


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#ifdef PI
	#include <wiringPi.h>
	#include <lcd.h>
#endif

#define play 0
#define pause 0
#define stop 0
#define next 0
#define prev 0
#define volup 0
#define voldown 0
#define rand 0
#define clear 0
#define playlist 0
#define up 0
#define down 0
#define left 0
#define right 0
#define select 0


#ifdef PI
void lcdPrint (int handle, char [] msg){

	lcdPosition(handle,0,0);							//resets position lcd stream
	lcdPuts(handle, "                                                                ");	//clear screen
	lcdPosition(handle,0,0);							//resets position lcd stream
	lcdPuts(handle, msg);

}

int getInput(){
	
	     if (digitalRead(play)	 == 1){	return play;	}
	else if (digitalRead(pause)	 == 1){	return pause;	}
	else if (digitalRead(stop)	 == 1){	return stop;	}
	else if (digitalRead(next)	 == 1){	return next;	}
	else if (digitalRead(prev)	 == 1){	return prev;	}
	else if (digitalRead(volup)	 == 1){	return volup;	}
	else if (digitalRead(voldown)	 == 1){	return voldown;	}
	else if (digitalRead(rand)	 == 1){	return rand;	}
	else if (digitalRead(clear)	 == 1){	return clear;	}
	else if (digitalRead(playlist)	 == 1){	return playlist;}
	else if (digitalRead(select)	 == 1){	return select;	}
	else if (digitalRead(up)	 == 1){	return up;	}
	else if (digitalRead(down)	 == 1){	return down;	}
	else if (digitalRead(left)	 == 1){	return left;	}
	else if (digitalRead(right)	 == 1){	return right;	}
	else 				      {	return -1;	}

}
#endif


/*
class Song 
{
	public:
		char [] title;
		bool isPlaying;
		Song( char[] tt, bool np){
			
			title = tt;
			isPlaying = np;
			
		}
		

};
*/







int main(int argc, char** argv){


//	Song test = new Song("songname", false);
//	std::cout<< test.title << std::endl;
//	delete test;

#ifdef PI
//========================== BEGIN INITIALIZING LCD =================================================
	std::cout<<">> Initializing WiringPi"<<std::endl;
	if (wiringPiSetup() == -1){					//init wiringpi
		std::cout<<">> Initialization Failed. WiringPi failed to set up properly"<<std::endl;
		return -1;
	}
	std::cout<<">> Creating Handle"<<std::endl;
	int handle = lcdInit(2,16,4,5,4,0,1,2,3,4,5,6,7);		//handle for lcd
//=========================== END INITIALIZING LCD ==================================================




//=========================== BEGIN PRINTING LOADING MESSAGE ========================================
	lcdPrint(handle, "Pi Car Mod      by: Tyler Duong");		//output loading screen
//================================ END PRINT ========================================================
#endif




//========================== BEGIN CALLS TO SCRIPTS =================================================
	system("bash /home/nimda/git/CarPi/scripts/play.sh");
	system("ncmpcpp --current-song={'%f'} |awk -F '.mp3' '{print $1}' > /tmp/myfile");	//output current playing to tmp file
	#ifdef DEBUG
		std::cout<<">> Scripts Completed"<<std::endl;
	#endif
//+========================== END CALLS TO SCRIPTS ==================================================





//============================= BEGIN FILE READ ======================================================
	FILE *file = fopen("/tmp/myfile","r");			//open to read
	#ifdef DEBUG
		std::cout<<">> opened ifstream"<<std::endl;
	#endif
	char fileContent[33];
	fgets(fileContent, 33,file);				//assign contents to fileContent	
	fclose(file);						//close file
//============================== END FILE READ =======================================================





	std::cout<<fileContent<<std::endl;			//for debugging
	
	#ifdef PI
		lcdPrint(handle, fileContent);				//output to lcd
	#endif
	#ifdef DEBUG
		#ifdef PI
		std::cout<<">> printed to lcd"<<std::endl;
		#endif
	#endif
	
	return 0;
}






//#include <vector>
//struct OutputChar 
//{
//	char data[100];
//	std::cout<<"Constructed size "<<100<<std::endl;
//};

//	std::string x = "hi";
//	lcdPuts(handle, x.c_str());					//example... DO NOT USE
