CC = gcc
CFLAGS = -Wall -Wextra

TARGET = flashos

SRC = src/main.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
