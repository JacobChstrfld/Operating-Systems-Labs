#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <pthread.h>



int maxBurgers, burgerCount, runServer;

int chefCount;

void * handleConnec(void * pclientSocket)
{
	using namespace ::std;
	char buffer[1024];
	string str;

	int chef = chefCount;

	int clientSocket = *((int*)pclientSocket);
	free(pclientSocket);

	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	srand((time_t)ts.tv_nsec);

	while (burgerCount < maxBurgers)
	{
		int time = (rand() % 2) + 1;
		if (time == 1)
		{
			sleep(4);
			time = 4;
		}
		else
		{
			sleep(6);
			time = 6;
		}
		burgerCount++;
		memset(buffer, 0, sizeof(buffer));
		if(burgerCount > maxBurgers) {
			runServer = 0;
			break;
		}
		str = "Chef #" + to_string(chef) + " made burger #" + to_string(burgerCount) + " in " + to_string(time) + " seconds and is now being delieverd to client #" + to_string(clientSocket - 3) + "\n";
		printf("%s", str.data());
		fflush(stdout);
		str = "Burger restaraunt can only make " + to_string(maxBurgers - burgerCount) + " more burgers today\n\n";
		printf("%s", str.data());
		fflush(stdout);
		if (burgerCount >= maxBurgers){
			 runServer = 0;
			 str = "last burger of the day";
		}else str = "burger";

		strcpy(buffer, str.data());
		send(clientSocket, buffer, 1024, 0);
		memset(buffer, 0, sizeof(buffer));
		recv(clientSocket, buffer, 1024, 0);
		if(buffer[0] == 's' || buffer[1] == '\0'){
			break;
		}
	}
	if(maxBurgers <= burgerCount) runServer = 0;
	chefCount--;
	return NULL;
	
}

int main(int argc, char *argv[])
{

	using namespace ::std;
	int sockfd, count, maxChefs;
	struct sockaddr_in serverAddr;
	int port =  54321;
	chefCount = 0;
	int clientSocket;
	struct sockaddr_in newAddr;
	int opt = 1;
	char buffer[1024];
	string str;

	runServer = 1;
	burgerCount = 0;
	maxBurgers = 25;
	maxChefs = 2;

	try{
        maxBurgers = stoi(argv[1]);
		maxChefs = stoi(argv[2]);
		port = stoi(argv[3]);
    }catch(exception &e){
        printf("Error with command line args, using default values..\n");
    }

	socklen_t addrLength;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
	{
		printf("Socket creation failed, terminating...\n");
		fflush(stdout);
		return (-1);
	}
	printf("Server Socket Created Sucessfully.\n");
	memset(&serverAddr, '\0', sizeof(serverAddr));

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("Set socket options failed\n");
		exit(-1);
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	addrLength = sizeof(serverAddr);

	if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
	{
		printf("Bind failed, terminating...\n");
		fflush(stdout);
		return (-1);
	}
	printf("Binded to Port number %d.\n", port);
	fflush(stdout);

	if (listen(sockfd, 10) < 0)
	{
		printf("listen failed, terminating...\n");
		fflush(stdout);
		return (-1);
	}
	printf("Listening...\n\n");
	fflush(stdout);

	while (runServer != 0)
	{
		if ((clientSocket = accept(sockfd, (struct sockaddr *)&serverAddr, (socklen_t *)&addrLength)) < 0)
		{
			printf("Accept failed, terminating...\n");
			fflush(stdout);
			return (-1);
		}
		if(maxChefs == chefCount){
			str = "All chefs are currently tied up, restart client to try again\n\n";
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, str.data());
			send(clientSocket, buffer, 1024, 0);
			fflush(stdout);
			close(clientSocket);
			continue;
		}
		chefCount++;

		int *pclient = (int*)malloc(sizeof(int));  
		*pclient = clientSocket;
		//printf("Connection handled\n\n");
		//fflush(stdout);

		pthread_t t;
		pthread_create(&t, NULL, handleConnec, pclient);
		//handleConnec(clientSocket);

	}

	printf("Closing the connection.\n");
	fflush(stdout);

	close(sockfd);

	return (0);
}