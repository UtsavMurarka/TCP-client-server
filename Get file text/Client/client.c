#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

void client_call(int sockfd) 
{ 
	char file_query[1024];
	char jn; // junk
	int n; 
	while(1) { 
		bzero(file_query, sizeof(file_query)); 
		
		printf("\nEnter filename: ");
		scanf("%s", file_query);
		jn = getchar();
		
		// open the file for writing
		FILE *fp;
		fp = fopen(file_query, "w+");

		write(sockfd, file_query, sizeof(file_query)); 
		bzero(file_query, sizeof(file_query)); 
		read(sockfd, file_query, sizeof(file_query));
		
		//write to file
		fputs(file_query, fp);
		fclose(fp);

		printf("%s", file_query);
		printf("\nData received from server and written to the file.");
	} 
} 


int main(){
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	//Take query details as input
	printf("Enter Server's IP address: ");
	char server_ip[20];
	scanf("%s", server_ip);

	printf("Enter Server's Port Number: ");
	int portno;
	scanf("%d", &portno);
	char jn; //junk newline character
	jn = getchar();

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
