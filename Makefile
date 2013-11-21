
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

cliente:
	make -C cliente all

clean:
	-rm -f *.o
	-rm -f $(TARGET)
	make -C cliente clean
