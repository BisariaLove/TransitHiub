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

#define MYUDPPORT 9922
#define MYUPD "9922"

#define CASINODPORT 11922
#define CASINOD "11922"

#define CCPHASE3PORT 10922
#define CCPHASE3 "10922" 

#define CBPHASE3PORT 8922
#define CBPHASE3 "8922" 

using namespace std;

char sendString[100] = "";
char sendStringUDP[100] = "";
char UTV2[100]= "<0,0,0,0>";
char UCV3[100] ="<0,0,0,0>";

FILE *fileA = NULL , *fileB = NULL , *fileC = NULL , *fileD = NULL;
