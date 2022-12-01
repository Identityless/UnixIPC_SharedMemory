server: server.c
	gcc shserver.c -o server -pthread && gcc shclient.c -o client -pthread
