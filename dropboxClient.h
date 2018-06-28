#ifndef DROPBOXCLIENT_HEADER
#define DROPBOXCLIENT_HEADER


//estrutura utilizada na sincronização do diretorio sync dir
struct sync_data {
	char client_new[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como novos na sync_dit
	char server_new[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como novos no server
	char client_old[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como vlhos na sync_dit
	char server_old[FILENAMESIZE][MAXARQINDIR];  //orquivos que o cliente percebe como velhos no server
};

//verifica se um nome está em uma lista de nomes
int encontrou(char name[FILENAMESIZE],char name_list[FILENAMESIZE][MAXARQINDIR]);

//recebe um path inteiro para um arquivo e retorna apenas o nome do mesmo
//tulizada para uploads; enviar apenas o nome de um arquivo para o servidor
void pickFileNameFromPath(char *path,char *filename);

//retorna o path do diretório sync_dir_user do usuário logado
char* devolvePathSyncDirBruto();

//pega o próximo arquivo dentre os arquivos recebidos por um list_server
char* findnext(char* list_server,int contador, int * contstr);

//usada para alterar o tempo de gap entre 2 syncs consecutivos
void setsynctime(int newsynctime);

//enviará pedido de login para o servidor e efetuará login do cliente
int login_server(char *host,int port);

//relizará o download de um arquivo de sync para finalPath
void get_file(char *filename, char * finalpath);

//realizará o upload de um arquivo para o server e para sync
void send_file(char *file);

//deleta um arquivo do servidor e do sync
void delete_file(char *filename);

//realiza a sincronizzação da sync_dir com o servidor
void executaSync(struct sync_data syncdata);

//realiza a primeira execução da sync (está execução verifica o que já está no server e deixa oeste cliente no mesmo estado que o server
void firstExecutaSync(struct sync_data syncdata);
void first_sync_client();
void sync_client();

//encerra sessão do cliente
void close_session();

//chama a list server para receber a lista de arquivos do servidor
char* list_server();

//liste os arquivos de sync dir
void list_client();

//thread que realiza a sincronização
void* thread_sync(void *vargp);


#endif
