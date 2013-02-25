#!/usr/bin/make -f
CC=gcc
LINKER=gcc
COPTS:=-Wall -Werr

SRCDIR = src
SOURCES := $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(SOURCES:%.c=%.o)
TARGET = main

$(TARGET): $(OBJECTS)
	$(LINKER) -o $@ $(LFLAGS) $(OBJECTS)

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm $(OBJECTS)

distclean: clean
	@rm $(TARGET)
