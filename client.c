#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <unistd.h>
/*

Diana Oh CS372 Fall 2018 Program 1 client.c 
This Program acts as the client in a mini chat application.
*/
//Cite: Some code reused from CS344 Operating Systems 
//TESTED ON FLIP 1

int socketFD; 

//Set up socket connection to talk to server
void createSocketConnection(int portNumber, char* host)
{
	//int  portNumber;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;

	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
//	portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname(host); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address
																										// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) fprintf(stderr, "CLIENT: ERROR opening socket");

	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		fprintf(stderr, "CLIENT: ERROR connecting");

}

//Send message to server 
void sendMessage(char *finalMessage, char * input) {

	int charsWritten = 0;
	//Send message to server  
	while (charsWritten < strlen(finalMessage))
	{

		charsWritten = send(socketFD, finalMessage, strlen(finalMessage), 0); // Write to the server

		if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
		if (charsWritten < strlen(input)) printf("CLIENT: WARNING: Not all data written to socket!\n");
	}

}

//Receive message from server 
void receiveMessage()
{
	char receivedInput[600];
	memset(receivedInput, '\0', sizeof(receivedInput)); // Clear out the buffer again for reuse

	int charsRead = 0; 
	charsRead = recv(socketFD, receivedInput, sizeof(receivedInput) - 1, 0); // Read data from the socket, leaving \0 at end
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");

	printf("%s\n", receivedInput);
}


int main(int argc, char *argv[])
{

int  portNumber, charsRead, charsWrittenv2;
struct sockaddr_in serverAddress;
struct hostent* serverHostInfo;

int charsWritten = 0; 

// CITE: https://c-for-dummies.com/blog/?p=1112
char *username; 
char *usernameFinalVersion; 
char *usernameFinalVersionv2;
size_t usernameSize = 32; 
size_t characters;
size_t userNameFinalVersion_size = 32; 
//char usernameFinalVersion[40]; 

username = (char *)malloc(usernameSize * sizeof(char)); 
usernameFinalVersion = (char *)malloc(userNameFinalVersion_size * sizeof(char)); 
usernameFinalVersionv2 = (char *)malloc(userNameFinalVersion_size * sizeof(char)); 


	if (argc < 2) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args
//printf("arg 1 is %s", argv[1]); 

//Get user name from Host B 
printf("Please provide username for Host B\n");
characters = getline(&username, &usernameSize, stdin); 
char name[32] = " ";

int i; 

//Alter username to add > at the end
for (i=0; i< strlen(username)-1; i++)
{

usernameFinalVersion[i] = username[i]; 
}

usernameFinalVersion[i] = '>';


strcpy(usernameFinalVersionv2, usernameFinalVersion);

//Transfer username to char array  instead of using pointer to char

for(i=0; strlen(usernameFinalVersion); i++)
{
name[i]= usernameFinalVersion[i];
}


// Create Socket Connection on client side 

// Set up the server address struct

portNumber = atoi(argv[2]); // Get the port number, convert to an integer from a string


createSocketConnection(portNumber, argv[1]);

char *finalMessage;
char *input; //char *receivedInput;

size_t input_size = 501;
size_t finalMessage_size = 600;  

//Make size for input and final message
input = (char *)malloc(input_size * sizeof(char)); 
finalMessage = (char *) malloc(finalMessage_size * sizeof(char)); 

//Cite: https://c-for-dummies.com/blog/?p=1112

//char  *captureString;
char *quit = "\\quit";


 
while(1) 

{

//Get message to send to server
memset(input, '\0',sizeof(input));
memset(finalMessage, '\0', sizeof(finalMessage)); 

// Print the name and the > the client entered 
printf("%s", name); 

// Collect inut from client
characters = getline(&input, &input_size, stdin); 

//Construct final message by adding client name and the input 
//Cite: Credit to student Brallen for the idea to use strcpy and strstr
strcpy(finalMessage,name); 
strcat(finalMessage, input); 

// Use strstr to find when quit  has been entered 
char *pch = strstr(input,quit); 

if(pch)
{
printf("Client chat session has ended. \n");
sendMessage(finalMessage,input); 
break; 
}


sendMessage(finalMessage, input);
	// Get return message from server
	receiveMessage();


}


	close(socketFD); // Close the socket

	return 0;

}
