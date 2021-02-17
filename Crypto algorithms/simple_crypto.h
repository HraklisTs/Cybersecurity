char* OTPenc(char* pt, unsigned char* k); 

char* OTPdec(char* pt, unsigned char* k);

char* caesarEnc(char* pt, int k);

char* caesarDec(char* pt, int k);

char* vigEnc(char* pt, 	unsigned char* k);

char* vigDec(char* pt, unsigned char* k);

char* skipChars(char* pt);

char* skipCharsVig(char* pt);