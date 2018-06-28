#ifndef DROPBOXSERVER_HEADER
#define DROPBOXSERVER_HEADER

//estrutura que define um cliente do servidor
struct client {
	char user_id [MAXNAME];     //id do cliente
	int session_active [MAXSESSIONS];   //vetor com todas as sess�es atiivas (dispositivos conectados) deste cliente
	short int session_port [MAXSESSIONS];  //porta que o cliente estarpa utilizando para comunica��o
	int socket_set[MAXSESSIONS];       //soket da sess�o
	SOCKET socket[MAXSESSIONS];
};
struct pair {
	int c_id;
	int s_id;
};
//informa��es tulizadas para r�plica de upload aos servidores servos
struct upload_info {
	char filename[MAXNAME];   //nome do arqivo a ser enviado
	int session_port;        //porta pela qual o arquivo ser� enviado
	char userID[MAXNAME];    //id do cliente que mandou o arquivo
};

<<<<<<< HEAD
struct login_pair {
	struct sockaddr_in client_addr;
	char userID[MAXNAME];
};

//gera o path em dropboxserver para um cliente userID
char * devolvePathHomeServer(char *userID);

//retorna 0 se o cliente index est� ativo em algum dispositivo, 1 caso contr�rio
int inactive_client(int index);

//atualiza o �ndice de um cliente na lista de clientes
int identify_client(char user_id [MAXNAME], int* client_index);

//emvia um arquivo de dropboxserver
void send_file(char *file, int socket, char *userID, struct sockaddr client_addr);

//recebe um arquivo em dropboxserver
void receive_file(char *file, int socket, char*userID);

//deleta um arquivo de dropboxserver
int delete_file(char *file, int socket, char*userID);

//lista os arqivos em dropboxserver
void list_files(SOCKET socket, struct sockaddr client, char *userID);

//informa ao front end dos clientes qual o novo servidor prim�rio ap�s uma elei��o
int inform_frontend(struct sockaddr_in client, SOCKET session_socket);

//realiza a r�plica de uploads para server secund�rios
void *replica_upload(void *args);

//gerencia as sess�es de usu�rios; recebe todas as opera��es e as manipula
void *session_manager(void* args);

//loga um cliente no servidor, alocando um socket e uma porta para o mesmo
int login(struct packet login_request);

//realiza a sincroniza��o do sync dir de um cliente
void* sync_server_manager();

//informa a resposta de uma elei��o
void* election_answer();

//verifica o estado das replicas dos servidores; envia pings e lan�a elei��o quando percebe timeout
=======
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
>>>>>>> 899c1fcf34e943dc359b0408acb4f079dd5bd1b4
void *replica_manager();

#endif
