CC = gcc
CFLAGS = -lpthread
DEPS =
OBJ = main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


multicat: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

#Agradecimentos a Gustavo A. Polli e Mateus Pim pela ajuda no makefile <3