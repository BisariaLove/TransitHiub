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

#define CASINOAPORT 5922 //Static UDP port for Casino A Phase2
#define CASINOA "5922" //Static UDP port for Casino A Phase2

#define TRANSITHUBPORT 31922 //Static UDP port for Transit Hub Phase2
#define TRANSITHUB "31922" //Static UDP port for Transit Hub Phase2

#define TRANSITHUBPORT3 32922//Static UDP port for Transit Hub Phase3
#define HUB3 "32922" //Static UDP port for Transit Hub Phase3

#define CDPHASE3PORT 12922
#define CDPHASE3 "12922"

#define MAXDATASIZE 100 // max number of bytes we can get at once


char receivedBytes[100] = "";
char sendString[100] = "<0,0,0,0>";
char UTV2[100]= "<0,0,0,0>";
static int globalSum=0;
int wagonsCount=0;
int summ = 0;
FILE *fileA = NULL , *fileB = NULL , *fileC = NULL , *fileD = NULL, *fp = NULL;
using namespace std;
