#include <netinet/in.h>                         // for sockaddr_in  
#include <sys/types.h>                          // for socket  
#include <sys/socket.h>                         // for socket  
#include <stdio.h>                              // for printf  
#include <stdlib.h>                             // for exit  
#include <string.h>                             // for bzero  
#include "utils/SocketLib.h"
#include "utils/Constants.h"
#include "utils/FileLib.h"
int main(int argc, char **argv) {                                   
	int client_socket = create_client_socket(SERVER_PORT); 

	char file_name[FILE_NAME_MAX_SIZE + 1];  
	bzero(file_name, sizeof(file_name));  
	printf("Please Input File Name On Server.\t");  
	scanf("%s", file_name);  

	char server_file_name[FILE_NAME_MAX_SIZE+1];
	sprintf(server_file_name,"upload/%s",file_name);

	sendFileName(server_file_name, client_socket);

	char client_file_name[FILE_NAME_MAX_SIZE+1];
	sprintf(client_file_name,"download/%s",file_name);
	recvFile(client_file_name, client_socket);
	close(client_socket);  
	return 0;  

}