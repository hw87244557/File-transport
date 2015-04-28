all: FileTransport
FileTransport: 
	#gcc -g FileServer.c utils/SocketLib.c utils/FileLib.c -o server
	#gcc -g FileClient.c utils/SocketLib.c utils/FileLib.c -o client
	gcc -g ThinkPad.c utils/SocketLib.c utils/FileLib.c -lpthread -o server
	gcc -g IdeaPad.c utils/SocketLib.c utils/FileLib.c -lpthread -o client