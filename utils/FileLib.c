#include <netinet/in.h>                         // for sockaddr_in  
#include <sys/types.h>                          // for socket  
#include <sys/socket.h>                         // for socket  
#include <stdio.h>                              // for printf  
#include <stdlib.h>                             // for exit  
#include <string.h>                             // for bzero  
#include "Constants.h"
#include "FileLib.h"

void sendFileName(char* file_name, int client_socket){
	if (strlen(file_name) >= FILE_NAME_MAX_SIZE) {
		printf("Too long file name\n");
		return;
	}
	char buffer[FILE_NAME_MAX_SIZE];  
	bzero(buffer, sizeof(buffer));  
	sprintf(buffer,"%s",file_name);
	// send(client_socket, buffer, strlen(buffer)+1, 0); 
	send(client_socket, buffer, FILE_NAME_MAX_SIZE, 0);
}
void recvFileName(char* file_name, int server_socket){
	char buffer[FILE_NAME_MAX_SIZE];  
	bzero(buffer, sizeof(buffer));  
	socklen_t length = recv(server_socket, buffer, FILE_NAME_MAX_SIZE, 0);  
	if (length < 0)  {  
		printf("Server Recieve Data Failed!\n");  
		exit(1);  
	}  
	bzero(file_name, sizeof(file_name));  
	sprintf(file_name,"%s",buffer);
}
void sendFile(char* file_name,int server_socket){
	char buffer[BUFFER_SIZE];
	FILE *fp = fopen(file_name, "r");  
	if (fp == NULL)  {  
		printf("File:\t%s Not Found!\n", file_name);  
	}  
	else  
	{  	
		fseek(fp,0,SEEK_END);
		int fileLen = ftell(fp);
		fseek(fp,0,SEEK_SET);
		send(server_socket, &fileLen, sizeof(int), 0);

		bzero(buffer, BUFFER_SIZE);  
		int file_block_length = 0;  
		while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {  
			// printf("file_block_length = %d\n", file_block_length);  
			if (send(server_socket, buffer, file_block_length, 0) < 0) {  
				printf("Send File:\t%s Failed!\n", file_name);  
				break;  
			}  
			bzero(buffer, sizeof(buffer));  
		}  
		fclose(fp);  
		// printf("File:\t%s Transfer Finished!\n", file_name);  
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

	int fileLen;
	recv(client_socket, &fileLen, sizeof(int), 0);
	// printf("fileLen: %d\n", fileLen);
	int length = 0;  
	// printf("Start to recv\n");
	while(length = recv(client_socket, buffer, fileLen < BUFFER_SIZE ? fileLen : BUFFER_SIZE, 0))  {  
		// printf("Reach with length %d\n", length);
		// printf("%s\n", buffer);
		if (length < 0)  {  
			printf("recvFile(): Recieve Data From Server %s Failed!\n", SERVER_IP);  
			break;  
		}  

		int write_length = fwrite(buffer, sizeof(char), length, fp);  
		if (write_length < length)  {  
			printf("recvFile(): File:\t%s Write Failed!\n", file_name);  
			break;  
		}  
		fileLen = fileLen - length;
		if (fileLen == 0) {// Last data block
			break;
		}
		bzero(buffer, BUFFER_SIZE);  
	}  
	// printf("recvFile(): Recieve File:\t %s From Server[%s] Finished!\n", file_name, SERVER_IP);  
	fclose(fp);  
}
