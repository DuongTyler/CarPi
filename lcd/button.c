/*
 * * File name   : btnAndLed.c
 * * Description : Toggle a led by button.
 * * Website     : www.adeept.com
 * * E-mail      : support@adeept.com
 * * Author      : Jason
 * * Date        : 2015/05/02
 * */
#include <wiringPi.h>
#include <stdio.h>

#define ButtonPin 25

int status = 1;

void myISR(void)
{
		status = !status;	
}

int main(void)
{
	if(wiringPiSetup() == -1){ //when initialize wiring failed,print message to screen
		printf("setup wiringPi failed !\n");
		return -1; 
	}
			
	pinMode(ButtonPin, INPUT);
	pullUpDnControl(ButtonPin, PUD_UP);
						
	if(wiringPiISR(ButtonPin, INT_EDGE_FALLING, myISR) < 0){
		printf("ISR setup error!\n");
		return -1;
	}

	while(1){
		printf("%d\n",status);
	}

	return 0;
}

