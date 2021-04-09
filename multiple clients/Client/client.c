#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

void client_call(int sockfd) {
	/*
		- scanf
		- write
		- read
		- print reverse
	*/
	char buff[1024];
	while(1){
		bzero(buff, 1024);
		printf("Enter String: ");
		fgets(buff, 1024, stdin);
		write(sockfd, buff, sizeof(buff));

		if(strcmp(buff, "exit\n") == 0){
			break;
		}

		bzero(buff, 1024);
		read(sockfd, buff, sizeof(buff));
		if(buff[0] == '\0'){
			return 0;
		}

		int i;
		for(i=strlen(buff)-1; i>=0; i--){
			printf("%c", buff[i]);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[]){
	/*Command line arguments:
		1. IP of server
		2. Port no. of server
	*/
	if(argc != 3){
		printf("Incorrect number of arguments passed\nPass IP and Port no. as parameters");
		return 0;
	}
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	//Take query details as input
	char server_ip[20];
	strcpy(server_ip, argv[1]);
	int portno = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0) { 
		printf("Error while opening Socket\n"); 
		return 0;
	}

	bzero(&servaddr, sizeof(servaddr)); 

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(server_ip); 
	servaddr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr * )&servaddr, sizeof(servaddr)) != 0){ 
		printf("Error while connecting to server\n"); 
		return 0;
	}

	client_call(sockfd);
	close(sockfd);

}