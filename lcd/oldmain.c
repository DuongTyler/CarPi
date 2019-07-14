	/***MEMO***/
//compile with g++/gcc and link mpdclient with -lmpdclient
//use -g to debug with gdb

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpd/client.h>
#include <string.h>

#include <signal.h>	//TODO: handle ctrl-c

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

void cleanup(struct mpd_connection *conn)
{
	while (getchar() != '\n');
	mpd_command_list_end(conn);
	mpd_response_finish(conn);
}

void printhelp()
{
	printf(	"commands:\n"
		"\ta	Add a song [WIP]\n"
		"\tp	Pause/Play the current playlist\n"
		"\t>	Next\n"
		"\t<	Previous\n"
		"\t+	Volume Up\n"
		"\t-	Volume Down\n"
		"\tu	Update Database\n"
		"\tl	List Database\n"
		"\th	Print this menu\n"
		"\tq	Quit\n"
		"\n"
		);
}

void addSong(struct mpd_connection *conn)
{
	while (getchar() != '\n');	//sometimes garbage is still in the instream
	char *m_song_uri;
	printf("Add a song by URI: ");
	scanf( "%[^\n]", m_song_uri );
	mpd_send_add(conn, m_song_uri);
	dbgprint("Added: %s\n", m_song_uri);
}

int main()
{
	struct mpd_connection	*m_connection;
	struct mpd_status	*m_status;
	struct mpd_song 	*song;
	struct mpd_entity 	*entity;
	struct sigaction	action;

	/***CONNECT TO DAEMON***/
	printf( DISP_NOTIFY"Connecting to Music Player Daemon..."DISP_RST"\n" );
	m_connection = mpd_connection_new( NULL, 0, 30000 );

	while ( 1 )
	{
		/***HANDLE ERRORS***/
//		if (sigaction (SIGINT, NULL, &action) < 0)
//		{
//			printf("SIGINT 1\n");
//		}
//		else if (action.sa_handler == SIG_DFL)
//		{
//			printf("SIGINT 2\n");
//		}

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

		if ( mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS )
		{
			fprintf( stderr, DISP_WARN"Could not connect to MPD: %s"DISP_RST"\n", 
					mpd_connection_get_error_message( m_connection ));
        		mpd_connection_free( m_connection );
			m_connection = NULL;
			return 1;
		}

		dbgprint( DISP_NOTIFY"ENTER Switch with %c"DISP_RST"\n", command );


		mpd_command_list_begin(m_connection, true);
		
		switch( command )
		{
				//TODO: Finish add song to playlist function
			case 'a':	addSong( m_connection );			break;	//add song to playing
			case 'p':	mpd_send_toggle_pause( m_connection );		break;	//pause/play
			case '>':	mpd_send_next( m_connection );			break;	//next
			case '<':	mpd_send_previous( m_connection );		break;	//prev
			case 'u':	mpd_send_update( m_connection, NULL );		break;	//update DB, NULL updates whole DB
			case 'l':	mpd_send_list_all( m_connection, NULL );	break;	//list, NULL lists whole DB
			case '+':	mpd_send_change_volume( m_connection, 5 );	break;	//vol +5
			case '-':	mpd_send_change_volume( m_connection, -5 );	break;	//vol -5
			case 'q':	mpd_connection_free( m_connection );		return 0;	//quit
					
			/***Special Commands***/
			case 'h': case '?':	//help menu
				printhelp();
				cleanup(m_connection);	//end command list, finish mpd response, flush instream
				continue;
			default:		//invalid command
				printf( DISP_WARN"Invalid Command"DISP_RST"\n" );
				cleanup(m_connection);	//end command list, finish mpd response, flush instream
				continue;
		}

		mpd_send_status(m_connection);
		mpd_send_current_song(m_connection);
		
		mpd_command_list_end(m_connection);
		
		//outputs DB content only if the 'l' command was called
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
			case MPD_STATE_PLAY:	m_state_str = "Playing"DISP_RST;		break;
			case MPD_STATE_PAUSE:	m_state_str = "Paused"DISP_RST;			break;
			case MPD_STATE_STOP:	m_state_str = "Stopped"DISP_RST;		break;
			default:		m_state_str = DISP_WARN"FAILED"DISP_RST;	break;
		}

		/***PRINT MPD STATUS***/
		//printf( DISP_BLUE"%s"DISP_RST"\n", m_state_str );

		//go to next command in the list (send_current_song)
		mpd_response_next(m_connection);

		song = mpd_recv_song(m_connection);
		printf(DISP_NOTIFY"%s: %s\n", m_state_str, mpd_song_get_uri(song));

		mpd_response_finish(m_connection);

		/***CLEAN UP***/
		while( getchar() != '\n' );	//flush stream
		if (m_status != NULL)	mpd_status_free( m_status );
		if (song != NULL)	mpd_song_free(song);
	}
	return 0;
}
