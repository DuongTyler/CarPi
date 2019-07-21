#include <stdio.h>
#include <unistd.h>
#include <mpd/client.h>

int main(void) {
	struct mpd_connection* m_connection = NULL;
	struct mpd_status* m_status = NULL;
	char* m_state_str;
	
	m_connection = mpd_connection_new(NULL, 0, 30000);

	while (1) {
	// this check works only on start up (i.e. when mpd_connection_new failed),
	//         // not when the connection is lost later
					    
		if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
			fprintf(stderr, "Could not connect to MPD: %s\n", mpd_connection_get_error_message(m_connection));
                        mpd_connection_free(m_connection);
                        m_connection = NULL;
                }
					    
		m_status = mpd_run_status(m_connection);
		if (mpd_status_get_state(m_status) == MPD_STATE_PLAY) {
		        m_state_str = "playing";
		} else if (mpd_status_get_state(m_status) == MPD_STATE_STOP) {
		        m_state_str = "stopped";
		} else if (mpd_status_get_state(m_status) == MPD_STATE_PAUSE) {
		        m_state_str = "paused";
		} else {
		        m_state_str = "unknown";
		}	
		printf("MPD state: %s\n", m_state_str);
		sleep(1);			      
	}
}
