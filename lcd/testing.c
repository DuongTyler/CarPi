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
	mpd_command_list_begin(m_connection, true);
	mpd_send_status(m_connection);
	mpd_send_current_song(m_connection);
	mpd_command_list_end(m_connection);
	m_status = mpd_recv_status(m_connection);
	
	if (m_status == NULL)	return 1;
	printf("Position: %i\n", mpd_status_get_elapsed_time(m_status));
	mpd_status_free(m_status);
	mpd_response_next(m_connection);
	song = mpd_recv_song(m_connection);
	printf("SONG: %s\n", mpd_song_get_uri(song));
	mpd_song_free(song);

	return 0;
}
