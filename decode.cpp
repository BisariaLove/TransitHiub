#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main()
{
	char str[50]= "<10,20,20,40>" , temp[50] ;
	char *p;
	int a=0,b=0,c=0,d=0, i ,k, count=0;
	printf("String is: %s \n", str);
	
	for(i=1, k=0;i<(strlen(str)-1); i++,k++)
	{
		temp[k] = str[i];
	}
	temp[k]='\0';
	printf("Temp String is: %s \n", temp);
	
	
	p=strtok(temp,",");
	
	while (p != NULL) {
		
		count++;
		switch(count)
		{
			case 1: a = atoi(p);
					break;
					
			case 2: b = atoi(p);
					break;
					
			case 3: c = atoi(p);
					break;
					
			case 4: d = atoi(p);
					break;
		}
		
        p = strtok (NULL, ",");
    }
	
	
	printf(" a: %d\n b: %d\n c: %d\n d: %d\n",a,b,c,d);
	
	return 0;
}