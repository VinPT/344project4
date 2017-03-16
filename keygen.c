#include <stdio.h>
#include <stdlib.h>

#define MAXBUFFER 100000


char randletter();

int main(int argc, char *argv[])
{
	int returnValue = 0;
	int input = 0;
	char randString[MAXBUFFER] = {"\0"};
	if (1 <= argc)
	{
		input = atoi(argv[1]);	

		int i = 0;
		while(i < input) // only less then makes up for programmer start vs normal start 0 vs 1
		{
			randString[i] = randletter();
			i++;	
		}
		randString[i] = '\n';

		printf("%s",randString);
	}
	else
	{
		return 1;
	}
	return returnValue;
}

char randletter()
{
	char rl =  ("ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random () % 26]);
	return rl;
}
