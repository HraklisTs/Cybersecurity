# Introduction

Developed from scratch an asymmetric encryption tool using C, which provides RSA key-pair generation, encryption and decryption. RSA is a public-key cryptosystem that is used for secure data transmission. The security behind RSA relies on the *factoring problem* , and the tasks of this toolkit on some fundamental number theory operations, such as *Modular Exponentiation, Sieve Of Eratosthenes, Extended Euclidean Algorithm* etc.

## **Tasks**
 - ***Key Derivation Function (KDF)*** : <sub>
Implementation of RSA key-pair generation algorithm. Created a function which generates a pool of primes using the Sieve Of Eratosthenes, so as to pick two random primes, compute their product and calculate Euler's totient function φ(n). Thence, choose a prime encryption exponent e, which is prime with φ(n) and it is not bigger than it. Last but not least, calculate d, which is the modular multiplicative inverse of e modulo φ(n) and create the public and private keys which consist of (n, d) and (n, e) , respectively, in this order. 
 
 - ***Data Encryption*** : <sub>
 Implementation of RSA encryption, using the keys generated in the *KDF* in the appropriate endianness format, calculation of modular exponentiation of plaintext and public exponent e modulus n, in order to finally get the ciphertext. 

 - ***Data Decryption*** : <sub>
 Implementation of RSA decryption, using the appropriate one of the two keys generated in the *KDF*, depending on which one was used for encryption previously. Calculate once more the modular exponentiation of ciphertext, in the appropriate endianness format, public/private exponent d modulus n, in order to finally get the plaintext.

## How to build

**Makefile** <sub> 
See *Makefile* for the build.

<sub>
To compile, simply run "make" at the command line in the current directory. This will generate an executable called assign_3. 
This command will use the tool created for public-key encryption, public-key decryption, private-key encryption and private-key decryption.

<sub>
If you want to remove any files that were created after "make" command, run "make clean". 

<sub>
The overall build process is controlled by the "Makefile". It provides the following commands:

``` c
make : Contains all targets described in Task D
make key
make encrypt_pub
make encrypt_priv
make decrypt_pub
make decrypt_priv
make clean
```

## Implementation

**Task A**
``` c
void rsa_keygen(void)
```
Generates an RSA key-pair and saves each key in a different file, using the helping functions below.

``` c
size_t * sieve_of_eratosthenes(int limit, int *primes_sz)
```
Generates an array of primes with predetermined limit ```RSA_SIEVE_LIMIT``` using the theory behind Sieve Of Eratosthenes.
``` c
int gcd(int a, int b)
```
Calculates Greatest Common Divisor of two primes, randomly picked from the array produced by the function above.

``` c
size_t choose_e(size_t fi_n, int lcm)
```
Chooses the public/encryption exponent e, which must be prime with Euler's totient function φ(n).
``` c
size_t mod_inverse(size_t a, size_t b)
```
Calculates the modular multiplicative	inverse of e modulo φ(n), called d.

**Task B**
``` c
void rsa_encrypt(char *input_file, char *output_file, char *key_file)
```
Encrypts an input file using one of the generated keys and dumps the ciphertext into an output file.
In order to use the key provided in the right format, reversing the contents of the key file was necessary, due to endianness issue. Then, calculation of modular exponentiation provided by a helping function created, ```size_t exponentMod(size_t x, size_t y, size_t n) ```,where x is the base, y is the exponent and n is the modulus, in order to complete RSA encryption and store the ciphertext into the output file.


**Task C**
``` c
void rsa_decrypt(char *input_file, char *output_file, char *key_file)
```
Decrypts an input file using the appropriate one of two keys, depending on which was used for the ciphertext encryption, then dumps the plaintext into an output file. Once more, reversing the contents of the input file and the key file was necessary, due to endianness issue. After that, calculation of modular exponentiation, as described in Task B, for RSA decryption completes the task and the plaintext is stored into the output file.
 

**Task D**
Run *make* at the command line, in order to do the operations described on the assignment.


###  Helper functions or macros
``` c
* errPrint():	Prints errors to stdout.
* int lcm(int a, int b):	Calculates Least Common Multiple 
* size_t exponentMod(size_t x, size_t y, size_t n):	Calculates Modular Exponentiation 
```
