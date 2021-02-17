CC = gcc
DBUG = -g
CCFLAGS = -O2 -Wall -pedantic
LIBSSL = -lssl -lcrypto -lm

TARGETS = assign_1

ENCRYPT = ../files/decryptme_256.txt 
DECRYPT = ../files/hpy414_encryptme_128.txt
SIGN = ../files/verifyme_128.txt
VERIFY128 = ../files/verified_128.txt
VERIFY256 = ../files/verified_256.txt

FILES = ${ENCRYPT} ${DECRYPT} ${SIGN}

all: $(TARGETS) 

assign_1: assign_1.c
	$(CC) $(CCFLAGS) $(DBUG) -o $@ $< $(LIBSSL)

encrypt:
	./$(TARGETS) -i ../files/encryptme_256.txt -o ${ENCRYPT} -p TUC2015030150 -e -b 256

decrypt:
	./$(TARGETS) -i ../files/hpy414_decryptme_128.txt -o ${DECRYPT} -p hpy414 -d -b 128

sign:
	./${TARGETS} -i ../files/signme_128.txt -o ${SIGN} -p TUC2015030150 -s -b 128

verify128:
	./$(TARGETS) -i ../files/hpy414_verifyme_128.txt -o ${VERIFY128} -p hpy414 -v -b 128

verify256:
	./$(TARGETS) -i ../files/hpy414_verifyme_256.txt -o ${VERIFY256} -p hpy414 -v -b 256

clean:
	-rm ../files/${FILES}
	rm -f $(TARGETS)
