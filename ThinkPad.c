#include <netinet/in.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <dirent.h>
#include <pthread.h>
#include "utils/SocketLib.h"
#include "utils/Constants.h"
#include "utils/FileLib.h"


void *peer_request_handler(void* arg);
int main(int argc, char const *argv[]) {
	int server_socket = create_server_socket(SERVER_PORT);

	int client_socket = accept(server_socket, NULL, NULL);  
	if (client_socket < 0)  {  
	            printf("ThinkPad: Server Accept Failed!\n");  
	            return 0;  
	} 
	printf("ThinkPad: Connected to IdeaPad\n");
	pthread_t peer_request_thread;
	if (pthread_create(&peer_request_thread, NULL, peer_request_handler, &client_socket)) {
		printf("ThinkPad: Error in pthread_create()\n");
		return 0;
	}

	while (1) {
		char command[100];
		scanf("%s", command);
		if (strcmp(command, "exit") == 0) {
			break;
		} else if(strcmp(command, "send") == 0) {
			DIR * dir_ptr;
			struct dirent * direntp;
			if ((dir_ptr = opendir(THINKPAD_PATH)) == NULL)
				printf("ThinkPad: Connot open %s\n", THINKPAD_PATH);
			else {
				while ((direntp = readdir(dir_ptr)) != NULL) {
					if (direntp->d_name[0] != '.' && direntp->d_name[strlen(direntp->d_name)-1] != '~' && direntp->d_type == FILE_TYPE) {
						sendFileName(direntp->d_name, client_socket);
						// printf("ThinkPad: sendFileName %s\n", direntp->d_name);
						char file_path[100];
						sprintf(file_path, "%s/%s", THINKPAD_PATH, direntp->d_name);
						sendFile(file_path, client_socket);
						printf("ThinkPad: sendFile %s\n", file_path);
					}
				}
				closedir(dir_ptr);
			}
		}
	}
	close(client_socket);
	close(server_socket);
	return 0;
}

void *peer_request_handler(void* arg){
	int client_socket = *((int *)arg);
	printf("ThinkPad: peer_request_handler set up\n");
	while (1) {
		char file_name[FILE_NAME_MAX_SIZE + 1];  
		recvFileName(file_name,client_socket);
		// printf("ThinkPad: recvFileName %s\n", file_name);
		char file_path[FILE_NAME_MAX_SIZE+1];
		sprintf(file_path,"%s/%s",THINKPAD_PATH,file_name);
		recvFile(file_path, client_socket);
		printf("ThinkPad: recvFile %s\n", file_path);
	}
	pthread_exit(NULL);
}