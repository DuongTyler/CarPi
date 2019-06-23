	/***MEMO***/
//compile with g++/gcc and link mpdclient with -lmpdclient
//use -g to debug with gdb

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpd/client.h>

#define DEBUG 1

#ifdef DEBUG
#define dbgprint(...) printf(__VA_ARGS__)
#else
#define dbgprint(...)
#endif

#define ESC "\033"
#define DISP_NORM  ESC"[0m"
#define DISP_RED   ESC"[0;31m"
#define DISP_GREEN ESC"[0;32m"
#define DISP_BROWN ESC"[0;33m"
#define DISP_BLUE  ESC"[0;34m"

int main()
{
	struct mpd_connection	*m_connection;
	struct mpd_status	*m_status;
	struct mpd_song 	*song;

	/***CONNECT TO DAEMON***/
	printf( DISP_BROWN"Connecting to Music Player Daemon..."DISP_NORM"\n" );
	m_connection = mpd_connection_new( NULL, 0, 30000 );
	printf( DISP_GREEN"Connection Suceeded"DISP_NORM"\n" );

	while ( 1 )
	{
		/***HANDLE DISCONNECTION***/
		if ( mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS )
		{
			fprintf( stderr, DISP_RED"Could not connect to MPD: %s"DISP_NORM"\n", 
					mpd_connection_get_error_message( m_connection ));
        		mpd_connection_free( m_connection );
			m_connection = NULL;
			return 1;
		}

		/***VARIABLES***/
		char *m_state_str;
		char command;

		/***GET INPUT FROM USER***/		
		printf(	"Send Command: ");
		scanf(	"%c", &command	);
		
		dbgprint( DISP_GREEN">>ENTER Switch"DISP_NORM"\n" );
		switch( command )
		{
			case 'p':
				dbgprint( DISP_GREEN"Sending Pause/Play"DISP_NORM"\n" );
				mpd_send_toggle_pause( m_connection );
				break;
			case 'q':
				printf( "EXITING\n" );	
				if ( m_connection != NULL ) 	mpd_connection_free(m_connection);
				return 0;

			default:
				while( getchar() != '\n' );	//flush stream
				printf( DISP_RED"Invalid Command"DISP_NORM"\n" );
				continue;
		}

		/***UPDATE THE STATUS***/
		dbgprint( DISP_BROWN">>GET mpd_recv_status"DISP_NORM"\n" );
		m_status = mpd_recv_status( m_connection );	//TODO: fix wrong values returned by mpd_recv_status()

		dbgprint( DISP_BROWN">>GET mpd_status_get_state\n"DISP_NORM );
		switch( mpd_status_get_state( m_status ) )		
		{
			case MPD_STATE_PLAY:	m_state_str = "playing";			break;
			case MPD_STATE_PAUSE:	m_state_str = "paused";				break;
			case MPD_STATE_STOP:	m_state_str = "stopped";			break;
			default:		m_state_str = DISP_RED"FAILED"DISP_NORM;	break;
		}

		/***PRINT MPD STATUS***/
		dbgprint( DISP_BROWN"MPD state: %s"DISP_NORM"\n", m_state_str );

		/***CLEAN UP***/
		while( getchar() != '\n' );	//flush stream
		mpd_status_free( m_status );	//free status struct
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
