#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>
#include <openssl/rand.h>


#define BLOCK_SIZE 16
#define CMAC_SIZE 16


/* function prototypes */
void print_hex(unsigned char *, size_t);
void print_string(unsigned char *, size_t); 
void usage(void);
void check_args(char *, char *, unsigned char *, int, int);
unsigned char* keygen(unsigned char *, int, int);
unsigned char* encrypt(unsigned char *, int, unsigned char *, int );
unsigned char* decrypt(unsigned char *, int, unsigned char *, int);
unsigned char* gen_cmac(unsigned char *, size_t, unsigned char *, int);
int verify_cmac(unsigned char *, unsigned char *);
void errPrint();


int SHA_DIGEST_LENGTH = 0;



/*
 * Prints the hex value of the input
 * 16 values per line
 */
void
print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("\n");
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}


/*
 * Prints the input as string
 */
void
print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}

/*
 * Print errors to stdout
 *
 */
void
errPrint()
{
	ERR_print_errors_fp(stderr);
	exit(EXIT_FAILURE);
}

/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_1 -i in_file -o out_file -p passwd -b bits" 
	        " [-d | -e | -s | -v]\n"
	    "    assign_1 -h\n"
	);
	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -p    psswd   Password for key generation\n"
	    " -b    bits    Bit mode (128 or 256 only)\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -s            Encrypt+sign input and store results to output\n"
	    " -v            Decrypt+verify input and store results to output\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);
}


/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void
check_args(char *input_file, char *output_file, unsigned char *password, 
    int bit_mode, int op_mode)
{
	if (!input_file) {
		printf("Error: No input file!\n");
		usage();
	}

	if (!output_file) {
		printf("Error: No output file!\n");
		usage();
	}

	if (!password) {
		printf("Error: No user key!\n");
		usage();
	}

	if ((bit_mode != 128) && (bit_mode != 256)) {
		printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Generates a key using a password
 */
unsigned char*
keygen(unsigned char *password, int l_pass, int bit_mode)
{
	/* Task A */
	printf("Wait for key generation\n");
	unsigned char* k = malloc(SHA_DIGEST_LENGTH);

	if(!(EVP_Digest(password, l_pass, k, NULL, EVP_sha1(), NULL))) errPrint();
	printf("Key generated! \n");

	return k;
}


/*
 * Encrypts the data
 */
unsigned char*
encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key, int bit_mode)
{
	/* Task B */
	printf("Encrypting. . .\n");
	int outl, cipher_len;
	unsigned char* cipher = NULL;
	cipher_len = ceil((float) plaintext_len / (float) BLOCK_SIZE) * BLOCK_SIZE;
	cipher = malloc(cipher_len * sizeof(char));
	EVP_CIPHER_CTX	*ctx;

	if(!(ctx = EVP_CIPHER_CTX_new())) errPrint();	/* Create and init context */

	if(bit_mode == 128)	/* Initialise the encryption operation for 128 and 256 bits AES*/
	{
		if(!(EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))) errPrint();
	}
	else if(bit_mode == 256)
	{
		if(!(EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL))) errPrint();		
	}
	else
	{
		printf("Bit mode length %d is not supported!\n", bit_mode);
		abort();
	}

	/* Provide the message to be encrypted, and obtain the encrypted output */
	if(!(EVP_EncryptUpdate(ctx, cipher, &outl, plaintext, plaintext_len))) errPrint();
	cipher_len = outl; 

	if(!(EVP_EncryptFinal_ex(ctx, cipher + outl, &outl))) errPrint();
	cipher_len += outl;

	EVP_CIPHER_CTX_cleanup(ctx);
	printf("Successfully encrypted!\n");

	return cipher;
}


/*
 * Decrypts the data and returns the plaintext size
 */
unsigned char*
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key, int bit_mode)
{
	/* Task C */
	printf("Decrypting. . .\n");
	int plaintext_len, outl, i;

	plaintext_len = ciphertext_len;
	unsigned char* pt_buff = malloc(ceil((plaintext_len) * sizeof(char)));
	EVP_CIPHER_CTX	*ctx;

	if(!(ctx = EVP_CIPHER_CTX_new())) errPrint();	/* Create and init context */
	if(bit_mode == 128)	/* Initialise the decryption operation for 128 and 256 bits AES*/
	{
		if(!(EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL))) errPrint();
	}
	else if(bit_mode == 256)
	{
		if(!(EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL))) errPrint();		
	}
	else
	{
		printf("Bit mode length %d is not supported!\n", bit_mode);
		abort();
	}

	/* Provide the message to be encrypted, and obtain the encrypted output */
	if(!(EVP_DecryptUpdate(ctx, pt_buff, &outl, ciphertext, ciphertext_len))) errPrint();
	plaintext_len = outl; 

	if(!(EVP_DecryptFinal_ex(ctx, pt_buff + outl, &outl))) errPrint();
	plaintext_len += outl;

	unsigned char* pt = malloc(plaintext_len * sizeof(char));
	for(i = 0; i < plaintext_len; i++)
	{
		pt[i] = pt_buff[i];
	}

	EVP_CIPHER_CTX_cleanup(ctx);
	printf("Successfully decrypted!\n");
	free(pt_buff);

	return pt;
}


/*
 * Generates a CMAC
 */
unsigned char*
gen_cmac(unsigned char *data, size_t data_len, unsigned char *key, int bit_mode)
{
	/*  Task D */
	printf("Signing Data. . .\n");
	unsigned char* cmac = malloc(sizeof(char) * CMAC_SIZE);
	size_t l_cmac;
	
	CMAC_CTX *ctx;
	
	if(!(ctx = CMAC_CTX_new()))	errPrint();
	if(!CMAC_Init(ctx, key, CMAC_SIZE, EVP_aes_128_ecb(), NULL))	errPrint();
	if(!CMAC_Update(ctx, data, data_len))	errPrint();
	if(!CMAC_Final(ctx, cmac, &l_cmac))	errPrint();

	CMAC_CTX_free(ctx);
	printf("CMAC generated!\n");
	return cmac;
}


/*
 * Verifies a CMAC
 */
int
verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
	/* Task E */
	
	int verify = 0;

	verify = strcmp(cmac1, cmac2);
	
	return verify;
}


/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with 
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int
main(int argc, char **argv)
{
	int opt;			/* used for command line arguments */
	int bit_mode;			/* defines the key-size 128 or 256 */
	int op_mode;			/* operation mode */
	char *input_file;		/* path to the input file */
	char *output_file;		/* path to the output file */
	unsigned char *password;	/* the user defined password */

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	password = NULL;
	bit_mode = -1;
	op_mode = -1;


	/*
	 * Get arguments
	 */
	while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
		switch (opt) {
		case 'b':
			bit_mode = atoi(optarg);
			break;
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'p':
			password = (unsigned char *)strdup(optarg);
			break;
		case 'd':
			/* if op_mode == 1 the tool decrypts */
			op_mode = 1;
			break;
		case 'e':
			/* if op_mode == 1 the tool encrypts */
			op_mode = 0;
			break;
		case 's':
			/* if op_mode == 1 the tool signs */
			op_mode = 2;
			break;
		case 'v':
			/* if op_mode == 1 the tool verifies */
			op_mode = 3;
			break;
		case 'h':
		default:
			usage();
		}
	}


	/* Check arguments */
	check_args(input_file, output_file, password, bit_mode, op_mode);

	/* Initialize the library */
	SHA_DIGEST_LENGTH = bit_mode / 8; /* Number of bytes */
	unsigned char* key = NULL;	/* Key from KDF */
	unsigned char* pt = NULL;	/* Plaintext */
	unsigned char* ct = NULL;	/* Ciphertext */
	long f_size = 0;
	int l_pt;
	int l_ct;


	/* Keygen from password */
	printf("\nTask A \n");
	int l_pass = strlen((char*) password); /* Password length */
	key = keygen(password, l_pass, bit_mode);

	/* Operate on the data according to the mode */
	FILE *fp;

	/* Encrypt */
	if(op_mode == 0)
	{
		printf("Task B\n");
		if(!(fp = fopen(input_file,"r"))) errPrint();

		/* Read plaintext from input file */
		if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
		f_size = ftell(fp);	/* Size of file */

		if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */

		pt = malloc(f_size * sizeof(char));
		if(!(fread(pt, 1, f_size, fp))) errPrint();
		fclose(fp);

		l_pt = f_size;	/* Ciphertext size */
		l_ct = ceil((float) l_pt / (float) BLOCK_SIZE) * BLOCK_SIZE;
		ct = encrypt(pt, l_pt, key, bit_mode);

		if(!(fp = fopen(output_file, "w")))	errPrint();

		if(!fwrite(ct, 1, l_ct, fp))	errPrint();

		fclose(fp);
		free(pt);
		free(ct);
	}

	/* Decrypt */
	if (op_mode == 1)
	{
		printf("Task C\n");
		if(!(fp = fopen(input_file,"r"))) errPrint();

		/* Get ciphertext from file */
		if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
		f_size = ftell(fp);	/* Size of file */

		if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */

		ct = malloc((f_size + BLOCK_SIZE) * sizeof(char));
		if(!(fread(ct, 1, f_size, fp))) errPrint();
		fclose(fp);

		l_ct = f_size;	/* Plaintext size */
		l_pt = f_size;	/* Multiple of block size */

		pt = decrypt(ct, l_ct, key, bit_mode);

		/* Write decrypted text to output file */
		if(!(fp = fopen(output_file, "w"))) errPrint(); 
		if(!fwrite(pt, 1, strlen((char*)pt), fp))	errPrint(); 

		fclose(fp);
		free(pt);
		free(ct);
	}

	/* Sign */
	if (op_mode == 2) 
	{
		unsigned char* cmac = NULL;
		printf("Task D\n");
		if(!(fp = fopen(input_file,"r"))) errPrint();

		/* Read plaintext from input file */
		if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
		f_size = ftell(fp);	/* Size of file */

		if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */

		pt = malloc(f_size * sizeof(char));
		if(!(fread(pt, 1, f_size, fp))) errPrint();
		fclose(fp);

		l_pt = f_size;	/* Ciphertext size */
		l_ct = ceil((float) l_pt / (float) BLOCK_SIZE) * BLOCK_SIZE;
		ct = encrypt(pt, l_pt, key, bit_mode);

		cmac = gen_cmac(pt, l_pt, key, bit_mode);

		/* Concatenate ciphertext with the CMAC in an output file. */
		if(!(fp = fopen(output_file, "w"))) errPrint(); 
		if(!fwrite(ct, 1, l_ct + CMAC_SIZE, fp))	errPrint(); 
		if (!fwrite(cmac, l_ct, CMAC_SIZE, fp)) errPrint();

		fclose(fp);
		free(cmac);
		free(pt);
		free(ct);
	}

	/* Verify */
	if (op_mode == 3)
	{
		printf("Task D\n");
		unsigned char* cmac1 = NULL;
		unsigned char* cmac2 = NULL;
		unsigned char* ct2 = NULL;
		int ct_len = 0;

		if(!(fp = fopen(input_file,"r"))) errPrint();

		/* Read ciphertext with CMAC from input file */
		if(fseek(fp, 0, SEEK_END)) errPrint();	/* To get the size of File */
		f_size = ftell(fp);	/* Size of file */

		if(fseek(fp, 0, SEEK_SET)) errPrint();	/* Set pointer to the beginning of File */

		ct = malloc((f_size - CMAC_SIZE) * sizeof(char));
		if(!(fread(ct, 1, f_size - CMAC_SIZE, fp))) errPrint();
		ct_len = ftell(fp);
		fclose(fp);

		/* Generate new CMAC to compare */
		pt = decrypt(ct, ct_len , key, bit_mode); 

		ct2 = encrypt(pt, strlen((char*) pt), key, bit_mode);
	
		cmac2 = gen_cmac(pt,strlen((char*) pt) , key, bit_mode);	
		if(!(fp = fopen(input_file,"r"))) errPrint();
		if(fseek(fp, 0, SEEK_END)) errPrint();	
		f_size = ftell(fp);	/* Size of file */
		if(fseek(fp, f_size - CMAC_SIZE, SEEK_SET)) errPrint();	/* Set pointer to the beginning of CMAC */

		/* Read CMAC from input file */
		cmac1 = malloc(CMAC_SIZE * sizeof(char));
		if(!(fread(cmac1, 1, 16, fp)))	errPrint();

		int x = verify_cmac(cmac1,cmac2);
		if(x) printf("Verification failed!\n");
		else
		{
			l_pt = strlen((char*) pt);
			if(!(fp = fopen(output_file, "w"))) errPrint(); 
			if(!fwrite(pt, 1, l_pt, fp))	errPrint(); 
		}

		fclose(fp);
		free(ct);
		free(cmac2);
	} 

	printf("\n");
	/* Clean up */
	free(input_file);
	free(output_file);
	free(password);


	/* END */
	return 0;
}
