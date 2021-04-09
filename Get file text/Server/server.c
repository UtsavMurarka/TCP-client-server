#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

void server_call(int sockfd){
	char buff[1024]; 

	while(1) { 
		bzero(buff, 1024); 
  
		read(sockfd, buff, sizeof(buff));
		if(buff[0] != '\0'){
			printf("Query filename: %s\n", buff); 
			int count = 0;
			FILE *fp;
			fp = fopen(buff,"r");
			bzero(buff, 1024);

			int empty = 0;
			if( fp == NULL ){
			   printf("Error while opening the file.\n");
			   empty = 1;
			}

			if(empty == 1){
				buff[0] = '\0';
			}
			else{
				char ch;
				while( ( ch = fgetc(fp) ) != EOF && count < 1024){
				  buff[count++] = ch; 
				} 
				fclose(fp);
			}
			write(sockfd, buff, 1024); 
		}
	}
}

int main(){
	int sockfd, connfd, len, portno;
	struct sockaddr_in servaddr, cli;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		printf("ERROR: Socket creation failed\n");
		return 0;
	}
	bzero(&servaddr, sizeof(servaddr));
	printf("Enter Port No. for hosting the server (between 2000 - 65535): ");
	scanf("%d", &portno);

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(portno);

	if((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		printf("ERROR: binding socket failed\nTry again with different port number\n"); 
		return 0; 
	}
	printf("Server Succesfully Hosted!\n");

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

		server_call(connfd);
	}
	
	close(sockfd);
}