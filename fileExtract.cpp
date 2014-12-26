#include <stdio.h>
#include <string.h>

int main()
{
  char buffer[5];
  char sendString[100] = "" , temp[5];
  int size =0, number;
  FILE *fr =  fopen("stop_a.txt","r");
	
	sendString[0]='<';
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	sprintf(temp, "%d", number);
	printf("temp = %s\n" , temp);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	printf("Number: %d \n", number);
	printf("SendString: %s \n", sendString);
	
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	printf("Number: %d \n", number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	printf("SendString: %s \n", sendString);
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	printf("Number: %d \n", number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = ',';
	printf("SendString: %s \n", sendString);
	
	
	fscanf(fr , "%s %s %s %d", buffer , buffer , buffer , &number);
	printf("Number: %d \n", number);
	sprintf(temp, "%d", number);
	strcat(sendString, temp);
	sendString[(strlen(sendString))] = '>';
	printf("SendString: %s \n", sendString);
	
	
    fclose(fr);
	
	

  return 0;
}
