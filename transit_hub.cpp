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

using namespace std;

char receivedBytes[100] = "";
static int globalSum=0;
int pid[5];
void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

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
	int numbytes;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	
	int count =0;
	int counter = 0;
	char stop;
	
	struct sockaddr_in client1;
	struct sockaddr_in client2;
	struct sockaddr_in client3;
	struct sockaddr_in client4;
	struct sockaddr_in server;
	struct hostent *he;
	
	he = gethostbyname("nunki.usc.edu"); // get the address of the local server
	socklen_t addr_len;
	addr_len = sizeof(server);
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP
	if ((rv = getaddrinfo("nunki.usc.edu", SERVER, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	
	memset(&server , 0 , sizeof(server)); //populating the socket structure
	server.sin_family = AF_INET;
	server.sin_addr = *((struct in_addr *)he -> h_addr);
	server.sin_port = htons(SERVERPORT);
	
	
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
	
	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	
	//cout<<"server: waiting for connections...\n";
	
	
		
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
		if (!(pid[counter] = fork())) { // this is the child process
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
				printf("Hub Phase1: End of Phase 1\n");
	
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
							globalSum = globalSum + a;
							break;
					
					case 2: b = atoi(p);
							globalSum = globalSum + b;
							break;
					
					case 3: c = atoi(p);
							globalSum = globalSum + c;
							break;
					
					case 4: d = atoi(p);
							globalSum = globalSum + d;
							break;
				}
		
				p = strtok (NULL, ",");
			}	
			//globalSum   = globalSum + (a+b+c+d);
	
			//printf(" a: %d\n b: %d\n c: %d\n d: %d\n",a,b,c,d);
		//printf("Sum is: %d \n" , globalSum);
			
			
	/*---------------------------------------------------------------------------------------*/
			
			if (send(new_fd, "Data Received!!", 13, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		
		close(new_fd);		// parent doesn't need this
		
		counter++;
	}
	
	return 0;
}
