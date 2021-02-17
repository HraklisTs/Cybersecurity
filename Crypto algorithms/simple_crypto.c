#include <stdio.h> 
#include <memory.h>
#include <stdlib.h>
#include <fcntl.h>

#include "simple_crypto.h"

#define size strlen(pt)

char* OTPenc(char* pt, unsigned char* k)
{	 	
	for(int i=0; i<size; ++i)
	{	
		pt[i] = (char)(pt[i]^k[i]);
	}
	printf("[OTP] encrypted: %0x",pt);
	return pt;
}

char* OTPdec(char* pt, unsigned char* k)
{
	for(int i=0; i<size; ++i)
	{	
		pt[i] = (char)(pt[i]^k[i]);
	}
	printf("\n[OTP] decrypted: %s",pt);
	
	return pt;
}


char* caesarEnc(char* pt, int k)
{
	for(int i=0; i<size; ++i)
	{
		pt[i] = pt[i] + k;
	}
	printf("[Caesars] encrypted: %s",pt);
	
	return pt;
}

char* caesarDec(char* pt, int k)
{
	for(int i=0; i<size; ++i)
	{
		pt[i] = pt[i] - k;
	}
	printf("\n[Caesars] decrypted: %s",pt);
	return pt;
}


char* vigEnc(char* pt, unsigned char* k)
{
	int i,j;
	int kLen = strlen((const char*)k);
 	int iLen = strlen(pt);
 	unsigned char newKey[iLen];

    for(i = 0, j = 0; i <= iLen; ++i, ++j)
    {
        if(j == kLen)
            j = 0;
        newKey[i] = k[j];		//New key
    } 
    newKey[i] = '\0';

	for(i = 0; i < strlen(pt); ++i)
	{
		pt[i] = ((pt[i] + k[i]) % 26) + 'A';
	}
	pt[i] = '\0';

	printf("[Vigenère] encrypted: %s",pt );

	return pt;
}

char* vigDec(char* pt, unsigned char* k)
{
	int i;
	for(i = 0; i < strlen(pt); ++i)
	{
		pt[i] = (((pt[i] - k[i]) + 26) % 26) + 'A';
	}
	pt[i] = '\0';

	printf("\n[Vigenère] decrypted: %s",pt);
	return pt;
}


char* skipChars(char* pt)
{	
	int i,j;
	for(i=0; pt[i] != '\0'; ++i)		//skipping special chars
 	{
 		while(!((pt[i]>='a'&& pt[i]<='z') || (pt[i]>='A'&& pt[i]<='Z') || (pt[i]>='0' && pt[i]<='9') || pt[i]=='\0'))
 		{
 			for(j=i; pt[j]!='\0';++j)
 			{
 				pt[j] = pt[j+1];
 			}
 			pt[j] = '\0';
 		}
 	}

 	return pt;
}


char* skipCharsVig(char* pt)
{	
	int i,j;
	for(i=0; pt[i] != '\0'; ++i)		//skipping special chars
 	{
 		while(!((pt[i]>='A'&& pt[i]<='Z') ||pt[i]=='\0'))
 		{
 			for(j=i; pt[j]!='\0';++j)
 			{
 				pt[j] = pt[j+1];
 			}
 			pt[j] = '\0';
 		}
 	}

 	return pt;
}
