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

#define MYUDPPORT 5922 //UDP PORT Number for Phase 2
#define MYUPD "5922"

#define CASINOBPORT 7922
#define CASINOB "7922"
using namespace std;

char sendString[100] = "";
char sendStringUDP[100]="";

// get sockaddr, IPv4 or IPv6:

void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}




int main(int argc, char *argv[])
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
	
	struct addrinfo hintsUDP , * servinfoUDP , *pUDP;
	int sockfdUDP;
	int rvUDP ;
	char bufUDP[MAXDATASIZE];
	struct sockaddr_storage their_addr;
	struct sockaddr_in client_udp;
	socklen_t udpclient_len;
	udpclient_len = sizeof(client_udp);

	struct sockaddr_in casinoB;
	
	int a , b , c , d;
	int a2 , b2 , c2 , d2;
/*---------------------------------------------------------------------*/
  Code for Reading from file */
	char buffer[5];
    char temp[5];
    int size =0, number;
    FILE *fr =  fopen("stop_a.txt","r");
	
	sendString[0]='A';
	sendString[1]='<';
	
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	a= number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	b= number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	c= number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	d= number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = '>';
	
    fclose(fr);
	
	//printf("Read String: %s \n", sendString);
/*-----------------------------------------------------------------------*/  
	
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
	printf("Casino A Phase 1: The casino A has dynamic TCP port number %d and IP address %s\n" ,ntohs(client.sin_port),s );
	
	char temp1[100];
	strcpy(temp1 , "");
	int i,k;
	
	for(i=1,k=0;i<strlen(sendString);i++,k++)
	{
		temp1[k] = sendString[i];
	}
	
	temp[k]='\0';
	printf("Casino A Phase 1: Sending the casino vector %s to the transit hub\n", temp1);
	//Sending the sendString over to the Server
	sendto(sockfd , sendString , sizeof(sendString),0 ,(struct sockaddr *)&server, sizeof(server));
	
	
	//printf("client: connecting to %s\n", s);
	freeaddrinfo(servinfo); // all done with this structure
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	buf[numbytes] = '\0';
	//printf("client: received '%s'\n",buf);
	close(sockfd);
	
	printf("Casino A Phase 1: End of Phase 1\n");
	
	/*----------------------------Code for UPD Phase2------------------------*/
	
	printf("\nCasino A Phase 2: The casino A has static UDP port number %d" , MYUDPPORT);

	memset(&hintsUDP, 0, sizeof hintsUDP);
    hintsUDP.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hintsUDP.ai_socktype = SOCK_DGRAM;
    hintsUDP.ai_flags = AI_PASSIVE; // use my IP
	
	memset(&casinoB , 0 , sizeof(casinoB)); //populating the socket structure
	casinoB.sin_family = AF_INET;
	casinoB.sin_addr = *((struct in_addr *)he -> h_addr);
	casinoB.sin_port = htons(CASINOBPORT);
	
	if ((rvUDP = getaddrinfo(NULL, MYUPD, &hintsUDP, &servinfoUDP)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUDP));
        return 1;
    }
	
	for(pUDP = servinfoUDP; pUDP != NULL; pUDP = pUDP->ai_next) {
        if ((sockfdUDP = socket(pUDP->ai_family, pUDP->ai_socktype, pUDP->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (bind(sockfdUDP, pUDP->ai_addr, pUDP->ai_addrlen) == -1) {
            close(sockfdUDP);
            perror("listener: bind");
            continue;
        }

        break;
    }

    if (pUDP == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

	freeaddrinfo(servinfoUDP);

    printf("listener: waiting to recvfrom...\n");
	
	addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfdUDP, bufUDP, MAXDATASIZE-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    
    printf("\nCasino A Phase 2: Received the 1st train vector %s from transit hub" , bufUDP);
	
	printf("\n listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    printf("\n listener: packet is %d bytes long\n", numbytes);
    bufUDP[numbytes] = '\0';
    printf("\n listener: packet contains \"%s\"\n", bufUDP);
	
	/* ##########Decoding the UDP Message#################*/
	
	char str[100], temp2[100] , str2[100];
	strcpy(str2, "");
	strcpy(str, "");
	strcpy(temp2, "");
	char *pp;
	int aa=0,bb=0,cc=0,dd=0, count=0;
	int ii , kk ;
	
	strncpy(str , bufUDP , strlen(bufUDP));
	strcpy(bufUDP , "");
	
	for(ii=1, kk=0;ii<(strlen(str)-2); ii++,kk++)
	{
		temp2[kk] = str[ii];
	}
	temp2[kk]='\0';
	
	pp=strtok(temp,",");
	
	while (pp != NULL) {
		
		count++;
		switch(count)
		{
			case 1: aa = atoi(pp);
					break;
					
			case 2: bb = atoi(pp);
					break;
					
			case 3: cc = atoi(pp);
					break;
					
			case 4: dd = atoi(pp);
					break;
		}
		
		pp = strtok (NULL, ",");
	}
	
	aa = 0;
	bb = bb + b;
	cc = cc + c;
	dd = dd + d;
	
	sprintf(str2, "<%d,%d,%d,%d>", aa , bb , cc ,dd);
	strncpy(sendStringUDP , str2 , strlen(str2));
	
	printf("\nSENDSTRING is: %s\n" , sendStringUDP);
	printf("\n Sending the Vector to Casino B");
	
	if ((rvUDP = getaddrinfo(NULL, CASINOB, &hintsUDP, &servinfoUDP)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUDP));
        return 1;
    }
	
	 for(pUDP = servinfoUDP; pUDP != NULL; pUDP = pUDP->ai_next) {
        if ((sockfdUDP = socket(pUDP->ai_family, pUDP->ai_socktype,pUDP->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

	if (pUDP == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }

    if ((numbytes = sendto(sockfdUDP, sendStringUDP, strlen(sendStringUDP), 0, pUDP->ai_addr, pUDP->ai_addrlen) == -1)) {
		perror("talker: sendto");
        exit(1);
	
	}

    freeaddrinfo(servinfoUDP);

    printf("talker: sent %d bytes to %s\n", numbytes, argv[1]);
    close(sockfdUDP);
/*---------------------------------------------------------------------*/
	
	return 0;
}
	


