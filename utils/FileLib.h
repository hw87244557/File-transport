#include<netinet/in.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h> 

void sendFileName(char* file_name, int client_socket);
void recvFileName(char* file_name, int server_socket);
void recvFile(char* file_name, int client_socket);
void sendFile(char* file_name,int server_socket);