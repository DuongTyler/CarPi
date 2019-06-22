//compile with g++/gcc and link mpdclient with -lmpdclient

#include <stdio.h>
#include <unistd.h>
#include <mpd/client.h>

#define DEBUG true

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


		if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS)
		{
			fprintf(stderr, "\033[0;31mCould not connect to MPD: %s\033[0m\n", mpd_connection_get_error_message(m_connection));
        		mpd_connection_free(m_connection);
			m_connection = NULL;
		}
#ifdef DEBUG
		printf("\033[0;32m>>ENTER Switch\033[0m\n");
#endif
		switch(command)
		{
			case 'p':
#ifdef DEBUG
				printf("\033[0;33mSending Pause/Play\033[0m\n");
#endif
				mpd_send_toggle_pause(m_connection);
				break;
			case 'q':
				printf("EXITING\n");
				if ( m_status != NULL ) mpd_status_free(m_status);	//TODO: Fix double free thingy
				return 0;

			default:
				while(getchar() != '\n');	//flush the stream
				printf("\n\033[0;31mInvalid Command\033[0m\n");
				continue;
		}
#ifdef DEBUG
		printf("\033[0;32m>>EXIT Switch\033[0m\n");
#endif


#ifdef DEBUG
		printf("\033[0;32m>>GET mpd_recv_status\033[0m\n");
#endif
		m_status = mpd_recv_status(m_connection);	//TODO: fix wrong values returned by mpd_recv_status()
#ifdef DEBUG
		printf("\033[0;32m>>GOT mpd_recv_status\033[0m\n");
		printf("MPD Status Code: %d\n", mpd_status_get_state(m_status));
		printf("\033[0;32m>>GET mpd_status_get_state\033[0m\n");
#endif
		switch(mpd_status_get_state(m_status))		//TODO: Fix mpd state to actually be useful
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
#ifdef DEBUG
		printf("\033[0;32m>>GOT mpd_status_get_state\033[0m\n");
#endif
		

		/*** Print MPD status ***/
		printf("\033[0;34mMPD state: %s\033[0m\n", m_state_str);

		while(getchar() != '\n');	//flush the input stream
		mpd_status_free(m_status);
	
	}
	return 0;
}











/*SAVE FOR LATER*/

	/*Add a new song to the playlist*/
//		char *path = "/home/nimda/Music/80s/Through_the_Fire.mp3";
//		mpd_send_add(m_connection, path);
	/*Recieve MPD SONG*/
//		printf("\033[0;32m>>GET mpd_recev_song\033[0m\n");
//		song = mpd_recv_song(m_connection);	//useless?
//		printf("\033[0;32m>>GOT mpd_recev_song\033[0m\n");
//
	/*IDK, Might be useful*/
//		printf("IDLE: %d\n", mpd_recv_idle(m_connection, false));
