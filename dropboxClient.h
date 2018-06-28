#ifndef DROPBOXCLIENT_HEADER
#define DROPBOXCLIENT_HEADER


//estrutura utilizada na sincroniza��o do diretorio sync dir
struct sync_data {
	char client_new[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como novos na sync_dit
	char server_new[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como novos no server
	char client_old[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como vlhos na sync_dit
	char server_old[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como velhos no server
};

//verifica se um nome est� em uma lista de nomes
int encontrou(char name[FILENAMESIZE],char name_list[FILENAMESIZE][MAXARQINDIR]);

//recebe um path inteiro para um arquivo e retorna apenas o nome do mesmo
//tulizada para uploads; enviar apenas o nome de um arquivo para o servidor
void pickFileNameFromPath(char *path,char *filename);

//retorna o path do diret�rio sync_dir_user do usu�rio logado
char* devolvePathSyncDirBruto();

//pega o pr�ximo arquivo dentre os arquivos recebidos por um list_server
char* findnext(char* list_server,int contador, int * contstr);

//usada para alterar o tempo de gap entre 2 syncs consecutivos
void setsynctime(int newsynctime);

//enviar� pedido de login para o servidor e efetuar� login do cliente
int login_server(char *host,int port);

//relizar� o download de um arquivo de sync para finalPath
void get_file(char *filename, char * finalpath);

//realizar� o upload de um arquivo para o server e para sync
void send_file(char *file);

//deleta um arquivo do servidor e do sync
void delete_file(char *filename);

//realiza a sincronizza��o da sync_dir com o servidor
void executaSync(struct sync_data syncdata);

//realiza a primeira execu��o da sync (est� execu��o verifica o que j� est� no server e deixa oeste cliente no mesmo estado que o server
void firstExecutaSync(struct sync_data syncdata);
void first_sync_client();
void sync_client();

//encerra sess�o do cliente
void close_session();

//chama a list server para receber a lista de arquivos do servidor
char* list_server();

//liste os arquivos de sync dir
void list_client();

//thread que realiza a sincroniza��o
void* thread_sync(void *vargp);


#endif
