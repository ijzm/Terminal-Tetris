CFLAGS= -g -Wall
CC=gcc

ifeq ($(OS),Windows_NT)
	TARGET=Tetris.exe
	RM = del /Q
	LIBS=-lpdcurses
else
	TARGET=$(shell basename $(CURDIR))
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