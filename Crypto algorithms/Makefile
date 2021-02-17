CC		=	gcc
CFLAGS	=	-Wall
LDFLAGS	=	
OFILES	=	simple_crypto.o demo.o
TARGET	=	demo


all: $(TARGET)

$(TARGET): $(OFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OFILES) $(LDFLAGS)

clean:
	rm -f $(OFILES) $(TARGET) *~