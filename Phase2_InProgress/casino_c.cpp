#include "casino_c.h"

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
	
	struct addrinfo hintsUDP , * servinfoUDP , *pUDP;
	int sockfdUDP;
	int rvUDP ;
	char bufUDP[MAXDATASIZE];
	struct sockaddr_storage their_addr;
	
	struct sockaddr_in clientUdp;
	socklen_t udpClient_len;
	udpClient_len = sizeof(clientUdp);
	
	int a = 0 , b = 0 , c = 0 , d = 0;
	int a2 , b2 , c2 , d2;
	char UCV2[100]= "<0,0,0,0>";
	
	struct sockaddr_in casinoD;
	int phase2Count = 1;
	
	/*Phase 3 Declaration*/
	struct addrinfo hintsUDP3 ,*servinfoUDP3, *pUDP3;
	struct sockaddr_in casinoB3;
	int phase3Count;
	
	struct sockaddr_in clientUdp3;
	socklen_t udpClient_len3;
	udpClient_len3 = sizeof(clientUdp3);
	int sockfdUDP3 , newFdUdp3;
	int rvUDP3 ;
	
	int a3 , b3 , c3 , d3;
	char UCV3[100]= "<0,0,0,0>";
	
/*----------------Code for Reading from file----------------------*/
	char buffer[5];
    char temp[5];
    int size =0, number;
    FILE *fr =  fopen("stop_c.txt","r");
	
	sendString[0]='C';
	sendString[1]='<';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	a = number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	b = number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	c = number;
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	d = number;
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
	printf("Casino C Phase 1: The casino C has dynamic TCP port number %d and IP address %s\n" ,ntohs(client.sin_port),s );

	char temp1[100];
	strcpy(temp1 , "");
	int i,k;
	
	for(i=1,k=0;i<strlen(sendString);i++,k++)
	{
		temp1[k] = sendString[i];
	}
	
	temp1[k]='\0';
	printf("Casino C Phase 1: Sending the casino vector %s to the transit hub\n", temp1);
	
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
	
	printf("Casino C Phase 1: End of Phase 1\n");
	
	/*#############################UDP PHASE 2 BEGINS#########################################*/
	printf("\nCasino C Phase 2: The casino C has static UDP port number %d" , MYUDPPORT);
	
	memset(&hintsUDP, 0, sizeof hintsUDP);
    hintsUDP.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hintsUDP.ai_socktype = SOCK_DGRAM;
    hintsUDP.ai_flags = AI_PASSIVE; // use my IP
	
	memset(&casinoD , 0 , sizeof(casinoD)); //populating the socket structure
	casinoD.sin_family = AF_INET;
	casinoD.sin_addr = *((struct in_addr *)he -> h_addr);
	casinoD.sin_port = htons(CASINODPORT);
	
	while(phase2Count <= 2)
	{
	/*--------------------Receiving Packet from Casino B----------------------------------*/
	
	if ((rvUDP = getaddrinfo("nunki.usc.edu", MYUPD, &hintsUDP, &servinfoUDP)) != 0) {
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

    //printf("\nlistener: waiting to recvfrom...\n");
	
	addr_len = sizeof their_addr;
    if ((numbytes = recvfrom(sockfdUDP, bufUDP, MAXDATASIZE-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
	
    if(phase2Count == 1)
    	printf("\nCasino C Phase 2: Received the 1st train vector %s from Casino B" , bufUDP);
	if(phase2Count == 2)
		printf("\nCasino C Phase 2: Received the 2nd train vector %s from Casino B" , bufUDP);

	
	//printf("\nlistener: got packet from %s\n", inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s));
    //printf("\nlistener: packet is %d bytes long\n", numbytes);
    bufUDP[numbytes] = '\0';
    //printf("\nlistener: packet contains \"%s\"\n", bufUDP);

    close(sockfdUDP);
    
/* ^^^^^^^^^^^^^^^^^^^^^^^^^Decoding the UDP Message^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	
	//printf("\n Phase2: Decoding and Modifying the UDP message in Round 1 of UDP");
	
	char str[100], temp2[100] , str2[100];
	strcpy(str2, "");
	strcpy(str, "");
	strcpy(temp2, "");
	char *pp;
	int aa=0,bb=0,cc=0,dd=0, count=0;
	int ii , kk ;
	
	strncpy(str , bufUDP , strlen(bufUDP));
	strcpy(bufUDP , "");
	strcpy(temp2 , "");
	
	for(ii=1, kk=0;str[ii] != '>'; ii++,kk++)
	{
		temp2[kk] = str[ii];
	}
	temp2[kk]='\0';
	
	//printf("\ntemp2: %s" , temp2);
	pp=strtok(temp2,",");
	
	while (pp != NULL) {
		
		count++;
		switch(count)
		{
			case 1: aa = atoi(pp);
					//printf("\n aa: %d", aa);
					break;
					
			case 2: bb = atoi(pp);
					//printf("\n bb: %d", bb);
					break;
					
			case 3: cc = atoi(pp);
					//printf("\n cc: %d", cc);
					break;
					
			case 4: dd = atoi(pp);
					//printf("\n dd: %d", dd);
					break;
		}
		
		pp = strtok (NULL, ",");
	}
	
	//printf("\n\na: %d || b: %d || c: %d || d: %d \n\n", a,b,c,d);
	//printf("\n\naa: %d || bb: %d || cc: %d || dd: %d \n\n", aa,bb,cc,dd);
	
	cc = 0;
	
	if(phase2Count == 1)
	{
		aa = aa + a;
		bb = bb + b;
		dd = dd + d;
	}
	strcpy(str2,"");
	sprintf(str2, "<%d,%d,%d,%d>", aa , bb , cc ,dd);
	strncpy(sendStringUDP , str2 , strlen(str2));
	//printf("\nSENDSTRING is: %s\n" , sendStringUDP);
	
	/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	
/*------------------------------Sending the Packet to Casino D-----------------------------------*/
	
	
	//printf("\n Sending the Vector to Casino D");
	
	if ((rvUDP = getaddrinfo("nunki.usc.edu", CASINOD, &hintsUDP, &servinfoUDP)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUDP));
        return 1;
    }
	
	 for(pUDP = servinfoUDP; pUDP != NULL; pUDP = pUDP->ai_next) {
        if ((sockfdUDP = socket(pUDP->ai_family, pUDP->ai_socktype,pUDP->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        
        int reuse = 1;
        
        if (setsockopt(sockfdUDP, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        
        break;
    }
	
	if (pUDP == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }
    int l;
    for(l = 0 ; l<100000000 ; l++)
    ;
    
		//printf("\n Just Before SendTo function ");
	//printf("\nSENDSTRING is: %s\n" , sendStringUDP);
	//printf("\n %d " , sockfdUDP);
    if ((numbytes = sendto(sockfdUDP, sendStringUDP, sizeof sendStringUDP, 0, pUDP->ai_addr, pUDP->ai_addrlen) == -1)) {
		perror("talker: sendto");
        exit(1);
	
	}
	if (getsockname(sockfdUDP, (struct sockaddr *)&clientUdp, &udpClient_len) == -1) {
			perror("getsockname() failed");
			return -1;
	}
	printf("\nCasino C Phase 2: The casino C has dynamic UDP port number %d" , ntohs(clientUdp.sin_port));
	/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@Updating the Casino Vector2@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	if(phase2Count == 1)
	{
		a2 = 0;
		b2 = 0;
		c2 = 0;
		d2 = 0;
		strcpy(UCV2 , "");
		
		fileA = fopen("stop_a.txt","r");
		fscanf(fileA , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileA , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileA , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		a2 = number;
		fclose(fileA);
		
		fileB = fopen("stop_b.txt","r");
		fscanf(fileB , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileB , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileB , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		b2 = number;
		fclose(fileB);
	
		sprintf(UCV2, "<%d,%d,%d,%d>", a2 , b2 , c2 ,d2);
	}
	
	if(phase2Count == 2)
	{
		c2 = 0;
		d2 = 0;
		strcpy(UCV2 , "");
		
	
		fileD = fopen("stop_d.txt","r");
		fscanf(fileD , "%s %s %s %d", buffer, buffer , buffer ,  &number);
		fscanf(fileD , "%s %s %s %d", buffer, buffer , buffer ,  &number);
		fscanf(fileD , "%s %s %s %d", buffer, buffer , buffer ,  &number);
		d2= number;
		//printf("\n d2: %d" , d2);
		fclose(fileD);
	
		sprintf(UCV2, "<%d,%d,%d,%d>", a2 , b2 , c2 ,d2);
	}
	
	/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	
	
	printf("\nCasino C Phase 2: Sending the updated train vector %s to casino D." , sendStringUDP);
	printf("\nCasino C Phase 2: The updated casino vector2 at C is %s" ,UCV2 );
    
    
    freeaddrinfo(servinfoUDP);

    //printf("\ntalker: sent %d bytes \n", numbytes);
    
    close(sockfdUDP);
    
    phase2Count++;
}
printf("\nCasino C Phase 2: End of Phase 2\n");
/*##############################################################################################*/

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$UDP Phase3$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/

	printf("\nCasino C Phase 3: The casino C has static UDP port number %d" , CCPHASE3PORT);

	memset(&hintsUDP3, 0, sizeof hintsUDP3);
    hintsUDP3.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hintsUDP3.ai_socktype = SOCK_DGRAM;
    //hintsUDP3.ai_flags = AI_PASSIVE; // use my IP
	
	memset(&casinoB3 , 0 , sizeof(casinoB3)); //populating the socket structure
	casinoB3.sin_family = AF_INET;
	casinoB3.sin_addr = *((struct in_addr *)he -> h_addr);
	casinoB3.sin_port = htons(CBPHASE3PORT);
	
	phase3Count =1;
	while(phase3Count <= 2)
	{
		//printf("\n phase3Count: %d",phase3Count);
		if ((rvUDP3 = getaddrinfo("nunki.usc.edu", CCPHASE3, &hintsUDP3, &servinfoUDP3)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUDP3));
        return 1;
    	}
	
		for(pUDP3 = servinfoUDP3 ; pUDP3 != NULL; pUDP3 = pUDP3->ai_next) {
        	if ((sockfdUDP3 = socket(pUDP3->ai_family, pUDP3->ai_socktype, pUDP3->ai_protocol)) == -1) {
            	perror("listener: socket");
            	continue;
        	}

        	if (bind(sockfdUDP3, pUDP3->ai_addr, pUDP3->ai_addrlen) == -1) {
            	close(sockfdUDP3);
            	perror("listener: bind");
            	continue;
       		 }

        	 break;
    	}

    	if (pUDP3 == NULL) {
        	fprintf(stderr, "listener: failed to bind socket\n");
        	return 2;
    	}

		freeaddrinfo(servinfoUDP3);

    	//printf("listener: waiting to recvfrom...\n");
	
		addr_len = sizeof their_addr;
    	if ((numbytes = recvfrom(sockfdUDP3, bufUDP, MAXDATASIZE-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) {
        	perror("recvfrom");
        	exit(1);
    	}
	
		bufUDP[numbytes] = '\0';
		
		if(1 == phase3Count)
			printf("\nCasino C Phase 3: Received the 1st train vector %s from casino D" , bufUDP);
		if(2 == phase3Count)
			printf("\nCasino C Phase 3: Received the 2nd train vector %s from casino D" , bufUDP);
		
		close(sockfdUDP3);
	
	/* ^^^^^^^^^^^^^^^^^^^^^^^^^Decoding the UDP Message^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
	
		//printf("\n Phase3: Decoding and Modifying the UDP message in Round %d of UDP PHASE3" , phase3Count);
	
		char str[100], temp2[100] , str2[100];
		strcpy(str2, "");
		strcpy(str, "");
		strcpy(temp2, "");
		int ii , kk ;
		char *pp;
		int aa=0,bb=0,cc=0,dd=0, hubNo=0;
		a3 = 0 , b3 = 0 , c3 = 0 , d3 = 0;
	
		strncpy(str , bufUDP , strlen(bufUDP));
		strcpy(bufUDP , "");
		strcpy(temp2 , "");
		strcpy(UCV2 , "");
	
		for(ii=1, kk=0;str[ii] != '>'; ii++,kk++)
		{
			temp2[kk] = str[ii];
		}
		temp2[kk]='\0';
	
		//printf("\ntemp2: %s" , temp2);
		pp=strtok(temp2,",");
	
		while (pp != NULL) {
			//printf("\nInside tokenizning while loop");
			hubNo++;
			//printf("\n Hub Number: %d" , hubNo );
			switch(hubNo)
			{
				case 1: aa = atoi(pp);
						//printf("\n aa: %d", aa);
						break;
					
				case 2: bb = atoi(pp);
						//printf("\n bb: %d", bb);
						break;
					
				case 3: cc = atoi(pp);
						//printf("\n cc: %d", cc);
						break;
					
				case 4: dd = atoi(pp);
						//printf("\n dd: %d", dd);
						break;
			}
		
			pp = strtok (NULL, ",");
		}
	
	
	/*!!!!!!!!!!!!!!!!!!!!!! Calculating the Casino Vector !!!!!!!!!!!!!!!!!!!!!!!!!*/
	
	if(phase3Count == 1)
	{
		
		fileA = fopen("stop_a.txt","r");
		fscanf(fileA , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileA , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileA , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		a3 = number;
		fclose(fileA);
		
		fileB = fopen("stop_b.txt","r");
		fscanf(fileB , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileB , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileB , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		b3 = number;
		fclose(fileB);
		
		fileC = fopen("stop_d.txt","r");
		fscanf(fileC , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileC , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		fscanf(fileC , "%s %s %s %d",buffer , buffer , buffer ,  &number);
		d3 = number;
		fclose(fileC);

		//printf("\n UCV2: %s", UCV2 );
	}
	
	//sprintf(UCV2, "<%d,%d,%d,%d>", a3 , b3 , c3 ,d3);
	
	/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	//printf("\n a3: %d || b3: %d || c3: %d || d3: %d \n\n", a3,b3,c3,d3);
	
	
	cc = 0;
	if(phase3Count == 1)
	{
		aa = aa + a3;
		bb = bb + b3;
		dd = dd + d3;
	}
	
	//printf("\n aa: %d || bb: %d || cc: %d || dd: %d \n\n", aa,bb,cc,dd);
	strcpy(sendStringUDP, "");
	sprintf(str2, "<%d,%d,%d,%d>", aa , bb , cc ,dd);
	strcpy(sendStringUDP , str2);
	//printf("\nSENDSTRING is: %s\n" , sendStringUDP);
	/*------------------------------Sending the Packet to Casino B-----------------------------------*/

	//printf("\n Sending the Vector to Casino B");
	if ((rvUDP3 = getaddrinfo("nunki.usc.edu", CBPHASE3, &hintsUDP3, &servinfoUDP3)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rvUDP3));
        return 1;
    }
	
	 for(pUDP3 = servinfoUDP3; pUDP3 != NULL; pUDP3 = pUDP3->ai_next) {
        if ((sockfdUDP3 = socket(pUDP3->ai_family, pUDP3->ai_socktype,pUDP3->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        
        int reuse = 1;
        
        if (setsockopt(sockfdUDP3, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        

        break;
    }
	
		if (pUDP3 == NULL) {
        fprintf(stderr, "talker: failed to bind socket\n");
        return 2;
    }
	//printf("\n Just Before SendTo function ");
	//printf("\nSENDSTRING is: %s\n" , sendStringUDP);
	//printf("\n %d " , sockfdUDP);
    if ((numbytes = sendto(sockfdUDP, sendStringUDP, sizeof sendStringUDP, 0, pUDP->ai_addr, pUDP->ai_addrlen) == -1)) {
		perror("talker: sendto");
        exit(1);
	}
	if (getsockname(sockfdUDP3, (struct sockaddr *)&clientUdp3, &udpClient_len3) == -1) {
			perror("getsockname() failed");
			return -1;
	}
	printf("\nCasino C Phase 3: The casino C has dynamic UDP port number %d",ntohs(clientUdp3.sin_port));
	printf("\nCasino C Phase 3: Sending the updated train vector %s to casino B." , sendStringUDP);
	printf("\nCasino C Phase 3: The updated casino vector2 at C is %s" , UCV3);
	phase3Count++;
	close(sockfdUDP3);
	}
	printf("\nCasino C Phase 3: End of Phase 3\n");

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
	
	return 0;
}
	


