#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define NBUFF 256
#define XBUFF 100000
void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[NBUFF];
	
	FILE *infile;
	char infilename[NBUFF];
	char infiletext[XBUFF];
	int infilelen = 0;
	FILE *key;
	char keyname[NBUFF];
	char keytext[XBUFF];
	int keylen = 0;
	
	char c;
	int i; //das universal counter
	if (argc != 4) { fprintf(stderr,"USAGE: %s hostname port\n", argv[0]); exit(0); } // Check usage & args

	strcpy(infilename,argv[1]);
	strcpy(keyname,argv[2]);
	portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string

	infile = fopen(infilename, "r");
	if (infile==NULL) error ("cant open infile");
	else
	{
		i = 0;
		do{
			c = getc(infile);

			if ((int)c < (int)'A' || (int)c> (int)'Z')
			{
				if ((int)c == '\n')  // 10 should be \n ... i could not get \n to work
				{
					c = '\0';
				}
				else if ((int)c == -1){}
				else if((int)c != (int)' ')
				{
					printf("%d",(int)c);
					error ("i bad input");
				}
			}

			infiletext[i] = c;

			i++;

		}while (c != EOF);
	}

	infilelen = i; 

	fclose(infile);

	key = fopen(keyname, "r");
	if (key==NULL) error ("cant open key");
	else
	{
		i = 0;
		do{
			c = getc(key);

			if ((int)c < (int)'A' || (int)c> (int)'Z')
			{
				if ((int)c == '\n')  // 10 should be \n ... i could not get \n to work
				{
					c = '\0';
				}
				else if ((int)c == -1){} // i think -1 is eof
				else if((int)c != (int)' ')
				{
					printf("%d",(int)c);
					error ("i bad input");
				}
			}



			keytext[i] = c;

			i++;

		}while (c != EOF);

		keylen = i;

	}
	if(keylen <= infilelen)
	{
		error("key to small");
	}



	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
	if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("CLIENT: ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("CLIENT: ERROR connecting");

	// Get input message from user
	//memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array

	// Send message to server
	//charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	charsWritten = send(socketFD, infiletext, strlen(infiletext), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");
	
	// Send message to server
	//charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
	charsWritten = send(socketFD, keytext, strlen(keytext), 0); // Write to the server
	if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");


	// Get return message from server
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket, leaving \0 at end
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	printf("%s\n", buffer);

	close(socketFD); // Close the socket
	return 0;
}
