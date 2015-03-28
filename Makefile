all: FileTransport
FileTransport: 
	gcc -g FileServer.c utils/SocketLib.c -o server
	gcc -g FileClient.c -o client