#include "transit_hub.h"

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
	int sockfdUdp , newFdUdp;
	
	int numbytes;
	char s[INET6_ADDRSTRLEN];
	struct addrinfo hints, *servinfo, *p ;
	struct addrinfo hintsUdp ,*servinfoUdp, *pUdp;
	struct sockaddr_storage their_addr; // connector's address information

	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char st[INET6_ADDRSTRLEN];
	
	int rv , rvUdp , rvUdp3;
	
	int counter = 0;
	char stop;
	
	struct sockaddr_in clientUdp;
	socklen_t udpClient_len;
	udpClient_len = sizeof(clientUdp);
	
	struct sockaddr_in server;
	struct sockaddr_in casinoA;
	struct hostent *he;
	
	char bufUDP[MAXDATASIZE];
	
	he = gethostbyname("nunki.usc.edu"); // get the address of the local server
	
	socklen_t addr_len;
	addr_len = sizeof(server);
	
	int phase2Count = 1 ;
	
	/*Phase 3 Declaration*/
	struct addrinfo hintsUdp3 ,*servinfoUdp3, *pUdp3;
	struct sockaddr_in casinoD3;
	int phase3Count =1;
	
	struct sockaddr_in clientUdp3;
	socklen_t udpClient_len3;
	udpClient_len3 = sizeof(clientUdp3);
	int sockfdUdp3 , newFdUdp3;
	
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
	
	
	//Temporary Variables for Decoding the Received Messages
	char str[100], temp[100] , str2[100];
	char *tok;
	int a=0,b=0,c=0,d=0, i ,k, count=0;
	
	sin_size = sizeof their_addr;
	strcpy(st, "");
	inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),st, sizeof st);	
	printf("\nHub Phase 1: The transit hub has TCP Port number %d and IP address %s\n",SERVERPORT , st );
	
	while(counter < 4) 
	{
		
		count = 0;
		strcpy(receivedBytes , "");
		
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
			
		if ((numbytes = recv(new_fd, receivedBytes, MAXBUFLEN-1, 0)) == -1) {
				perror("recv");
				exit(1);
		}
	
		//Initialization Block
		strcpy(str2, "");
		strcpy(str, "");
		strcpy(temp, "");
		
		//Copying the Received bytes into a temporary work
		strncpy(str , receivedBytes , strlen(receivedBytes));
		
		
		for(i=1, k=0;i<(strlen(str)-1) && str; i++,k++)
		{
			str2[k] = str[i];
				
			if(str[i] == '>')
			{
				k++;
				break;
			}
		}
		
		str2[k] = '>';
		str2[++k] = '\0';
		
		printf("Hub Phase 1: The transit hub received casino vector from %c %s \n", str[0] , str2);
		
		for(i=2, k=0;i<(strlen(str)-1); i++,k++)
		{
			temp[k] = str[i];
		}
		temp[k]='\0';
		strcpy(receivedBytes , "");
		
		tok=strtok(temp,",");
		
		
		while (tok != NULL) 
		{
			count++;
			switch(count)
			{
				case 1: a = atoi(tok);
						globalSum  = globalSum + a;
						break;
					
				case 2: b = atoi(tok);
						globalSum  = globalSum + b;
						break;
					
				case 3: c = atoi(tok);
						globalSum  = globalSum + c;
						break;
					
				case 4: d = atoi(tok);
						globalSum  = globalSum + d;
						break;
			}
		
			tok = strtok (NULL, ",");
		}
		
		wagonsCount = globalSum/10 ; 
		int rem = globalSum %10;
		if(rem>0)
		++wagonsCount;
		
		close(new_fd);	
		counter++;
			
	}
	printf("Hub Phase 1: End of Phase 1\n");

//TCP Phase Ends

/*#############################UDP PHASE 2 BEGINS#########################################*/

memset(&hintsUdp, 0, sizeof hintsUdp);
hintsUdp.ai_family = AF_UNSPEC;
hintsUdp.ai_socktype = SOCK_DGRAM;

memset(&casinoA , 0 , sizeof(casinoA)); //populating the socket structure
casinoA.sin_family = AF_INET;
casinoA.sin_addr = *((struct in_addr *)he -> h_addr);
casinoA.sin_port = htons(CASINOAPORT);

/*--------------------Sending Packet to Casino A--------------------------------*/
	while(phase2Count <= 2)
	{
		if ((rvUdp = getaddrinfo("nunki.usc.edu", CASINOA, &hintsUdp, &servinfoUdp)) != 0) {
				fprintf(stderr, "[transit_hub.cpp]TCP getaddrinfo: %s\n", gai_strerror(rvUdp));
				return 1;
			}
	
		for(pUdp = servinfoUdp; pUdp != NULL; pUdp = pUdp->ai_next) {

			if ((sockfdUdp = socket(pUdp->ai_family, pUdp->ai_socktype,pUdp->ai_protocol)) == -1) {
				perror("server: socket");
				continue;
			}
	
			break;
		}


		
		
		if (pUdp == NULL) {
        	fprintf(stderr, "talker: failed to bind socket\n");
        	return 2;
		}

		if ((numbytes = sendto(sockfdUdp, sendString, strlen(sendString), 0, pUdp->ai_addr, pUdp->ai_addrlen) == -1)) {
			perror("talker: sendto");
        	exit(1);
	
		}
		if (getsockname(sockfdUdp, (struct sockaddr *)&clientUdp, &udpClient_len) == -1) {
				perror("getsockname() failed");
				return -1;
		}

		printf("\nHub Phase 2: The transit hub has dynamic UDP port number %d" ,ntohs(clientUdp.sin_port));
		if(phase2Count == 1){
			printf("\nHub Phase 2: The transit hub has dynamic UDP port number %d" ,ntohs(clientUdp.sin_port));
			printf("\nHub Phase 2: Sending the train vector with wagons %d to casino A", wagonsCount);
			printf("\nHub Phase 2: The transit hub has static UDP port number %d" , TRANSITHUBPORT );
		}
		freeaddrinfo(servinfoUdp);
		//printf("talker: sent %d bytes to %s\n", numbytes, sendString);
		close(sockfdUdp);

/*----------------------------Receiving Packet from D----------------------------------------*/
		if ((rvUdp = getaddrinfo("nunki.usc.edu", TRANSITHUB, &hintsUdp, &servinfoUdp)) != 0) {
        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUdp));
       	 	return 1;
   		 }
		
		for(pUdp = servinfoUdp; pUdp != NULL; pUdp = pUdp->ai_next) {
		
        	if ((sockfdUdp = socket(pUdp->ai_family, pUdp->ai_socktype, pUdp->ai_protocol)) == -1) {
            	perror("listener: socket");
            	continue;
       		 }

        	if (bind(sockfdUdp, pUdp->ai_addr, pUdp->ai_addrlen) == -1) {
            	close(sockfdUdp);
            	perror("listener: bind");
            	continue;
        	}

        	break;
    	}

    	if (pUdp == NULL) {
        	fprintf(stderr, "listener: failed to bind socket\n");
        	return 2;
    	}

		freeaddrinfo(servinfoUdp);

    	//printf("listener: waiting to recvfrom...\n");
	
		addr_len = sizeof their_addr;
		
    	if ((numbytes = recvfrom(sockfdUdp, bufUDP, MAXDATASIZE-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        	perror("recvfrom");
        	exit(1);
    	}
    	
        if(phase2Count ==1)  
    		printf("\nHub Phase 2: Received the 1st train vector %s from casino D ", bufUDP);
	
		if(phase2Count == 2)
			printf("\nHub Phase 2: Received the 2nd train vector %s from casino D ", bufUDP);
			
		bufUDP[numbytes] = '\0';
		strcpy(sendString , bufUDP);
			
		//printf("\n listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    	//printf("\n listener: packet is %d bytes long\n", numbytes);
    	
    	//printf("\n listener: packet contains \"%s\"\n", bufUDP);
	
		close(sockfdUdp);
		phase2Count++ ; 
	}
	printf("\nHub Phase 2: End of Phase 2\n");
/*#########################################################################################*/


/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$UDP Phase3$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

strcpy(sendString , "<0,0,0,0>");
//printf("\nSendString: %s" , sendString);

memset(&hintsUdp3, 0, sizeof hintsUdp3);
hintsUdp3.ai_family = AF_UNSPEC;
hintsUdp3.ai_socktype = SOCK_DGRAM;

memset(&casinoD3 , 0 , sizeof(casinoD3)); //populating the socket structure
casinoD3.sin_family = AF_INET;
casinoD3.sin_addr = *((struct in_addr *)he -> h_addr);
casinoD3.sin_port = htons(CDPHASE3PORT);

//printf("\n Just before the While loop\n");

while(phase3Count <= 2)
	{
		//printf("\n phase3Count: %d" , phase3Count);
		if ((rvUdp3 = getaddrinfo("nunki.usc.edu", CDPHASE3, &hintsUdp3, &servinfoUdp3)) != 0) {
				fprintf(stderr, "[transit_hub.cpp]TCP getaddrinfo: %s\n", gai_strerror(rvUdp3));
				return 1;
			}
			//printf("\n Before the For Loop");
	
		for(pUdp3 = servinfoUdp3; pUdp3 != NULL; pUdp3 = pUdp3->ai_next) {
				
				//printf("\n Inside the for loop");
			if ((sockfdUdp3 = socket(pUdp3->ai_family, pUdp3->ai_socktype,pUdp3->ai_protocol)) == -1) {
				perror("server: socket");
				continue;
			}
			
			int reuse = 1;
        
        	if (setsockopt(sockfdUdp3, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
            	perror("setsockopt");
            	exit(1);
        	}
			//printf("\n After ");
			break;
		}
		
		
		if (pUdp3 == NULL) {
        	fprintf(stderr, "talker: failed to bind socket\n");
        	return 2;
		}
		//printf("\n Before SendTo function");
		
		int l;
    	for(l = 0 ; l<100000000 ; l++)
    	;

		if ((numbytes = sendto(sockfdUdp3, sendString, strlen(sendString), 0, pUdp3->ai_addr, pUdp3->ai_addrlen) == -1)) {
			perror("talker: sendto");
        	exit(1);
	
		}
		if (getsockname(sockfdUdp3, (struct sockaddr *)&clientUdp3, &udpClient_len3) == -1) {
				perror("getsockname() failed");
				return -1;
		}
		if(phase3Count == 1){
			printf("\nHub Phase 3: The transit hub has dynamic UDP port number %d" , ntohs(clientUdp3.sin_port));
			printf("\nHub Phase 3: Sending the train vector %s to casino D",sendString);
			printf("\nHub Phase 3: The transit hub has static UDP port number %d" , TRANSITHUBPORT3 );
		}

		freeaddrinfo(servinfoUdp3);
		//printf("talker: sent %d bytes to %s\n", numbytes, sendString);
		close(sockfdUdp3);
		
		

/*----------------------------Receiving Packet from A----------------------------------------*/
		if ((rvUdp3 = getaddrinfo("nunki.usc.edu", HUB3, &hintsUdp3, &servinfoUdp3)) != 0) {
        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUdp3));
       	 	return 1;
   		 }
		
		for(pUdp3 = servinfoUdp3; pUdp3 != NULL; pUdp3 = pUdp3->ai_next) {
		
        	if ((sockfdUdp3 = socket(pUdp3->ai_family, pUdp3->ai_socktype, pUdp3->ai_protocol)) == -1) {
            	perror("listener: socket");
            	continue;
       		 }

        	if (bind(sockfdUdp3, pUdp3->ai_addr, pUdp3->ai_addrlen) == -1) {
            	close(sockfdUdp3);
            	perror("listener: bind");
            	continue;
        	}

        	break;
    	}

    	if (pUdp3 == NULL) {
        	fprintf(stderr, "listener: failed to bind socket\n");
        	return 2;
    	}

		freeaddrinfo(servinfoUdp3);

    	//printf("listener: waiting to recvfrom...\n");
	
		addr_len = sizeof their_addr;
		
    	if ((numbytes = recvfrom(sockfdUdp3, bufUDP, MAXDATASIZE-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        	perror("recvfrom");
        	exit(1);
    	}
    	
    	bufUDP[numbytes] = '\0' ;
        if(phase3Count ==1)  
    		printf("\nHub Phase 3: Received the 1st train vector %s from casino A", bufUDP);
	
		if(phase3Count == 2)
			printf("\nHub Phase 3: Received the 1st train vector %s from casino A", bufUDP);

		strcpy(sendString , bufUDP);
			
		//printf("\n listener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    	//printf("\n listener: packet is %d bytes long\n", numbytes);
    	
    	//printf("\n listener: packet contains \"%s\"\n", bufUDP);
		//printf("\nReceived Packet: %s" , bufUDP);
		close(sockfdUdp3);
		phase3Count++ ; 
	}
	printf("\nHub Phase 3: End of Phase 3\n");


/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
	return 0;
}
