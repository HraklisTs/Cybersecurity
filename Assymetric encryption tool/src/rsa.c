#include "rsa.h"
#include "utils.h"

/*
 * Sieve of Eratosthenes Algorithm
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t *
sieve_of_eratosthenes(int limit, int *primes_sz)
{	
	int i, j, A[limit+1];
	size_t *primes;
	int count = 0;

	for(i =2 ; i <= limit; i++)
		A[i] = i;

	i=2;
	while((i*i) <= limit)
	{
		if(A[i] != 0)
		{
			for(j=2; j < limit; j++)
			{
				if(A[i]*j > limit)
					break;
				else
					A[A[i]*j] = -1;
			}
		}
		i++;
	}
	primes = malloc(sizeof(int)*limit);
	j=0;
	for(i = 2; i <= limit; i++)
	{
		if(A[i]!= -1)
		{
			count++;
			primes[j++] = A[i];
		}
	}

	*primes_sz = count;
	
	return primes;
}
/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int
gcd(int a, int b)
{
	if(b == 0)
		return a;
	return gcd(b, a % b);
}


/*
 * Least Common Multiple
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the LCM
 */
int
lcm(int a, int b)
{		
	return (a / gcd(a, b)) * b;	
}


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t fi_n, int lcm)
{
	size_t e;
	size_t *primes;
	int primes_sz;
	size_t tmp;	

	int i = 1;
	primes = sieve_of_eratosthenes(fi_n, &primes_sz);

	tmp = primes[rand() % lcm];	/* e must be between 1 - lcm */
	while(1)
	{

		if((tmp %fi_n != 0) && (gcd(tmp, fi_n) == 1))
		{
			e = tmp;
			return e;
		}
		else
		{
			i++;
			tmp = primes[rand() % lcm];
		}
	}

	return e;	
}


size_t
mod_inverse(size_t a, size_t b)
{
	int x;
	a = a%b;
	for(x =1 ; x<b; x++)
		if((a*x)%b == 1)
			return (size_t)x;
}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void)
{	
	size_t p, q, n, fi_n, e, d, *primes;
	long f_size = 0;
	int primes_sz;
	int lc;
	FILE *fp;
	srand(time(0));

	primes = malloc(sizeof(size_t) * RSA_SIEVE_LIMIT/2);
	primes = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, &primes_sz);

	p = primes[rand() % primes_sz]; 
	q = primes[rand() % primes_sz];
	n = p * q;

	fi_n = (p - 1) * (q - 1);
	lc = lcm(p, q);

	e = choose_e(fi_n, lc);
	d = mod_inverse(e,fi_n);

	if(!(fp = fopen("../files/public.key", "w"))) errPrint();
	if(!fwrite(&n, 1, sizeof(n), fp))	errPrint();	
	if(!fwrite(&d, 1, sizeof(d), fp))	errPrint();
	fclose(fp);

	if(!(fp = fopen("../files/private.key", "w"))) errPrint();
	if(!fwrite(&n, 1, sizeof(n), fp))	errPrint();	
	if(!fwrite(&e, 1, sizeof(e), fp))	errPrint();	
	fclose(fp);

	free(primes);
}


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_encrypt(char *input_file, char *output_file, char *key_file)
{
	printf("Encrypting . . .\n");
	unsigned char* key = NULL;	/* Key from KDF */
	unsigned char* pt = NULL;
	size_t ct_len = 0;	/* Cipher's length */
	size_t key_len = 0;	/* Key length from key_file */
	size_t  n,e,f_size = 0;	/* Plaintext size */
	int i;

	FILE *fp;

	/* Read plaintext from input file */
	if(!(fp = fopen(input_file,"r")))	errPrint();
	if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
	f_size = ftell(fp);	/* Size of file */
	if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */
	pt = malloc(f_size * sizeof(char));
	if(!fread(pt, 1, f_size, fp))	errPrint();

	fclose(fp);

	/* Read key from key file */	
	if(!(fp = fopen(key_file,"r")))	errPrint();
	if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
	key_len = ftell(fp);	/* Size of file */
	if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */
	key = malloc(key_len * sizeof(char));
	if(!fread(key, 1, key_len, fp))	errPrint();
	fclose(fp);

	n = key[0] | key[1] << 8 | key[2] << 16 | key[3] << 24 | key[4] << 32 | key[5] << 40 | key[6] << 48 | key[7] << 56;
	key+=8;
	e = key[0] | key[1] << 8 | key[2] << 16 | key[3] << 24 | key[4] << 32 | key[5] << 40 | key[6] << 48 | key[7] << 56;	

	size_t res;	 /* Ciphertext */

	/* Store ciphertext to output file */
	if(!(fp = fopen(output_file,"w")))	errPrint();
	
	/* Calculate m^e for RSA encryption */
	for(i=0; i < f_size; i++)
	{
		res = exponentMod((size_t)pt[i], e, n);
		if(!fwrite(&res, 1, 8, fp))	errPrint();
	}
	fclose(fp);
	free(pt);
}


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_decrypt(char *input_file, char *output_file, char *key_file)
{
	printf("Decrypting . . .\n");
	unsigned char* key = NULL;	/* Key from KDF */
	size_t key_len = 0;	/* Key length from key_file */
	size_t e,d,n,f_size = 0;	/* Plaintext size */
	int i = 0;

	FILE *fp;

	/* Read ciphertext from input file */
	if(!(fp = fopen(input_file,"r")))	errPrint();
	if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
	f_size = ftell(fp);	/* Size of file */
	if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */
	
	unsigned char* ct;
	ct = malloc(sizeof(char)*f_size);
	if(!fread(ct, 1, f_size, fp))	errPrint();	
	fclose(fp);

	size_t buffer_ct[f_size/8];
	for(i = 0; i < f_size; i+=8)
	{
		buffer_ct[i/8] = ct[i] | ct[i+1] << 8 | ct[i+2] << 16 | ct[i+3] << 24 | ct[i+4] << 32 | ct[i+5] << 40 | ct[i+6] << 48 | ct[i+7] << 56;
	}
	
	/* Read key from key file */
	if(!(fp = fopen(key_file,"r")))	errPrint();
	if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
	key_len = ftell(fp);	/* Size of file */
	
	key = malloc(sizeof(char) * key_len);
	if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */
	if(!fread(key, 1, key_len, fp))	errPrint();
	fclose(fp);

	n = key[0] | key[1] << 8 | key[2] << 16 | key[3] << 24 | key[4] << 32 | key[5] << 40 | key[6] << 48 | key[7] << 56;
	key+=8;
	d = key[0] | key[1] << 8 | key[2] << 16 | key[3] << 24 | key[4] << 32 | key[5] << 40 | key[6] << 48 | key[7] << 56;

	/* Calculate modular exponentiation for RSA decryption */
	unsigned char res[f_size/8];
	for(i=0; i < f_size/8; i++)
	{
		res[i] = (unsigned char)exponentMod(buffer_ct[i], d, n);
	}

	/* Store plaintext to output file */
	if(!(fp = fopen(output_file,"w")))	errPrint();
	if(!fwrite((unsigned char*)res, 1, f_size/8, fp))	errPrint();
	fclose(fp);

	free(ct);
}