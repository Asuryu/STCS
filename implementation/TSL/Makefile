CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I/path/to/config.h/directory
TARGET = prog
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
