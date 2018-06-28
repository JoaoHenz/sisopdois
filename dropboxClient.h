#ifndef DROPBOXCLIENT_HEADER
#define DROPBOXCLIENT_HEADER

struct sync_data {
	char client_new[FILENAMESIZE][MAXARQINDIR];
	char server_new[FILENAMESIZE][MAXARQINDIR];
	char client_old[FILENAMESIZE][MAXARQINDIR];
	char server_old[FILENAMESIZE][MAXARQINDIR];
};

int encontrou(char name[FILENAMESIZE],char name_list[FILENAMESIZE][MAXARQINDIR]);
void pickFileNameFromPath(char *path,char *filename);
char* devolvePathSyncDirBruto();
char* findnext(char* list_server,int contador, int * contstr);
void setsynctime(int newsynctime);
int login_server(char *host,int port);
void get_file(char *filename, char * finalpath);
void send_file(char *file);
void delete_file(char *filename);
void executaSync(struct sync_data syncdata);
void firstExecutaSync(struct sync_data syncdata);
void first_sync_client();
void sync_client();
void close_session();
char* list_server();
void list_client();
void* thread_sync(void *vargp);


#endif
