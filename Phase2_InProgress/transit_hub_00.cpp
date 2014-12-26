#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <inttypes.h>
#include <fcntl.h>
#include<pthread.h>


#define MAXBUFLEN 1024 //Maximum Size for the buffer
#define SERVERPORT 21922 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define SERVER "21922"

#define CASINOAPORT 5922 //Static UDP port for Casino A
#define CASINOA "5922" //Static UDP port for Casino A


using namespace std;

char receivedBytes[100] = "";
char sendString[100] = "<0,0,0,0>";
static int globalSum=0;
int summ = 0;
FILE *fileA = NULL , *fileB = NULL , *fileC = NULL , *fileD = NULL, *fp = NULL;


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
	int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
	int sockfd_udp , new_fp_udp;
	int numbytes;
	struct addrinfo hints, *servinfo, *p ;
	struct addrinfo hints_upd ,*servinfo_udp, *p_udp;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	int rv_udp;
	
	int count =0;
	int counter = 0;
	char stop;
	
	struct sockaddr_in client_udp;
	socklen_t udpclient_len;
	udpclient_len = sizeof(client_udp);
	
	struct sockaddr_in server;
	struct sockaddr_in casinoA;
	struct hostent *he;
	
	he = gethostbyname("nunki.usc.edu"); // get the address of the local server
	socklen_t addr_len;
	addr_len = sizeof(server);
	
	//Opening the File
	
	
	// TCP Connection
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	
	memset(&server , 0 , sizeof(server)); //populating the socket structure
	server.sin_family = AF_INET;
	server.sin_addr = *((struct in_addr *)he -> h_addr);
	server.sin_port = htons(SERVERPORT);
	
	if ((rv = getaddrinfo("nunki.usc.edu", SERVER, &hints, &servinfo)) != 0) {
		fprintf(stderr, "[transit_hub.cpp]TCP getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
	
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
	
		if (bind(sockfd, (struct sockaddr*) &server, sizeof(server)) == -1) {
		perror("server: bind");
		continue;
		}
	
		break;
	}
		
	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}
	
	freeaddrinfo(servinfo); // all done with this structure
	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}
	
	
		
	while(counter < 4) { // main accept() loop
		
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		
		inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
		if(counter==0)
			printf("Hub Phase 1: The transit hub has TCP Port number %d and IP address %s\n",SERVERPORT , s );
		if (new_fd == -1) {
			//perror("accept");
			continue;
		}
		//printf("server: got connection from %s\n", s);
		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			strcpy(receivedBytes , "");
			if ((numbytes = recv(new_fd, receivedBytes, MAXBUFLEN-1, 0)) == -1) {
				perror("recv");
				exit(1);
			}
	/*-----------------------------Decoding the message---------------------------------------*/
			
			char str[100], temp[100] , str2[100];
			strcpy(str2, "");
			strcpy(str, "");
			strcpy(temp, "");
			strncpy(str , receivedBytes , strlen(receivedBytes));
			strcpy(receivedBytes , "");
			char *p;
			int a=0,b=0,c=0,d=0, i ,k, count=0;
			
			for(i=1, k=0;i<(strlen(str)-1) && str; i++,k++)
			{
				str2[k] = str[i];
				
				if(str[i] == '>')
				{
					k++;
					break;
				}
			}
			
			str2[k] = '\0';
			printf("Hub Phase 1: The transit hub received casino vector from %c %s \n", str[0] , str2);
			if(counter == 3)
			{
				printf("Hub Phase1: End of Phase 1\n");
				printf("\nNew Block\n");
				fp = fopen("a.txt", "r");
				fscanf(fp , "%d" , &summ);
				globalSum = globalSum + summ;
			
				fp = fopen("b.txt", "r");
				fscanf(fp , "%d" , &summ);
				globalSum = globalSum + summ;
			
				fp = fopen("c.txt", "r");
				fscanf(fp , "%d" , &summ);
				globalSum = globalSum + summ;
			
				fp = fopen("d.txt", "r");
				fscanf(fp , "%d" , &summ);
				globalSum = globalSum + summ;
			
				printf("\nTotal sum is: %d\n" , globalSum);
			}
				
	
			for(i=2, k=0;i<(strlen(str)-1); i++,k++)
			{
				temp[k] = str[i];
			}
			temp[k]='\0';
			strcpy(receivedBytes , "");
			//printf("Temp String is: %s \n", temp);
	
	
			p=strtok(temp,",");
	
			while (p != NULL) {
		
				count++;
				switch(count)
				{
					case 1: a = atoi(p);
							fileA = fopen("a.txt", "w");
							fclose(fileA);
							break;
					
					case 2: b = atoi(p);
							fileB = fopen("b.txt", "w");
							fclose(fileB);
							break;
					
					case 3: c = atoi(p);
							fileC = fopen("c.txt", "w");
							fclose(fileC);
							break;
					
					case 4: d = atoi(p);
							fileD = fopen("d.txt", "w");
							fclose(fileD);
							break;
				}
		
				p = strtok (NULL, ",");
			}	
			
			
	
			//printf(" a: %d\n b: %d\n c: %d\n d: %d\n",a,b,c,d);
		//printf("Sum is: %d \n" , globalSum);
			
			
	/*---------------------------------------------------------------------------------------*/
			
			//if (send(new_fd, "Data Received!!", 13, 0) == -1)
				//perror("send");
			close(new_fd);
			counter++;
			exit(0);
		}
		
		close(new_fd);			// parent doesn't need this
		counter++;
	}
	
	
	/*---------------------------------------------------------------------------------------------------*/	
	//UDP Connection
	
	counter++;
	
	if(counter == 6){
	
	printf("\nEntering in the new UDP block\n");
	memset(&hints_upd, 0, sizeof hints_upd);
	hints_upd.ai_family = AF_UNSPEC;
	hints_upd.ai_socktype = SOCK_DGRAM;
	
	memset(&casinoA , 0 , sizeof(casinoA)); //populating the socket structure
	casinoA.sin_family = AF_INET;
	casinoA.sin_addr = *((struct in_addr *)he -> h_addr);
	casinoA.sin_port = htons(CASINOAPORT);
	
	if ((rv_udp = getaddrinfo("nunki.usc.edu", CASINOA, &hints_upd, &servinfo_udp)) != 0) {
        fprintf(stderr, "[transit_hub.cpp]UDP getaddrinfo: %s\n", gai_strerror(rv_udp));
        return 1;
    }
	
	for(p_udp = servinfo_udp; p_udp != NULL; p_udp = p_udp->ai_next) {
        if ((sockfd_udp = socket(p_udp->ai_family, p_udp->ai_socktype, p_udp->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
		
        break;
    }
	
	if (p_udp == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

   
	
	if ((numbytes = sendto(sockfd_udp, sendString, strlen(sendString), 0, p_udp->ai_addr, p_udp->ai_addrlen) == -1)) {
		perror("talker: sendto");
        exit(1);
	
	}
    freeaddrinfo(servinfo_udp);

    printf("talker: sent %d bytes to %s\n", numbytes, sendString);
    close(sockfd_udp);
/*-----------------------------------------------------------------------------------------------------------*/

	}
	
	return 0;
}
