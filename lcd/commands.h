void m_add(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);

	while (getchar() != '\n');
//	char *m_song_uri;
	char m_song_uri[256];
	printf("Add a song by URI: ");
	scanf("%[^\n]", m_song_uri);
	if (mpd_send_add(conn, m_song_uri))
		printf("Added %s successfully\n", m_song_uri);
	else
		printf("Failed to add song: %s\n",
				mpd_connection_get_error_message(conn));
	mpd_command_list_end(conn);
}


void m_play(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_toggle_pause(conn);
	mpd_command_list_end(conn);
}

void m_next(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_next(conn);
	mpd_command_list_end(conn);
}

void m_prev(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_previous(conn);
	mpd_command_list_end(conn);
}


void m_update(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_update(conn, NULL);
	mpd_command_list_end(conn);
}

void m_list_all(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_list_all(conn, NULL);
	mpd_command_list_end(conn);
	m_print_songs(conn);
}

void m_list_queue(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_list_queue_meta(conn);
	mpd_command_list_end(conn);
	m_print_songs(conn);
}

void m_list_current(struct mpd_connection *conn)
{
	struct mpd_status *m_status;
	struct mpd_song *m_song;
	char *m_state_str;
	mpd_command_list_begin(conn, true);
	mpd_send_status(conn);
	mpd_send_current_song(conn);
	mpd_command_list_end(conn);
	m_status = mpd_recv_status(conn);
	switch (mpd_status_get_state(m_status))
	{
		case MPD_STATE_PLAY:	m_state_str = "Playing";	break;
		case MPD_STATE_PAUSE:	m_state_str = "Paused";		break;
		case MPD_STATE_STOP:	m_state_str = "Stopped";	break;
		default:		m_state_str = "ERROR";		break;
	}
	mpd_response_next(conn);

	m_song = mpd_recv_song(conn);

	printf("%s: %s\n", m_state_str, mpd_song_get_uri(m_song));
	mpd_response_finish(conn);
}

void m_vol_up(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_change_volume(conn, 5);
	mpd_command_list_end(conn);
}

void m_vol_down(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_change_volume(conn, -5);
	mpd_command_list_end(conn);
}

void m_quit(struct mpd_connection *conn)
{
	mpd_command_list_begin(conn, true);
	mpd_send_change_volume(conn, 5);
	mpd_command_list_end(conn);
}

void m_help()
{
		printf(	"commands:\n"
		"\ta	Add a song [WIP]\n"
		"\tp	Pause/Play the current playlist\n"
		"\t>	Next\n"
		"\t<	Previous\n"
		"\tc	List Queue\n"
		"\t+	Volume Up\n"
		"\t-	Volume Down\n"
		"\tu	Update Database\n"
		"\tl	List Database\n"
		"\th	Print this menu\n"
		"\tq	Quit\n"
		"\n"
		);

}


void m_print_songs(struct mpd_connection *conn)
{
	struct mpd_entity *entity;
	while ((entity = mpd_recv_entity(conn)) != NULL)
	{
		const struct mpd_song *song;
		const struct mpd_directory *dir;
		const struct mpd_playlist *pl;
		switch (mpd_entity_get_type(entity))
		{
			case MPD_ENTITY_TYPE_SONG:
				song = mpd_entity_get_song(entity);
				printf("SONG> %s\n", mpd_song_get_uri(song));
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
}
