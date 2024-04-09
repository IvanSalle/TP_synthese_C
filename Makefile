CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS =

SRCS = brainfuck_main.c brainfuck_helper.c
OBJS = $(SRCS:.c=.o)
EXECUTABLE = brainfuck

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXECUTABLE)
