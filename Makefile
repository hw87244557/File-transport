all: FileTransport
FileTransport: 
	gcc -g FileServer.c utils/SocketLib.c utils/FileLib.c -o server
	gcc -g FileClient.c utils/SocketLib.c utils/FileLib.c -o client