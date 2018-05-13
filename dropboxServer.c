#ifndef DROPBOXSERVER_C
#define DROPBOXSERVER_C

#define SOCKET int
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "dropboxUtils.h"

#define MAIN_PORT 6000

// Structures
struct file_info {
  char name[MAXNAME];
  char extension[MAXNAME];
  char last_modified[MAXNAME];
  int size;
};

struct client {
  int device[2];
  char userid [MAXNAME];
  struct file_info info [MAXFILES];
  int logged_in;
};

struct session {
	int client_id;
	int client_address_len;
	struct sockaddr client_address;
	char session_buffer[1250];
};

// Global Variables
struct client client_list[10];
struct session session_list[20];
int client_active[10];
int session_active[20];
int session_counter = 0;

// Auxiliary Functions
int min(int right, int left){
	if(right > left){
		return left;
	}
	else{
		return right;
	}
}

int get_session_spot(){
	int i;
	for (i = 0; i < 20; i++){
		if(session_active[i] == 0){
			return i;
		}
	}
	return -1;
}

int get_sequence_num(char packet_buffer[1250]){
	char aux_str[5];
	int i, value;
	for(i = 0; i < 4; i++){
		aux_str[i] = packet_buffer[6+i];
	}
	aux_str[4] = '\0';
	value = *(int *) aux_str;
	printf("Val is: %d", value);
	return value;
}

int socket_cmp(struct session *left, struct session *right){
    socklen_t min_address_len = min(left->client_address_len, right->client_address_len);
    // If head matches, longer is greater.
    int default_rv = right->client_address_len - left->client_address_len;
    int rv = memcmp(left, right, min_address_len);
    return rv ?: default_rv;
}

int redirect_package(char packet_buffer[1250], struct sockaddr client, int client_len){
	int i;
	struct session *new_session;
	new_session->client_address = client;
	new_session->client_address_len = client_len;
	for(i = 0; i < 20; i++){
		if (socket_cmp(&(session_list[i]),new_session)== 0){
			strcpy(session_list[i].session_buffer,packet_buffer);
			return 0;
		}
	}
 	return 1;
}

void *session_manager(void *args){
	struct session *client_session;
	client_session = (struct session *) args;
	printf("Client ID is: %d\n\n", client_session->client_id);
}

int login(char packet_buffer[1250], struct sockaddr client, int client_len){
 	char aux_username[20];
	struct client new_client;
	struct session new_session;
	pthread_t tid;
	int i, not_done = 1, aux_index;
	// Verify client list
	for (i = 0; i < 20; i++){
		aux_username[i] = packet_buffer[10+i];
	}
	aux_index = get_session_spot();
	if (aux_index == -1){
		return 0;
	}
	create_home_dir(aux_username);
	for(i = 0; i < 10; i++){
		if(strcmp(client_list[i].userid,aux_username) == 0){
			if(client_list[i].logged_in == 0){
				return 0;
			}
			else{			
				client_list[i].logged_in = 0; // Update logged_in
				new_session.client_id = i; 
				new_session.client_address = client;
				new_session.client_address_len = client_len;
				session_list[aux_index] = new_session;
				printf("New session created, added device to active client:\n");
				pthread_create(&tid, NULL, session_manager, &new_session);	
				return 1;
			}
		}
	}
	for(i = 0; i < 10; i++){
		if(client_active[i] == 0){
			strcpy(new_client.userid, aux_username); // Set userid
			new_client.logged_in = 1; // Set logged_in
			client_list[i] = new_client; // Place new client into the client list
			new_session.client_id = i; // Inform the session of the client's index in the list
			new_session.client_address = client;
			new_session.client_address_len = client_len;
			session_list[aux_index] = new_session;
			printf("New session created:\n");
			pthread_create(&tid, NULL, session_manager, &new_session);
			return 1;
		}
	}
}

// Specification subroutines
void sync_server(){

}

void send_file(char *file){

}

void receive_file(char *file){

}

// Server's main thread
int main(int argc,char *argv[]){
	struct sockaddr_in server;
	struct sockaddr client;
	SOCKET s_socket;
	int server_len, received, i, online = 1, client_len = sizeof(struct sockaddr_in), login_value, seq_num;
	char packet_buffer[1250];
	char reply_buffer[1250];
	char ack_buffer[11];
	char op_code[7];

	// Initializing client list (temporary measure - until we get a user list file)
	for(i = 0; i < 10; i++){
		struct client new_client;
		client_list[i] = new_client;
		client_list[i].logged_in = 2;
		strcpy(client_list[i].userid," ");
		client_active[i] = 0;
	}
	for (i = 0; i < 20; i++){
		session_active[i] = 0;
	}

	if((s_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("ERROR: Socket creation failure.\n");
		exit(1);
	}
/*	
	Configures domain, IP (set to any) and port (set to MAIN_PORT)
	Binds s_socket to the client sockaddr_in structure
*/
	 memset((void *) &server,0,sizeof(struct sockaddr_in));
	 server.sin_family = AF_INET;
	 server.sin_addr.s_addr = htonl(INADDR_ANY);
	 server.sin_port = htons(MAIN_PORT); 
	 server_len = sizeof(server);
	 if(bind(s_socket,(struct sockaddr *) &server, server_len)) {
		  printf("Binding error\n");
		  exit(1);
	 }
	printf("Socket initialized, waiting for requests.\n\n");
/*
	Main loop - receives packages and either 
		1) redirects them to a session thread, where:
			- it updates the folder (receive_file or sync are called)
			- it sends a file (sync_client or send_file are called)
		2) uses their info to create a new session thread
*/
	while(online){
		received = recvfrom(s_socket,packet_buffer,sizeof(packet_buffer),0,(struct sockaddr *) &client,(socklen_t *)&client_len);
		if (!received){
			printf("ERROR: Package reception error.\n\n");
		}
		strncpy(op_code,packet_buffer,6);
		op_code[6] = '\0';
		if (strcmp(op_code,"logins") == 0){
			if (login(packet_buffer, client, client_len)){
				strncpy(ack_buffer,"ACKACK",6);
				for(i = 0; i < 4; i++){
					ack_buffer[6+i] = packet_buffer[6+i];
				}
				ack_buffer[10] = '\0';
				sendto(s_socket,ack_buffer,sizeof(ack_buffer),0,(struct sockaddr *)&client, client_len);
				printf("Login succesful...\n");				
			}
			else{
				strncpy(ack_buffer,"NOTACK",6);
				for(i = 0; i < 4; i++){
					ack_buffer[6+i] = packet_buffer[6+i];
				}
				ack_buffer[10] = '\0';
				sendto(s_socket,ack_buffer,sizeof(ack_buffer),0,(struct sockaddr *)&client, client_len);
				printf("ERROR: Login unsuccesful...\n");
			}
		}
		else{
			printf("Redirecting packet to session manager...");
			if (redirect_package(packet_buffer, client, client_len)){
				//seq_num = get_sequence_num(packet_buffer);
				strncpy(ack_buffer,"NOTACK",6);
				for(i = 0; i < 4; i++){
					ack_buffer[6+i] = packet_buffer[6+i];
				}
				ack_buffer[10] = '\0';
				sendto(s_socket,ack_buffer,sizeof(ack_buffer),0,(struct sockaddr *)&client, client_len);
				printf("ERROR: Packet delivery unsuccesful...\n");
			}
		}
		// clear packet and auxiliaries
		memset(packet_buffer,0,1250);
		memset(op_code,0,7);
		//TIME TO RESPOND - check send queue and do as appropriate:
	}
	return 0;
}


#endif
