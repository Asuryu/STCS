CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -pthread
TARGET = ../bin/TSL.o
SRC = main.c
HEADER = config.h

all: $(TARGET)

$(TARGET): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)