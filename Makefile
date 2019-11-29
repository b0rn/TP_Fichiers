.SUFFIXES:

CC=gcc
CFLAGS= -Wall -std=c99 -DDEBUG
LDFLAGS= $(HOST32)
INCLUDE=./include
SRC=./src
TARGET=./bin
OBJ=./obj
LIB=./lib
DEPS= $(OBJ)/fichier.o
PROGRAMS=main formattedString
PROGRAMS_STATIC=main.static main.dynamic
PROGRAMS_DYN=formattedString.static formattedString.dynamic
TESTFILES=formatted.txt copyRandomText.txt

.PHONY: clean all test test_ls

all: $(PROGRAMS) $(PROGRAMS_STATIC) $(PROGRAMS_DYN) $(TESTFILES)

%: $(OBJ)/%.o $(DEPS) | $(TARGET)
		$(CC) -o $(TARGET)/$@ $^ $(CFLAGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
		$(CC) -g -fPIC -I$(INCLUDE)/ -o $@ -c $< $(CFLAGS)

%.static: $(OBJ)/%.o $(LIB)/libstdes.a | $(TARGET)
	$(CC) -o $(TARGET)/$@ $< -L$(LIB) -lstdes

%.dynamic: $(OBJ)/%.o $(LIB)/libstdes.so.1 | $(TARGET)
	export LD_LIBRARY_PATH=$(LIB)
	$(CC) -o $(TARGET)/$@ $^

$(LIB)/libstdes.a: $(DEPS) | $(LIB)
	ar q $(LIB)/libstdes.a $^

$(LIB)/libstdes.so.1: $(DEPS) | $(LIB)
	$(CC) -shared $^ -o $(LIB)/libstdes.so.1

randomText.txt:
	base64 /dev/urandom | head -c 10000000 > $@
%.txt:
	touch $@
deleteTexts:
	rm *.txt

withoutLibTest: $(TARGET)/main $(TARGET)/formattedString $(TESTFILES) randomText.txt
	@echo "\nLancement du test lecture/ecriture (sans bibliothèque)"
	$(TARGET)/main randomText.txt copyRandomText.txt
	@echo "\nLancement du test lecture/ecriture formartée (sans bibliothèque)"
	$(TARGET)/formattedString formatted.txt

staticLibTest: $(TARGET)/main.static $(TARGET)/formattedString.static $(TESTFILES) randomText.txt
	@echo "\nLancement du test lecture/ecriture (bibliothèque statique)"
	$(TARGET)/main.static randomText.txt copyRandomText.txt
	@echo "\nLancement du test lecture/ecriture formartée (bibliothèque statique)"
	$(TARGET)/formattedString.static formatted.txt

dynamicLibTest: $(TARGET)/main.dynamic $(TARGET)/formattedString.dynamic $(TESTFILES) randomText.txt
	@echo "\nLancement du test lecture/ecriture (bibliothèque dynamique)"
	$(TARGET)/main.dynamic randomText.txt copyRandomText.txt
	@echo "\nLancement du test lecture/ecriture formartée (bibliothèque dynamique)"
	$(TARGET)/formattedString.dynamic formatted.txt

$(TARGET):
	mkdir $(TARGET)
$(OBJ):
	mkdir $(OBJ)
$(LIB):
	mkdir $(LIB)

clean:
	rm -f $(TARGET)/*
	rm -f $(OBJ)/*
	rm -f $(LIB)/*
	rm -f *.txt
