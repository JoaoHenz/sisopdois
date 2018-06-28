#ifndef DROPBOXSERVER_HEADER
#define DROPBOXSERVER_HEADER


struct client {
	char user_id [MAXNAME];
	int session_active [MAXSESSIONS];
	short int session_port [MAXSESSIONS];
	int socket_set[MAXSESSIONS];
	SOCKET socket[MAXSESSIONS];
};
struct pair {
	int c_id;
	int s_id;
};
struct upload_info {
	char filename[MAXNAME];
	int session_port;
	char userID[MAXNAME];
};

char * devolvePathHomeServer(char *userID);
int inactive_client(int index);
int identify_client(char user_id [MAXNAME], int* client_index);
void send_file(char *file, int socket, char *userID, struct sockaddr client_addr);
void receive_file(char *file, int socket, char*userID);
int delete_file(char *file, int socket, char*userID);
void list_files(SOCKET socket, struct sockaddr client, char *userID);
int inform_frontend(struct sockaddr client, SOCKET session_socket);
void *replica_upload(void *args);
void *session_manager(void* args);
int login(struct packet login_request);
void* sync_server_manager();
void* election_answer();
void *replica_manager();

#endif
