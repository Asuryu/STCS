CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -pthread
TARGET = prog
TARGET2 = pipetest
SRC = main.c
SRC2 = pipetest.c
HEADER = config.h

all: $(TARGET) $(TARGET2)

$(TARGET): $(SRC) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

$(TARGET2): $(SRC2) $(HEADER)
	$(CC) $(CFLAGS) -o $(TARGET2) $(SRC2)

clean:
	rm -f $(TARGET) $(TARGET2)
