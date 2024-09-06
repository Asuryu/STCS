CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -pthread
TARGET = ../bin/TCF.o
SRC = TCF.c
SRC2 = PID.c
ARGS = 3 # frequency

all: $(TARGET)

$(TARGET): $(SRC) $(SRC2)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(SRC2)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) $(ARGS)