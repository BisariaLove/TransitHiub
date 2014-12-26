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

#define CAPHASE3PORT 6922
#define CAPHASE3 "6922" 

#define TRANSITHUBPORT3 32922
#define HUB3 "32922"

using namespace std;

char sendString[100] = "";
char sendStringUDP[100]="";
char UCV3[100] = "<0,0,0,0>";

FILE *fileA = NULL , *fileB = NULL , *fileC = NULL , *fileD = NULL;
