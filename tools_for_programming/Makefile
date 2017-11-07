#Makefile créé par Paul Vialart, à partir de celui de Vincent Boyer
#Octobre 2017

#definition des commandes
CC = gcc
RM = rm

#options du compilateur
CFLAGS = -I/opt/local/include
LDFLAGS = -L/opt/local/lib -lpng

#fichiers et dossiers
PROGNAME = testyprogram
HEADERS= library.h
SRC = $(wildcard *.c) 
OBJ = $(SRC:.c=.o)

all: $(PROGNAME)

$(PROGNAME) : $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o : %.c
	$(CC) $(CFLAGS) $< -c

# Clean all temp files
clean:
	$(RM) -f $(OBJ) *~ $(PROGNAME)



