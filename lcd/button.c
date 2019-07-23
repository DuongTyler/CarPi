#include <wiringPi.h>
#include <stdio.h>

#include <mpd/client.h>

#include <unistd.h>

int main(void)
{
	if(wiringPiSetup() == -1)
	{
		printf("setup wiringPi failed !\n");
		return -1; 
	}
			
	for (int pin = 1; pin <= 4; pin++)
	{
		pinMode(pin, INPUT);
		pullUpDnControl(pin, PUD_UP);
	}
	
	struct mpd_connection *m_conn;
	
	
	while (1)
	{
		m_conn = mpd_connection_new(NULL, 0, 60000);	//connect each time. Reason: sometimes connection times out and causes a segfault

		mpd_command_list_begin(m_conn, true);

		if (!digitalRead(1))
		{
			printf("btn 1\n");
		}
		else if (!digitalRead(2))	//next
		{
			printf("btn 2\n");
			mpd_send_next(m_conn);
		}
		else if (!digitalRead(3))	//play/pause
		{
			printf("btn 3\n");
			mpd_send_toggle_pause(m_conn);
		}
		else if (!digitalRead(4))	//prev
		{
			printf("btn 4\n");
			mpd_send_previous(m_conn);
		}
		

		mpd_command_list_end(m_conn);
			
		mpd_response_finish(m_conn);

		printf("%d:%d:%d:%d\n",
				digitalRead(1),
				digitalRead(2),
				digitalRead(3),
				digitalRead(4));
		mpd_connection_free(m_conn);
		usleep(500000);

	}
	return 0;
}

