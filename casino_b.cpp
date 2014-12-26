#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVERPORT 21922
#define MAXDATASIZE 100 // max number of bytes we can get at once
#define SERVER "21922"
using namespace std;

char sendString[100] = "";

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




int main(void)
{


	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	
	struct sockaddr_in client;
	socklen_t client_len;
	client_len = sizeof(client);
	
	struct sockaddr_in server;
	struct hostent *he;
	
	he = gethostbyname("nunki.usc.edu"); // get the address of the local server
	socklen_t addr_len;
	addr_len = sizeof(server);
	
/*-----------------------------------
  Code for Reading from file */
	char buffer[5];
    char temp[5];
    int size =0, number;
    FILE *fr =  fopen("stop_b.txt","r");
	
	sendString[0]='B';
	sendString[1]='<';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = '>';
	
    fclose(fr);
	
	//printf("Read String: %s \n", sendString);
/*-----------------------------------*/  

	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((rv = getaddrinfo("nunki.usc.edu", SERVER, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	memset(&server , 0 , sizeof(server)); //populating the socket structure
	server.sin_family = AF_INET;
	server.sin_addr = *((struct in_addr *)he -> h_addr);
	server.sin_port = htons(SERVERPORT);
	
	
	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		
		if (getsockname(sockfd, (struct sockaddr *)&client, &client_len) == -1) {
			perror("getsockname() failed");
			return -1;
		}
		
		if(bind (sockfd , (struct sockaddr *)&client, sizeof(client))==-1)
		{
			perror("Bind");
			exit(1);
		}
		
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}
	
	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}
	
	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
	//printf("client: connecting to %s\n", s);
	printf("Casino B Phase 1: The casino B has dynamic TCP port number %d and IP address %s\n" ,ntohs(client.sin_port),s );

	char temp1[100];
	strcpy(temp1 , "");
	int i,k;
	
	for(i=1,k=0;i<strlen(sendString);i++,k++)
	{
		temp1[k] = sendString[i];
	}
	
	temp[k]='\0';
	printf("Casino B Phase 1: Sending the casino vector %s to the transit hub\n", temp1);	
	//Sending the sendString over to the Server
	sendto(sockfd , sendString , sizeof(sendString),0 ,(struct sockaddr *)&server, sizeof(server));
	
	
	freeaddrinfo(servinfo); // all done with this structure
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	buf[numbytes] = '\0';
	//printf("client: received '%s'\n",buf);
	close(sockfd);
	
	printf("Casino B Phase 1: End of Phase 1\n");
	return 0;
}
	


