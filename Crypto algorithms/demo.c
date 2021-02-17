#include <stdio.h> 
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <assert.h>

#include "simple_crypto.h"
#define size strlen(input)

int main() 
{
	char input[100];
	int key;
 	unsigned char* buff = malloc(sizeof(unsigned char)*size);

	/*********************** One-time pad ***********************/
	printf("[OTP] input: ");
 	fgets(input,sizeof(input),stdin);
 	skipChars(input);
	int fd = open("/dev/urandom", O_RDONLY);	//generate random key
	assert(fd>=0);	
	int k = read(fd,buff,size*sizeof(unsigned char));
	assert(k>=0);
	close(fd);
	
	OTPenc(input,buff);		
	OTPdec(input,buff);


	/*********************** Caesar's cipher ***********************/
	printf("\n[Caesars] input: ");
 	fgets(input,sizeof(input),stdin);
 	printf("[Caesars] key: ");
 	scanf("%d",&key);

 	skipChars(input);
 	caesarEnc(input,key);
	caesarDec(input,key);

	/*********************** Vigenère’s cipher **********************/
	printf("\n[Vigenère] input: ");
 	scanf("%s",input);
 	printf("[Vigenère] key: ");
 	scanf("%s",buff);

 	skipCharsVig(input);
 	vigEnc(input,buff); 	
	vigDec(input,buff);

    return 0; 
}