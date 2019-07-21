#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <lcd.h>

#include <unistd.h>

#include <mpd/client.h>

#define skip 22
#define stop 23
#define play 24
#define prev 25

//must link mpdclient, wiringPi, and wiringPiDev

int main(int args, char *argv[])
{

	struct mpd_connection* m_connection = NULL;
	struct mpd_status* m_status = NULL;
	char* m_state_str;

	m_connection = mpd_connection_new(NULL, 0, 30000);


	FILE *fp;
	int fd;
	char temp_char[15];
	char Total[20];
	char Free[20];
	float Temp;
	
	if(wiringPiSetup() == -1){
		printf("wiringPi setup failed !\n");
		exit(1);
	}
	
	fd = lcdInit(2,16,4, 5,4, 0,1,2,3,4,5,6,7); //see /usr/local/include/lcd.h
	if (fd == -1){
		printf ("lcdInit 1 failed\n") ;
		return 1;
	}
	sleep(1);

	lcdPosition(fd, 0, 0); lcdPuts (fd, "  Car Pi v1.0");
	sleep(1);

	if(argv[1]){
		lcdPosition(fd, 0, 0);
		lcdPuts(fd, "                ");
		lcdPosition(fd, 0, 0); lcdPuts(fd, argv[1]);
	} 
	for (int pin = 22; pin <= 25; pin++)
	{
		pinMode(pin, INPUT);
		pullUpDnControl(pin, PUD_UP);
	}

	while(1){
		
	/*	printf("prev: %d play: %d pause: %d skip: %d\n", 
				digitalRead(prev), 
				digitalRead(play), 
				digitalRead(stop), 
				digitalRead(skip));
	*/	
		int command = -1;
		for (int pin = 22; pin <= 25; pin++)
		{
			int stat = digitalRead(pin);
			if (digitalRead(pin) == 0)
			{
				printf("Pressed key %d\nValue: %d\n", pin, digitalRead(pin));
				command = pin;
				usleep(500000);		//sleep for 0.5 seconds
				//break;
			}
		}

		switch (command)
		{
			case skip:
				system("echo skip");
				break;
			case stop:
				system("echo stop");
				break;
			case play:
				system("echo play");
				break;
			case prev:
				system("echo prev");
				break;
			default:
				break;
		}
		
		if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
			fprintf(stderr, "Could not connect to MPD: %s\n", mpd_connection_get_error_message(m_connection));
                        mpd_connection_free(m_connection);
                        m_connection = NULL;
                }
					    
		m_status = mpd_run_status(m_connection);
		if (mpd_status_get_state(m_status) == MPD_STATE_PLAY) {
		        m_state_str = "playing";
		} else if (mpd_status_get_state(m_status) == MPD_STATE_STOP) {
		        m_state_str = "stopped";
		} else if (mpd_status_get_state(m_status) == MPD_STATE_PAUSE) {
		        m_state_str = "paused";
		} else {
		        m_state_str = "unknown";
		}	
		printf("MPD state: %s\n", m_state_str);
		printf("%u%\n", mpd_status_get_volume(m_status));

	}

	return 0;

}


