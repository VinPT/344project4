#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define  BUFF 1000

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char key[BUFF];
	char infile[BUFF];
	struct sockaddr_in serverAddress, clientAddress;
	char itskind[BUFF];
	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) error("ERROR opening socket");

	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections

	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
	if (establishedConnectionFD < 0) error("ERROR on accept");


	// Get the message from the client and display it
	memset(itskind, '\0', BUFF);
	charsRead = recv(establishedConnectionFD, itskind, BUFF, 0); // Read the client's message from the socket
	if (charsRead < 0) error("ERROR reading from socket");
	
	if ('d' != itskind[0])
	{
		itskind[0] = 'v';
		// Send a fail message back to the client
		charsRead = send(establishedConnectionFD, itskind, 39, 0); // Send success back
		if (charsRead < 0) error("ERROR writing to socket");
		error("Encrpt Server has been connected to the wrong program");
	}
	else
	{
		// Send a Success message back to the client
		charsRead = send(establishedConnectionFD, itskind, 39, 0); // Send success back
		if (charsRead < 0) error("ERROR writing to socket");
	}


	// Get the message from the client and display it
	memset(infile, '\0', BUFF);
	charsRead = recv(establishedConnectionFD, infile, BUFF, 0); // Read the client's message from the socket
	if (charsRead < 0) error("ERROR reading from socket");

	// Get the key from the client and display it
	memset(key, '\0', BUFF);
	charsRead = recv(establishedConnectionFD, key, BUFF, 0); // Read the client's message from the socket
	if (charsRead < 0) error("ERROR reading from socket");

	///
	//
	//
		char output[BUFF] = {'\0'};
		int i = 0;
		int calc = 0;

		while(BUFF > i)
		{
			if(' ' == infile[i])
			{
				infile[i] = (char)25;
			}
			else if((int)'A'>(int)infile[i]||(int)'Z'<(int)infile[i])
			{
				infile[i]='\0';
				break;
			}
			else
			{
				infile[i] = (char)((int)infile[i]-(int)'A');
			}
			calc = (((int)infile[i]) - ((int)key[i]-(int)'A'));

			if (0 > calc)
			{
				calc = calc +26;
			}

			if(25 == calc)
			{
				output[i] = ' ';
			}
			else
				output[i] = (char)(calc + (int)'A');

			i++;
		}

	//
	//
	///
	
	// Send a Success message back to the client
	charsRead = send(establishedConnectionFD, output, 39, 0); // Send success back
	if (charsRead < 0) error("ERROR writing to socket");
	close(establishedConnectionFD); // Close the existing socket which is connected to the client
	close(listenSocketFD); // Close the listening socket
	return 0; 
}

