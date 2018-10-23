CFLAGS= -g -Wall
CC=gcc
TARGET=Tetris.exe

ifeq ($(OS),Windows_NT)
	RM = del /Q
	LIBS=-lpdcurses
else
	LIBS=-lncurses
	RM = rm -f
endif

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	$(RM) -f *.o
	$(RM) -f $(TARGET)

run : $(TARGET)
	./$(TARGET)