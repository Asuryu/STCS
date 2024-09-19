CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -pthread
TARGET = tcf.o
SRC = tcf_main.c
SRC2 = pid.c
ARGS = 3

all: $(TARGET)

$(TARGET): $(SRC) $(SRC2)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(SRC2)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) $(ARGS)


