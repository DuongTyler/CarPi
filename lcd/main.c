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
#define DISP_RST		ESC"[0m"
#define DISP_WARN		ESC"[0;31m[!!!] "
#define DISP_GREEN	ESC"[0;32m"
#define DISP_NOTIFY		ESC"[0;33m==> "
#define DISP_PURPLE	ESC"[0;35m"
#define DISP_BLUE	ESC"[0;34m"
#define DISP_BLINK

int main()
{
	struct mpd_connection	*m_connection;
	struct mpd_status	*m_status;
	struct mpd_song 	*song;
	struct mpd_entity 	*entity;

	/***CONNECT TO DAEMON***/
	printf( DISP_NOTIFY"Connecting to Music Player Daemon..."DISP_RST"\n" );
	m_connection = mpd_connection_new( NULL, 0, 30000 );

	while ( 1 )
	{
		/***HANDLE ERRORS***/
		if ( mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS )
		{
			fprintf( stderr, DISP_WARN"Could not connect to MPD: %s"DISP_RST"\n", 
					mpd_connection_get_error_message( m_connection ));
        		mpd_connection_free( m_connection );
			m_connection = NULL;
			return 1;
		}
		
		char *m_state_str;
		char command;

		

		/***GET INPUT FROM USER***/		
		
		printf(	"Send Command: ");
		scanf(	"%c", &command	);
		
		dbgprint( DISP_NOTIFY"ENTER Switch with %c"DISP_RST"\n", command );

		mpd_command_list_begin(m_connection, true);
		
		switch( command )
		{
			//case 'a':	//add
				//TODO: Finish add song to playlist function
			//	mpd_send_add()
			//	break;
			case 'p':	//pause/play
				dbgprint( DISP_NOTIFY"Sending Pause/Play"DISP_RST"\n" );
				mpd_send_toggle_pause( m_connection );
				break;
			case '>':	//next
				mpd_send_next( m_connection );
				break;
			case '<':	//previous
				mpd_send_previous( m_connection );
				break;
			case 'u':	//update
				mpd_send_update( m_connection, NULL );	//NULL updates the entire DB
				break;
			case 'l':	//list
				//TODO: Finish list function
				mpd_send_list_all( m_connection, NULL );
				break;
			case 'h': case '?':
				printf(		"commands:\n"
						"a	Add a song [WIP]\n"
						"p	Pause/Play the current playlist\n"
						">	Next\n"
						"<	Previous\n"
						"u	Update Database\n"
						"l	List Database\n"
						"h	Print this menu\n"
						"q	Quit\n"
						);
				while( getchar() != '\n' );	//flush stream
				mpd_command_list_end(m_connection);
				mpd_response_finish(m_connection);
				continue;
			case 'q':	//quit
				printf( "EXITING\n" );	
				if ( m_connection != NULL ) 	mpd_connection_free(m_connection);
				return 0;

			default:
				while( getchar() != '\n' );	//flush stream
				printf( DISP_WARN"Invalid Command"DISP_RST"\n" );
				mpd_command_list_end(m_connection);
				mpd_response_finish(m_connection);
				continue;
		}

		mpd_send_status(m_connection);
		mpd_send_current_song(m_connection);
		
		mpd_command_list_end(m_connection);
		//end of command list
		
		//for list command
		while ((entity = mpd_recv_entity(m_connection)) != NULL)
		{
			const struct mpd_song *listSong;
			const struct mpd_directory *dir;
			const struct mpd_playlist *pl;
			switch (mpd_entity_get_type(entity))
			{
				case MPD_ENTITY_TYPE_SONG:
					listSong = mpd_entity_get_song(entity);
					printf("SONG> %s\n", mpd_song_get_uri(listSong));
					break;
				case MPD_ENTITY_TYPE_DIRECTORY:
					dir = mpd_entity_get_directory(entity);
					printf("DIR> %s\n", mpd_directory_get_path(dir));
					break;
				case MPD_ENTITY_TYPE_PLAYLIST:
					pl = mpd_entity_get_playlist(entity);
					printf("PL> %s\n", mpd_playlist_get_path(pl));
					break;
			}
			mpd_entity_free(entity);
		}
		//goes to next command to send after the switch command sends (send_status)
		mpd_response_next(m_connection);
		
		/***UPDATE THE STATUS***/
		m_status = mpd_recv_status( m_connection );	
		if(m_status == NULL)	
		{
			dbgprint(DISP_WARN"NULL m_status"DISP_RST"\n");
			dbgprint(DISP_WARN"MPD: %s\n",mpd_connection_get_error_message(m_connection));
			continue;
		}

		switch( mpd_status_get_state( m_status ) )		
		{
			case MPD_STATE_PLAY:	m_state_str = DISP_PURPLE"playing"DISP_RST;	break;
			case MPD_STATE_PAUSE:	m_state_str = DISP_PURPLE"paused"DISP_RST;	break;
			case MPD_STATE_STOP:	m_state_str = DISP_PURPLE"stopped"DISP_RST;	break;
			default:		m_state_str = DISP_WARN"FAILED"DISP_RST;	break;
		}

		/***PRINT MPD STATUS***/
		dbgprint( DISP_BLUE"MPD state: %s"DISP_RST"\n", m_state_str );

		//go to next command in the list (send_current_song)
		mpd_response_next(m_connection);

		song = mpd_recv_song(m_connection);
		printf("Current Song: %s\n", mpd_song_get_uri(song));

		mpd_response_finish(m_connection);

		/***CLEAN UP***/
		while( getchar() != '\n' );	//flush stream
		if (m_status != NULL)	mpd_status_free( m_status );
		if (song != NULL)	mpd_song_free(song);
//		if (entity != NULL)	mpd_entity_free( entity );
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
