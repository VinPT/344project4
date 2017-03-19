#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define  BUFF 1000
#define XBUFF 100000

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
	socklen_t sizeOfClientInfo;
	char key[XBUFF];
	char infile[XBUFF];
	struct sockaddr_in serverAddress, clientAddress;
	char itskind[BUFF];
	char buffer[BUFF];
	pid_t pid;
	int children = 0;

	char output[XBUFF];
	int i = 0;
	int calc = 0;

	memset(key, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	memset(infile, '\0', sizeof(buffer)); // Clear out the buffer again for reuse

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
	while (1)
	{

			// Accept a connection, blocking if one is not available until one connects
			sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
			establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
			if (establishedConnectionFD > 0);
			{
				pid = fork();
				if(-1 == pid)
				{
					error("SERVER: fork broke\n");
				}
				if (0 == pid)
				{

					// Get the message from the client and display it
					memset(itskind, '\0', BUFF);
					charsRead = recv(establishedConnectionFD, itskind, BUFF, 0); // Read the client's message from the socket
					if (charsRead < 0) error("ERROR reading from socket");

					if ('e' != itskind[0])
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

				//printf(itskind);
					// Get the message from the client and display it
				//	memset(infile, '\0', BUFF);
				//	charsRead = recv(establishedConnectionFD, infile, BUFF, 0); // Read the client's message from the socket
				//	if (charsRead < 0) error("ERROR reading from socket");

					while (1)
					{
						// Get message from client
						memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
						charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
						if (charsRead < 0) error("CLIENT: ERROR reading from socket");

						strcat(infile, buffer);
//printf("Server | MESSAGE ... %s  %s\n", buffer, infile);
						if (0 != strstr(buffer, "@"))
						{
							break;
						}
					}

					while (1)
					{
						// Get message from client
						memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
						charsRead = recv(establishedConnectionFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
						if (charsRead < 0) error("CLIENT: ERROR reading from socket");

						strcat(key, buffer);
	//printf("SERVER | KEY ... %s  %s\n", buffer, key);
						if (0 != strstr(buffer, "@"))
						{
							break;
						}
					}
					// Get the key from the client and display it
				//	memset(key, '\0', BUFF);
				//	charsRead = recv(establishedConnectionFD, key, BUFF, 0); // Read the client's message from the socket
				//	if (charsRead < 0) error("ERROR reading from socket");
				//printf(key);
					///
					//
					//


						while(XBUFF > i)
						{
							if(' ' == infile[i])
							{
								infile[i] = (char)26; //used to ve 25
							}
							else if((int)'A'>(int)infile[i]||(int)'Z'<(int)infile[i])
							{
								output[i]='@';
								break;
							}
							else
							{
								infile[i] = (char)((int)infile[i]-(int)'A');
							}
							calc = ((((int)infile[i]) + ((int)key[i]-(int)'A')) % 27);  ///used to be 26

							if(26 == calc) // used to be 25
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
					i = 0;
					while (i*BUFF < strlen(output))
					{
//printf("%s\n", output);
						memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
						strncpy(buffer, &output[i*BUFF], BUFF-1);
					//printf("client   %s\n",buffer);

						// Send message to server
						charsRead = send(establishedConnectionFD, buffer, BUFF, -1); // Send success back
						if (charsRead < 0) error("ERROR writing to socket");
						i++;
					}
					// Send a Success message back to the client


					close(establishedConnectionFD); // Close the existing socket which is connected to the client
					return 0;
				}
				else
				{
					children++;
					//holdingpattern
					if (children >= 5)
					{
			 			pid_t actualPid = waitpid(-1, NULL, 0);
						children--;
					}
					else
					{
						pid_t actualPid = waitpid(-1, NULL, WNOHANG);
							if(actualPid > 0)
							{
								children--;
							}
						}
					}
	 			}
			}

	close(listenSocketFD); // Close the listening socket
	return 0;
}
