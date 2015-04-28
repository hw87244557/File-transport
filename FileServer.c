#include <netinet/in.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "utils/SocketLib.h"
#include "utils/Constants.h"
#include "utils/FileLib.h"

int main(int argc, char **argv)  
{  
    int server_socket = create_server_socket(SERVER_PORT);

    while(1)  
    {  
        struct sockaddr_in client_addr;  
        socklen_t length = sizeof(client_addr);    

        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
        if (new_server_socket < 0)  {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
        char file_name[FILE_NAME_MAX_SIZE + 1];  

        recvFileName(file_name,new_server_socket);

        sendFile(file_name,new_server_socket);  

        close(new_server_socket);  
    }  

    close(server_socket);  

    return 0;  
}