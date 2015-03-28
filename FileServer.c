#include<netinet/in.h>  
#include<sys/types.h>  
#include<sys/socket.h>  
#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  
#include"utils/SocketLib.h"
#define SERVER_PORT    6666  
#define BUFFER_SIZE                1024  
#define FILE_NAME_MAX_SIZE         512  
  
int main(int argc, char **argv)  
{  
    int server_socket = create_server_socket(SERVER_PORT);
  
     while(1)  
    {  
        struct sockaddr_in client_addr;  
        socklen_t          length = sizeof(client_addr);    
        
        int new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);  
        if (new_server_socket < 0)  
        {  
            printf("Server Accept Failed!\n");  
            break;  
        }  
  
        char buffer[BUFFER_SIZE];  
        bzero(buffer, sizeof(buffer));  
        length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);  
        if (length < 0)  
        {  
            printf("Server Recieve Data Failed!\n");  
            break;  
        }  
  
        char file_name[FILE_NAME_MAX_SIZE + 1];  
        bzero(file_name, sizeof(file_name));  
        strncpy(file_name, buffer,  
                strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));  
  
        FILE *fp = fopen(file_name, "r");  
        if (fp == NULL)  
        {  
            printf("File:\t%s Not Found!\n", file_name);  
        }  
        else  
        {  
            bzero(buffer, BUFFER_SIZE);  
            int file_block_length = 0;  
            while( (file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)  
            {  
                printf("file_block_length = %d\n", file_block_length);  
  
                // 发送buffer中的字符串到new_server_socket,实际上就是发送给客户端  
                if (send(new_server_socket, buffer, file_block_length, 0) < 0)  
                {  
                    printf("Send File:\t%s Failed!\n", file_name);  
                    break;  
                }  
  
                bzero(buffer, sizeof(buffer));  
            }  
            fclose(fp);  
            printf("File:\t%s Transfer Finished!\n", file_name);  
        }  
  
        close(new_server_socket);  
    }  
  
    close(server_socket);  
  
    return 0;  
}