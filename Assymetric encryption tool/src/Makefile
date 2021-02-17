CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic 
OBJFILES = main.o rsa.o utils.o
LIBS = -lm

TARGET = assign_3

ENCRYPT_PUB = ../files/TUC2015030150_encrypted_pub.txt
ENCRYPT_PRIV = ../files/TUC2015030150_encrypted_priv.txt
DECRYPT_PUB = ../files/TUC2015030150_decrypted_pub.txt
DECRYPT_PRIV = ../files/TUC2015030150_decrypted_priv.txt
PUBLIC_KEY = ../files/public.key
PRIVATE_KEY = ../files/private.key

FILES = ${PUBLIC_KEY} ${PRIVATE_KEY} ${ENCRYPT_PUB} ${ENCRYPT_PRIV} ${DECRYPT_PUB} ${DECRYPT_PRIV}

all: $(TARGET) key encrypt_pub encrypt_priv decrypt_pub decrypt_priv 

$(TARGET): $(OBJFILES)
	$(CC) $(CCFLAGS) $(DBUG) -o $(TARGET) $(OBJFILES)  ${LIBS}

key:
	./${TARGET} -g

encrypt_pub:
	./${TARGET} -i ../files/hpy414_encryptme_pub.txt -o ${ENCRYPT_PUB} -k ../files/hpy414_public.key -e

encrypt_priv:
	./${TARGET} -i ../files/hpy414_encryptme_priv.txt -o ${ENCRYPT_PRIV} -k ../files/hpy414_private.key -e

decrypt_pub:
	./${TARGET} -i ../files/hpy414_decryptme_pub.txt -o ${DECRYPT_PUB} -k ../files/hpy414_public.key -d

decrypt_priv:
	./${TARGET} -i ../files/hpy414_decryptme_priv.txt -o ${DECRYPT_PRIV} -k ../files/hpy414_private.key -d

clean:
	rm -f $(TARGET) *.o
	-rm ../files/${FILES}