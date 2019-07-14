	/***MEMO***/
//compile with g++/gcc and link mpdclient with -lmpdclient
//use -g to debug with gdb

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpd/client.h>
#include <string.h>
#include "commands.h"

#include <signal.h>

//#define DEBUG 1

#ifdef DEBUG
#define dbgprint(...) printf(__VA_ARGS__)
#else
#define dbgprint(...)
#endif

#define ESC			"\033"
#define DISP_RST		ESC"[0m"
#define DISP_WARN		ESC"[0;31m[!!!] "
#define DISP_GREEN		ESC"[0;32m"
#define DISP_NOTIFY		ESC"[0;33m==> "
#define DISP_PURPLE		ESC"[0;35m"
#define DISP_BLUE		ESC"[0;34m"
#define DISP_BLINK

int main()
{
	struct mpd_connection	*m_connection;

	//signal(SIGPIPE, SIG_IGN); //this will cause worse things to happen
	//signal(SIGINT, m_sigint);
	/***CONNECT TO DAEMON***/
	printf( DISP_NOTIFY"Connecting to Music Player Daemon..."DISP_RST"\n" );
	m_connection = mpd_connection_new( NULL, 0, 60000 );

	while ( 1 )
	{
		if ( mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS )
		{
			fprintf( stderr, DISP_WARN"Could not connect to MPD: %s"DISP_RST"\n", 
					mpd_connection_get_error_message( m_connection ));
        		mpd_connection_free( m_connection );
			m_connection = NULL;
			//return 1;
			m_connection = mpd_connection_new( NULL, 0, 60000 );
		}
		
		char *m_state_str;
		char command;

		/***GET INPUT FROM USER***/		
		printf(	"Send Command: ");
		scanf(	"%c", &command	);

		dbgprint( DISP_NOTIFY"ENTER Switch with %c"DISP_RST"\n", command );
		
		switch( command )
		{
			case 'a':	m_add( m_connection );			break;	//add song to playing
			case 'p':	m_play( m_connection );			break;	//pause/play
			case '>':	m_next( m_connection );			break;	//next
			case '<':	m_prev( m_connection );			break;	//prev
			case 'u':	m_update( m_connection );		break;	//update DB, NULL updates whole DB
			case 'l':	m_list_all( m_connection );		break;	//list, NULL lists whole DB
			case 'c':	m_list_current( m_connection );		break;	//list queue
			case 'i':	m_list_queue( m_connection );		break;	//list queue
			case '+':	m_vol_down( m_connection );		break;	//vol +5
			case '-':	m_vol_up( m_connection );		break;	//vol -5
			case 'q':	m_quit( m_connection );			return 0;	//quit
					
			/***Special Commands***/
			case 'h': case '?':	m_help();					break;
			default:	printf( DISP_WARN"Invalid Command"DISP_RST"\n" );	break;
		}
		dbgprint("exit switch\n");
		mpd_response_finish( m_connection );
		/***CLEAN UP***/
		while( getchar() != '\n' );	//flush stream
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
