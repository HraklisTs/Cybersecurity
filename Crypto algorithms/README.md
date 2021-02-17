## **Introduction** ##
This assignment is about creating a cryptographic library named *simple_crypto,* which will provide the implementation of three fundamental algorithms, (i) ​ *One-time pad*​ , (ii) ​ *Caesar’s cipher and* (iii) ​ *Vigenère’s cipher​* , in programming language *C*.

**How to build**
-----------------
See *Makefile* for the build scripts.

To build the command line, simply run *make* at the command line in the current directory.

This will generate an executable called demo. Then to run the program, run ./demo.
If you want to remove any files that were created after *make* command, run *make clean*. 
 
**Makefile**
The overall build process is controlled by the *Makefile*. It provides the following commands:

*make*
*make clean*

**Implementation**
---------------------
The header file *simple_crypto.h* file contains function declaration, and more specifically :

 - char* OTPenc(char* , unsigned char* )
 - char* OTPdec(char* , unsigned char* )
 - char* caesarEnc(char* , int )
 - char* caesarDec(char* , int )
 - char* vigEnc(char* , unsigned char* )
 - char* vigDec(char* , unsigned char* )
 - char* skipChars(char* )
 - char* skipCharsVig(char* )
 
The *simple_crypto.c* file contains the implementation of the above functions in C, specifically:
  
- char* OTPenc(char* pt, unsigned char* k) is the encryption function for OTP algorithm, which takes as arguments the plaintext given by the user and a random generated key using *"/dev/urandom"* and XOR-ing them, then prints the encrypted message in hex format, and returns the output of the operation.

- char* OTPdec(char* pt, unsigned char* k) is the decryption function for OTP algorithm, which takes as arguments the encrypted message and the random key used to encrypt the plaintext, and by XOR-ing them again, provides the decrypted message, which was the plaintext user gave as input from terminal. Then, function prints and returns the decrypted message.

- char* caesarEnc(char* pt, int k) is the encryption function for Caesar's algorithm, which takes as arguments the plaintext and the key given by user. In this function, each byte/character of the plaintext is replaced by a byte/character found at *k* number of positions down the ASCII set, so it shifts them left, then prints and returns the output, which is the encrypted message.

- char* caesarDec(char* pt, int k) is the decryption function for Caesar's algorithm. Takes as arguments the encrypted message and the key used, shifts right each byte/character found in the encrypted message according to fixed *k* number, then prints and returns the output, aka the decrypted message.

-  char* vigEnc(char* pt, unsigned char* k) is the encryption function for Vigenère's algorithm, which takes as arguments the plaintext and the key-phrase given by user. In this algorithm, key must be the same size as plaintext, so at first it repeatedly appends the key-phrase until it matches plaintext's size. Then according to algorithm's documentation, the first letter of the plaintext is used as column indicator and the first letter of the key is used as row indicator, so the first letter of the ciphertext will be the letter at the point where the selected column meets the selected row, etc. For this operation, I used the *"%26* due to the 26 letters of alphabet *+'A'* because the only permitted characters are A-Z. After this operation, function prints and returns the ciphertext.

- char* vigEnc(char* pt, unsigned char* k) is the decryption function for Vigenère's algorithm, which takes as arguments the ciphertext and the key-phrase used. I used *(First letter of cipher text - First letter of key + 26) % 26) + 'A'* in order to decrypt the message. Then function prints and returns the decrypted message.

-  char* skipChars(char* pt) is a helping function, which takes as argument the plaintext and skips special characters that we were not allowed to print in the output of the first two algorithms. In other words, everything but characters/numbers A-Z , a-z, 0-9 are not permitted.

- char* skipCharsVig(char* pt) is a helping function for Vigenère's algorithm, which takes as argument the plaintext and skips any character, but A-Z, according to our implementation details.

Also I have used a macro *#define size strlen(pt)*,
with which I count every time I use *size* the length of the plaintext.

 The *demo.c* file contains all the above function calls, plus any extra interaction needed with user, so he/she could give the plaintexts and the keys needed.