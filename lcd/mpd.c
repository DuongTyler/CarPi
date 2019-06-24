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

#define ESC		"\033"
#define RST_TXT		ESC"[0m"
#define WARN		ESC"[0;31m[!!!] "
#define STAT_OK		ESC"[0;32m"
#define NOTIFY		ESC"[0;33m==> "
#define DISP_BLUE	ESC"[0;34m"

int main()
{
	struct mpd_connection	*m_connection;
	struct mpd_status	*m_status;
	struct mpd_song 	*song;

	/***CONNECT TO DAEMON***/
	printf( NOTIFY"Connecting to Music Player Daemon..."RST_TXT"\n" );
	m_connection = mpd_connection_new( NULL, 0, 30000 );

	while ( 1 )
	{
		/***HANDLE DISCONNECTION***/
		if ( mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS )
		{
			fprintf( stderr, WARN"Could not connect to MPD: %s"RST_TXT"\n", 
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
		
		dbgprint( NOTIFY"ENTER Switch"RST_TXT"\n" );
		switch( command )
		{
			case 'a':
			//	mpd_send_
				break;
			case 'p':
				dbgprint( NOTIFY"Sending Pause/Play"RST_TXT"\n" );
				mpd_send_toggle_pause( m_connection );
				break;
			case 'q':
				printf( "EXITING\n" );	
				if ( m_connection != NULL ) 	mpd_connection_free(m_connection);
				return 0;

			default:
				while( getchar() != '\n' );	//flush stream
				printf( WARN"Invalid Command"RST_TXT"\n" );
				continue;
		}

		/***UPDATE THE STATUS***/
		dbgprint( NOTIFY"GET mpd_run_status"RST_TXT"\n" );
		//m_status = mpd_recv_status( m_connection );	//TODO: fix wrong values returned by mpd_recv_status()
		m_status = mpd_run_status( m_connection );	//TODO: fix wrong values returned by mpd_recv_status()
		
		if(m_status == NULL)	
		{
			dbgprint(WARN"NULL POINTER"RST_TXT"\n");
			dbgprint(NOTIFY"MPD: %s\n",mpd_connection_get_error_message(m_connection));
		}

		dbgprint( NOTIFY"GET mpd_status_get_state"RST_TXT"\n" );
		switch( mpd_status_get_state( m_status ) )		
		{
			case MPD_STATE_PLAY:	m_state_str = NOTIFY"playing"RST_TXT;	break;
			case MPD_STATE_PAUSE:	m_state_str = NOTIFY"paused"RST_TXT;	break;
			case MPD_STATE_STOP:	m_state_str = NOTIFY"stopped"RST_TXT;	break;
			default:		m_state_str = WARN"FAILED"RST_TXT;	break;
		}

		/***PRINT MPD STATUS***/
		dbgprint( DISP_BLUE"MPD state: %s"RST_TXT"\n", m_state_str );

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
