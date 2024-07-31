CC = gcc

CFLAGS = -Wall -Wextra -std=c99

TARGET = prog

SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
