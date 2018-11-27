#include <stdio.h>
#include <stdlib.h>
//#include <wiringPi.h>
//#include <lcd.h>
#include <iostream>
#include <fstream>
#include <string>


/*void clearLCD (int handle){

	lcdPosition(handle,0,0);										//resets position lcd stream
	lcdPuts(handle, "                                                                ");			//clear screen
	lcdPosition(handle,0,0);										//resets position lcd stream

}*/

int main(int argc, char** argv){

											std::cout<<">> Initializing WiringPi"<<std::endl;
/*
	wiringPiSetup();						//set up wiringpi
											std::cout<<">> Starting Display"<<std::endl;
	int handle = lcdInit(2,16,4,5,4,0,1,2,3,4,5,6,7);		//handle for lcd
	clearLCD(handle);
	lcdPuts(handle, "Pi Car Mod      by: Tyler Duong");		//output init
*/	system("sleep 1");						//wait to see init



	system("bash /home/nimda/git/CarPi/scripts/play.sh");
	system("ncmpcpp --current-song={'%f'} |awk -F '.mp3' '{print $1}' > /tmp/myfile");	//output current playing to tmp file
	/*debug*/std::cout<<">> executed ncmpcpp command"<<std::endl;



	FILE *file = fopen("/tmp/myfile","r");			//open to read
	/*debug*/std::cout<<">> opened ifstream"<<std::endl;
	char fileContent[33];					//assign contents to fileContent
	fgets(fileContent, 33,file);				//close file	
	fclose(file);		

//	clearLCD(handle);
//	lcdPosition(handle,0,0);				//resets position lcd stream
	std::cout<<fileContent<<std::endl;			//for debugging
//	clearLCD(handle);
//	lcdPuts(handle, fileContent);				//output to lcd
	/*debug*/std::cout<<">> printed to lcd"<<std::endl;

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
