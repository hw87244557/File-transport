#include<netinet/in.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h> 
#include"SocketLib.h"

#define LENGTH_OF_LISTEN_QUEUE     20  

int create_server_socket(int ServerPort){
// set server socket's address information    
	struct sockaddr_in   server_addr;  
	bzero(&server_addr, sizeof(server_addr));  
	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
	server_addr.sin_port = htons(ServerPort);  

// create a stream socket    
	int server_socket = socket(PF_INET, SOCK_STREAM, 0);  
	if (server_socket < 0)  
	{  
		printf("create_server_socket(): Create Socket Failed!\n");  
		exit(1);  
	}  

// bind the socket with the server's address
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))  
	{  
		printf("create_server_socket(): Server Bind Port: %d Failed!\n", ServerPort);  
		exit(1);  
	}  

// listen the socket
	if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))  
	{  
		printf("create_server_socket(): Server Listen Failed!\n");  
		exit(1);  
	}  
	return server_socket;
}