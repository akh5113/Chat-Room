/*********************************************************************************
* Anne Harris
* CS 372
* Program 1
* chatclient.c
* Description: This is file serves as the client side to a chat room simulation
* program. The user will first start the server and then start this client to 
* make connection with the server
* Last modified: 2/10/19
*********************************************************************************/

/*********************************************************************************
* CLIENT
*********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Source: Beej's Guide to Nework Programming - Using Internet Sockets
// https://beej.us/guide/bgnet/html/multi/index.html
// Source: "Computer Networking - A top-down Approach" 7 ed. Kurose & Ross, pg 164-170
// CS 344 Program 4 by Anne Harris

// get address information
// put in IP address, port number, get backs a pointer to a linked-list of results
int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

int main(int argc, char *argv[]){
	//variables for messages
	int  charsSent;
	int  charsRecvd;
	char messageBuffer[500]; //size of chat message from user is 500
	char chatBuffer[513]; //size of chat is 500, size of handle is 10, plus end string char plus '> ' chars

	//variables for connection
	int clientSocket;
	int portNum;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	
	//check for number of arguments from command line
	//should be 3 for executable, host, portnumber
	if (argc != 3) {
		fprintf(stderr, "USAGE: %s hostname port\n", argv[0]);
		exit(0);
	}

	//set up server (Host A) addess struct
	//clear out address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress));
	//get port number from command line
	portNum = atoi(argv[2]);

	// create network socket
	serverAddress.sin_family = AF_INET;
	//store port number
	serverAddress.sin_port = htons(portNum);
	//get server host address from command line
	serverHostInfo = gethostbyname(argv[1]);
	//copy in address
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length);
	
	// get users handle (10 chars)
	char clientHandle[11];
	//prompt user
	printf("Enter in your handle (10 character max): ");
	//clear out array
	memset(clientHandle, '\0', sizeof(clientHandle));
	//get response
	fgets(clientHandle, sizeof(clientHandle)-1, stdin);
	//remove \n from fgets
	clientHandle[strcspn(clientHandle, "\n")] = '\0';
		
	//set up socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {	//error check
		error("Error opening socket");
		printf("Socket connection failed\n");
		exit(0);
	}

	//connect to server
	if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		error("CLIENT: error connecting");
	}
	else{
		printf("Connected to server, type '\\quit' to exit the chat\n");
	}
	
	// variable to wait for quit message
	int quitChat = 0; 

	//loop for chat and getting connection
	while(quitChat == 0){	
		// get chat message from the client
		printf("%s> ", clientHandle);
		//clear buffer for chat
		memset(chatBuffer, '\0', sizeof(chatBuffer));
		//get input from user
		fgets(chatBuffer, sizeof(chatBuffer) - 1, stdin);
		chatBuffer[strcspn(chatBuffer, "\n")] = '\0';

		// clear out message buffer
		memset(messageBuffer, '\0', sizeof(messageBuffer));
		//combine chat message with handle to send
		strcpy(messageBuffer, clientHandle);
		strcat(messageBuffer, "> ");
		strcat(messageBuffer, chatBuffer);

		//print statement to check the message
		//printf("Message to send: %s\n", messageBuffer);
	
		//send message to server
		charsSent = send(clientSocket, messageBuffer, strlen(messageBuffer), 0); 
		//printf("Chars sent: %d\n", charsSent);
		if (charsSent < 0) {
			error("error writting to socket");
		}
		if (charsSent < strlen(messageBuffer)) {
			printf("warning: not all data written to socket\n");
		}
		
		//check for quit statement
		if((strcmp(chatBuffer, "\\quit")) ==0){
			close(clientSocket);
			quitChat = 1;
			exit(1);
		}

		//get return message from server
		//clear out buffer
		memset(messageBuffer, '\0', sizeof(messageBuffer));
		//read data from the socket
		charsRecvd = recv(clientSocket, messageBuffer, sizeof(messageBuffer) - 1, 0);
		if (charsRecvd < 0) {
			error("error reading from socket");
		}
		//check for quit from server
		if(charsRecvd == 1){
			close(clientSocket);
			quitChat = 1;
			exit(1);
		}

		//print chat message
		printf("%s\n", messageBuffer);
	}
	//close the socket
	close(clientSocket);

	return 0;
}
