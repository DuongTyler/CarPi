//compile with g++/gcc and link mpdclient with -lmpdclient

#include <stdio.h>
#include <unistd.h>
#include <mpd/client.h>

int main(void) 
{
	struct mpd_connection* m_connection = NULL;
	struct mpd_status* m_status = NULL;
	struct mpd_song *song;

	char *m_state_str = "\033[0;31mERR\033[0m";

	printf("\033[0;33mConnecting to Music Player Daemon...\033[0m\n");

//	m_connection = mpd_connection_new(NULL, 0, 30000);

	while (1)
	{
		char command;
		printf("Send Command: ");
		scanf("%c", &command);
		m_connection = mpd_connection_new(NULL, 0, 30000);


//		printf("IDLE: %d\n", mpd_recv_idle(m_connection, false));

		if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS)
		{
			fprintf(stderr, "\033[0;31mCould not connect to MPD: %s\033[0m\n", mpd_connection_get_error_message(m_connection));
        		mpd_connection_free(m_connection);
			m_connection = NULL;
		}

		printf("\033[0;32m>>ENTER Switch\033[0m\n");
		switch(command)
		{
			case 'p':
				printf("\033[0;33mSending Pause/Play\033[0m\n");
				mpd_send_toggle_pause(m_connection);
				break;

			default:
				while(getchar() != '\n');	//do not get newline
				printf("\n\033[0;31mInvalid Command\033[0m\n");
				continue;
		}
		printf("\033[0;32m>>EXIT Switch\033[0m\n");


		printf("\033[0;32m>>GET mpd_recv_status\033[0m\n");
		m_status = mpd_recv_status(m_connection);
		printf("\033[0;32m>>GOT mpd_recv_status\033[0m\n");

		printf("Status Code: %d\n", mpd_status_get_state(m_status));
		
//		mpd_run_current_song(m_connection);
		printf("\033[0;32m>>GET mpd_status_get_state\033[0m\n");
		switch(mpd_status_get_state(m_status))
		{
			case MPD_STATE_PLAY:
				m_state_str = "playing";
				break;
			case MPD_STATE_PAUSE:
				m_state_str = "paused";
				break;
			case MPD_STATE_STOP:
				m_state_str = "stopped";
				break;
			default:
				m_state_str = "FAILED";
				break;
		}
		printf("\033[0;32m>>GOT mpd_status_get_state\033[0m\n");
		

		/*** Print MPD status ***/
		printf("\033[0;34mMPD state: %s\033[0m\n", m_state_str);


//		char *path = "/home/nimda/Music/80s/Through_the_Fire.mp3";
//		mpd_send_add(m_connection, path);

		/*** mpd_recv_song ***/
		//printf("\033[0;32m>>GET mpd_recev_song\033[0m\n");
		//song = mpd_recv_song(m_connection);	//don't segfault
		//printf("\033[0;32m>>GOT mpd_recev_song\033[0m\n");

		sleep(1);
		while(getchar() != '\n');	//flush the input stream
		mpd_status_free(m_status);
	
	}
	return 0;
}
