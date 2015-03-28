#include <netinet/in.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h> 
#include "SocketLib.h"
#include "Constants.h"


int create_server_socket(int ServerPort){
	// set server socket's address information    
	struct sockaddr_in   server_addr;  
	bzero(&server_addr, sizeof(server_addr));  
	server_addr.sin_family = AF_INET;  
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);  
	server_addr.sin_port = htons(ServerPort);  

	// create a stream socket    
	int server_socket = socket(PF_INET, SOCK_STREAM, 0);  
	if (server_socket < 0)  {  
		printf("create_server_socket(): Create Socket Failed!\n");  
		exit(1);  
	}  

	// bind the socket with the server's address
	if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)))  {  
		printf("create_server_socket(): Server Bind Port: %d Failed!\n", ServerPort);  
		exit(1);  
	}  

	// listen the socket
	if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE))  {  
		printf("create_server_socket(): Server Listen Failed!\n");  
		exit(1);  
	}  
	return server_socket;
}

int create_client_socket(int ServerPort){
	struct sockaddr_in client_addr;  
	bzero(&client_addr, sizeof(client_addr));  
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htons(INADDR_ANY); 
	client_addr.sin_port = htons(0); 

	int client_socket = socket(AF_INET, SOCK_STREAM, 0);  
	if (client_socket < 0)  {  
		printf("create_client_socket(): Create Socket Failed!\n");  
		exit(1);  
	}  

	if (bind(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)))  {  
		printf("create_client_socket(): Client Bind Port Failed!\n");  
		exit(1);  
	}  

	struct sockaddr_in  server_addr;  
	bzero(&server_addr, sizeof(server_addr));  
	server_addr.sin_family = AF_INET;  

	if (inet_aton(SERVER_IP, &server_addr.sin_addr) == 0)  {  
		printf("create_client_socket(): Server IP Address Error!\n");  
		exit(1);  
	}  

	server_addr.sin_port = htons(ServerPort);  
	socklen_t server_addr_length = sizeof(server_addr);  

	if (connect(client_socket, (struct sockaddr*)&server_addr, server_addr_length) < 0) {  
		printf("create_client_socket(): Can Not Connect To %s!\n", SERVER_IP);  
		exit(1);  
	}  
	return client_socket;
}