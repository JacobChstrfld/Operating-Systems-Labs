#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>

bool t;

int main(int argc, char* argv[]){

    using namespace::std;
    t = false;
	
	int clientSocket, count, maxBurgers, time;
	struct sockaddr_in serverAddr;
	char buffer[1024];
    const char* ip = "127.0.0.1";
    int port = 54321;
    maxBurgers = 10;
    try{
        const char* ip = argv[1];
        port = stoi(argv[2]);
        maxBurgers = stoi(argv[3]);
    }catch(exception &e){
        printf("Error with command line args, using default values..\n");
    }

    string str, temp;

	if((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) <= 0){
        printf("Socket creation failed, terminating...\n");
		fflush(stdout);
		return(-1);
    }
	printf("Client Socket Created Sucessfully.\n");
    fflush(stdout);

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);

	if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
        printf("Connect failed, trying again in 5 seconds...\n");
		fflush(stdout);
        sleep(5);
        if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
            printf("Connect failed, trying again in 5 seconds...\n");
		    fflush(stdout);
            sleep(5);
            if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
            printf("Connect failed, terminating...\n");
		    fflush(stdout);
            return(-1);
            }
        } 
    }
	printf("Connected to Server.\n\n");
    fflush(stdout);
    count = 0;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    srand((time_t)ts.tv_nsec);


    while(1){

        count++;
        time = (rand() % 3) + 1;
        fflush(stdout);
        memset(buffer, 0, sizeof(buffer));
        recv(clientSocket, buffer, 1024, 0);
        if(buffer[0] == '\0') break;
        else if(buffer[0] == 'A'){
            temp = buffer;
            printf("%s",temp.data());
            break;
        }
	    printf("Client recieved %s\n", buffer);
        temp = buffer;
        fflush(stdout);
        memset(buffer, 0, sizeof(buffer));
        if(time == 1){
            sleep(3);
            time = 3;
        }
        else if(time == 2){
            sleep(5);
            time = 5;
        }
        else{
            sleep(7);
            time = 7;
        }
        str = "Client consumed " + temp + " in " + to_string(time) + " seconds and can only consume " 
        + to_string(maxBurgers - count) + " more burgers\n\n";
        printf("%s",str.data());
        fflush(stdout);
        if(count == maxBurgers || temp[0] == 'l'){
            str = "stop";
            t= true;
            strcpy(buffer, str.data());
            send(clientSocket, buffer, 1024, 0);
            break;
        } 
        else str = "continue";
        strcpy(buffer, str.data());
        send(clientSocket, buffer, 1024, 0);

    }

    printf("Closing the connection.\n");
    fflush(stdout);
    close(clientSocket);

    if(t){
        socket(PF_INET, SOCK_STREAM, 0);
        memset(&serverAddr, '\0', sizeof(serverAddr));
	    serverAddr.sin_family = AF_INET;
	    serverAddr.sin_port = htons(port);
	    serverAddr.sin_addr.s_addr = inet_addr(ip);
        connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    }
    return(0);

}