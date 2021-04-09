#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/mman.h>

static int *connected;

void server_call(int sockfd){
	/*
		- read
		- print reverse
		- scanf
		- write
	*/
	char buff[1024];
	while(1){
		bzero(buff, 1024);
		read(sockfd, buff, sizeof(buff));
		if(strcmp(buff, "exit\n") == 0){
			break;
		}
		int i;
		for(i = strlen(buff)-1; i>=0; i--){
			printf("%c", buff[i]);
		}
		printf("\n");

		printf("Enter String: ");
		bzero(buff, 1024);
		fgets(buff, 1024, stdin);
		write(sockfd, buff, sizeof(buff));

	}

}

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Incorrect number of arguments passed\nPass Port number as argument\n");
		return 0;
	}
	int sockfd, connfd, len, portno;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		printf("ERROR: Socket creation failed\n");
		return 0;
	}
	bzero(&servaddr, sizeof(servaddr));
	portno = atoi(argv[1]);

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(portno);

	// int connected = 0;

	connected = mmap(NULL, sizeof *connected, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	*connected = 0;

	// void* shmem = mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	// memcpy(shmem, &connected, sizeof(connected));


	if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		printf("ERROR: binding socket failed\nTry again with different port number\n"); 
		return 0; 
	}
	printf("Server Succesfully Hosted at Port No.: %d \n", portno);

	fflush(stdin);
	while(1){
		if((listen(sockfd, 5)) != 0) {
			printf("ERROR: Listening failed\n"); 
			return 0;
		}
		
		len = sizeof(cli);


		connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
		if (connfd < 0) { 
			printf("ERROR: Could not connect to server\n"); 
			return 0;
		}
		
		*connected = *connected + 1;

		if(*connected > 4){
			printf("\nCANNOT ACCEPT MORE CLIENTS\n");
			char buff[1024];
			bzero(buff, 1024);
			write(connfd, buff, sizeof(buff));
			close(connfd);
			*connected = *connected - 1;
			continue;
		}

		printf("\n-----NEW CLIENT JOINED: %d ACTIVE CONNECTIONS-----\n", *connected);

		if(fork() == 0){
			//child
			server_call(connfd);
			*connected = *connected - 1;
			printf("\n-----CLIENT LEFT: %d ACTIVE CONNECTIONS-----\n", *connected);
			//dec count
			return 0;
		}
		
	}
	
	close(sockfd);
}