#ifndef DROPBOXUTILS_HEADER
#define DROPBOXUTILS_HEADER

#include<string.h>
#include<stdlib.h>
#include <sys/socket.h>

#define SOCKET int
#define MAXNAME 20
#define MAXFILES 10
#define CHUNK 1240
#define OPCODE 10
#define TRUE 1
#define FALSE 0
#define PACKETSIZE 1250
#define SOCKET int
#define MAXCLIENTS 10
#define MAXSESSIONS 2
#define NACK 0
#define ACK 1
#define UPLOAD 2
#define DOWNLOAD 3
#define DELETE 4
#define LIST 5
#define CLOSE 6
#define LOGIN 7
#define FILEPKT 8
#define LASTPKT 9
#define PING 10
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define FILENAMESIZE 50
#define MAXARQINDIR 30


//definição do packet que é utilizado na comunicação
struct packet {
	short int opcode;   //opcode informa qual a qual operação este packet é referente. como upload ping login
	short int seqnum;   // número de sequencia do packet
	char data [PACKETSIZE - 4];   //área de dados do packet
};
struct file_info{
	char name[MAXNAME];
	char extension[MAXNAME];
	char last_modified[MAXNAME];
	int size;
};
struct login_data{
	char userID[20];
	struct sockaddr_in adress;
}

void removeBlank(char * filename);

//cria o diretorio do user syn_dir_user
int create_home_dir(char *userID);

//cria o diretório de user dentro de dropboxserver
int create_home_dir_server(char *userID);

//recebe um comando do nosso shell e retorna qual o argumento (path normalmente)
char * getArgument(char* command);
char * getSecondArgument(char* command);

//cria a pasta dropboxserver quando o servidor é iniciado
int create_server_root();
int create_server_userdir(char *userID);
int receive_int_from(int socket);
int send_int_to(int socket, int op);
char* receive_string_from(int socket);
int send_string_to(int socket, char* str);

//funções de envio e recebimento de arquivo via os sockets
int receive_file_from(int socket, char* filename);
int send_file_to(int socket, char* filepath, struct sockaddr destination);

#endif
