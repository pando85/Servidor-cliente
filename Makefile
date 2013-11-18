#siempre actualiza cliente

TARGET = servidor
LIBS = -lpthread
CC = gcc
CFLAGS =

.PHONY: default all clean

default: $(TARGET) cliente
all: default

#Debug: bin/Debug/Arbol\ binario
#Release: all
OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

#bin/Debug/Arbol\ binario: $(OBJECTS)
#	$(CC) $(OBJECTS) -Wall -g $(LIBS) -o bin/Debug/Arbol\ binario

cliente/cliente.o:cliente/cliente.c $(HEADERS)
	$(CC) $(CFLAGS) -c cliente/cliente.c -o cliente/cliente.o

#cliente/get.o:cliente/get.c $(HEADERS)
#	$(CC) $(CFLAGS) -c cliente/get.c -o cliente/get.o

cliente: cliente/cliente.o
	$(CC) cliente/cliente.o  -Wall $(LIBS) -o client

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	-rm -f cliente/*.o
	-rm -f client
