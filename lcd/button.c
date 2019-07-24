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
			
	//for (int pin = 0; pin <= 4 && pin != 1; pin++)
	//{
	//	pinMode(pin, INPUT);
	//	pullUpDnControl(pin, PUD_UP);
	//}

	pinMode(0, INPUT);
	pullUpDnControl(0, PUD_UP);

	pinMode(3, INPUT);
	pullUpDnControl(3, PUD_UP);
	
	pinMode(4, INPUT);
	pullUpDnControl(4, PUD_UP);

	pinMode(2, INPUT);
	pullUpDnControl(2, PUD_UP);

	struct mpd_connection *m_conn;
	
	m_conn = mpd_connection_new(NULL, 0, 60000);
	
	mpd_command_list_begin(m_conn, true);

	mpd_send_crossfade(m_conn, 5);
	mpd_send_shuffle(m_conn);

	mpd_command_list_end(m_conn);
	mpd_response_next(m_conn);
	mpd_response_finish(m_conn);
	mpd_connection_free(m_conn);

	while (1)
	{
		m_conn = mpd_connection_new(NULL, 0, 60000);	//connect each time. Reason: sometimes connection times out and causes a segfault

		mpd_command_list_begin(m_conn, true);

		if (!digitalRead(0))		//vol up
		{
			printf("btn 1\n");
			mpd_send_change_volume(m_conn, 5);
		}
		else if (!digitalRead(3))	//next
		{
			printf("btn 3\n");
			mpd_send_next(m_conn);
		}	
		else if (!digitalRead(4))	//pause/play
		{
			printf("btn 4\n");
			mpd_send_toggle_pause(m_conn);
		}
		else if (!digitalRead(2))	//vol down
		{
			printf("btn 2\n");
			mpd_send_change_volume(m_conn, -5);
		}
	
		

		mpd_command_list_end(m_conn);
			
		mpd_response_finish(m_conn);

		printf("%d:%d:%d:%d\n",
				digitalRead(0),	//17
				digitalRead(3),	//23
				digitalRead(4),	//27
				digitalRead(2));//22
		mpd_connection_free(m_conn);
		usleep(500000);

	}
	return 0;
}

