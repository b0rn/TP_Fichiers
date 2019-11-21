.SUFFIXES:

CC=gcc
CFLAGS= -Wall -std=c99 -DDEBUG
LDFLAGS= $(HOST32)
INCLUDE=./include
SRC=./src
TARGET=./bin
OBJ=./obj
DEPS= $(OBJ)/fichier.o
PROGRAMS=main

.PHONY: clean all test test_ls

all: $(PROGRAMS)

%: $(OBJ)/%.o $(DEPS) | $(TARGET)
		$(CC) -o $(TARGET)/$@ $^ $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
		$(CC) -g -I$(INCLUDE)/ -o $@ -c $< $(CFLAGS)

$(TARGET):
	mkdir $(TARGET)
$(OBJ):
	mkdir $(OBJ)

clean:
	rm $(TARGET)/*
	rm $(OBJ)/*
