#include <netinet/in.h>                         // for sockaddr_in  
#include <sys/types.h>                          // for socket  
#include <sys/socket.h>                         // for socket  
#include <stdio.h>                              // for printf  
#include <stdlib.h>                             // for exit  
#include <string.h>                             // for bzero  
#include "Constants.h"
#include "FileLib.h"

void sendFileName(char* file_name, int client_socket){
	char buffer[BUFFER_SIZE];  
	bzero(buffer, sizeof(buffer));  
	strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));  
	send(client_socket, buffer, BUFFER_SIZE, 0);  
}
void recvFileName(char* file_name, int server_socket){
	char buffer[BUFFER_SIZE];  
	bzero(buffer, sizeof(buffer));  
	socklen_t length = recv(server_socket, buffer, BUFFER_SIZE, 0);  
	if (length < 0)  {  
		printf("Server Recieve Data Failed!\n");  
		exit(1);  
	}  
	bzero(file_name, sizeof(file_name));  
	strncpy(file_name, buffer,  strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));
}
void sendFile(char* file_name,int server_socket){
	char buffer[BUFFER_SIZE];
	FILE *fp = fopen(file_name, "r");  
	if (fp == NULL)  {  
		printf("File:\t%s Not Found!\n", file_name);  
	}  
	else  
	{  
		bzero(buffer, BUFFER_SIZE);  
		int file_block_length = 0;  
		while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {  
			printf("file_block_length = %d\n", file_block_length);  
			if (send(server_socket, buffer, file_block_length, 0) < 0) {  
				printf("Send File:\t%s Failed!\n", file_name);  
				break;  
			}  
			bzero(buffer, sizeof(buffer));  
		}  
		fclose(fp);  
		printf("File:\t%s Transfer Finished!\n", file_name);  
	}
}
void recvFile(char* file_name, int client_socket){
	FILE *fp = fopen(file_name, "w");  
	if (fp == NULL)  {  
		printf("recvFile(): File:\t%s Can Not Open To Write!\n", file_name);  
		exit(1);  
	}  
	char buffer[BUFFER_SIZE]; 
	bzero(buffer, sizeof(buffer));  
	int length = 0;  
	while(length = recv(client_socket, buffer, BUFFER_SIZE, 0))  {  
		if (length < 0)  {  
			printf("recvFile(): Recieve Data From Server %s Failed!\n", SERVER_IP);  
			break;  
		}  

		int write_length = fwrite(buffer, sizeof(char), length, fp);  
		if (write_length < length)  {  
			printf("recvFile(): File:\t%s Write Failed!\n", file_name);  
			break;  
		}  
		bzero(buffer, BUFFER_SIZE);  
	}  
	printf("recvFile(): Recieve File:\t %s From Server[%s] Finished!\n", file_name, SERVER_IP);  
	fclose(fp);  
}
