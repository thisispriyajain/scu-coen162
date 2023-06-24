//Priya Jain
//COEN 162: Web Infrastructure
//Code Project 1: Middleman Proxy

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
	int oldSocket, newSocket, listenfd;
	char buffer[1024];
	char bufferCopy[1024];

	struct sockaddr_in serverAddr;
	struct sockaddr_storage serverStorage;
	
	int addr_size;

	//Create Server Socket
	oldSocket = socket(PF_INET, SOCK_STREAM, 0);
	serverAddr.sin_family = AF_INET;

	serverAddr.sin_port = htons(9000);

	unsigned long address = INADDR_NONE;

	address = inet_addr("127.0.0.1");

	serverAddr.sin_addr.s_addr = address;

	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	//bind(oldSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));	
	//if(listenfd, (struct sockaddr *)&serverAddr, sizeof(struct sockaddr)) <  0) {
		//perror("Cannot bind the server address to the endpoint socket\n");
	
		//exit(1);
	//}

	//listen(listenfd, 5);
	
	//if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        	//perror("setsockopt");
		 //exit(EXIT_FAILURE);
    	//}

    	//address.sin_family = AF_INET;
   	//address.sin_addr.s_addr = INADDR_ANY;
    	//address.sin_port = htons(PORT);
  
    // Forcefully attaching socket to the port 8080
    if (bind(oldSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    	 perror("bind failed");
    	 exit(EXIT_FAILURE);
    }
    if (listen(oldSocket, 3) < 0) {
	perror("listen");
    	exit(EXIT_FAILURE);
    }

 	while(1) {
		printf("Server listening\n");
		addr_size = sizeof(serverStorage);
		newSocket = accept(oldSocket, (struct sockaddr *)&serverStorage, (socklen_t *) &addr_size);

		read(newSocket, buffer, 1023);

		strcpy(bufferCopy, buffer);

		int i = 0;
		char *token = strtok(bufferCopy, "\r\n");

		while (i != 1) {
			token = strtok(NULL, "\r\n");
			i += 1;
		}

		printf("%s\n", token);

		char *name = NULL;
		name = strtok(token, " ");
		name = strtok(NULL, " ");

		printf("%s\n", name);
		int size = strlen(name);
		printf("Size: %d\n", size);
	
		struct hostent *host;
		struct in_addr ipAdd;
		host = gethostbyname(name);
		ipAdd = *(struct in_addr *)(host->h_addr); 
		
		int clientSocket;
		struct sockaddr_in clientAddr;
		char bufferTwo[1024];
		socklen_t addr_sizeTwo;
		addr_sizeTwo = sizeof(clientAddr);
	
		if((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
			printf("Failed to create a socket for %s.\n", name);
			exit(1);
		}
		else {
			printf("Socket created for %s.\n", name);
		}	

		clientAddr.sin_family = AF_INET;
		clientAddr.sin_port = htons(80);

		unsigned long addressTwo = INADDR_NONE;
		addressTwo = inet_addr(inet_ntoa(ipAdd));

		clientAddr.sin_addr.s_addr = addressTwo;

		printf("Printing IP Address: %s\n", inet_ntoa(ipAdd));

		memset(clientAddr.sin_zero, '\0', sizeof clientAddr.sin_zero);

		connect(clientSocket, (struct sockaddr*)&clientAddr, addr_sizeTwo);
		printf("Connected client\n");

		send(clientSocket, buffer, strlen(buffer), 0);
		printf("Sent message from client\n");

		printf("Before while loop\n");
		printf("%s\n", bufferTwo);

		int n;
		while((n = read(clientSocket, bufferTwo, 1024)) > 0) {
			printf("In while loop\n");
			write(newSocket, bufferTwo, n);
			printf("%s\n", bufferTwo);
			bzero(bufferTwo, n);
		}
		
		printf("After while loop\n");

		close(newSocket);
		close(clientSocket);
	} 

	return 0;	
}
