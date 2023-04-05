CC = gcc
HEADER_DIR = include
CFLAGS = -Wall -O2 -I$(HEADER_DIR)
SRCDIR = src
BINDIR = bin

LIBS = -lallegro -lallegro_font -lallegro_primitives -lallegro_image

all: $(BINDIR)/main run

$(BINDIR)/main: $(SRCDIR)/main.c
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

run: 
	@./$(BINDIR)/main

clean:
	del "$(BINDIR)\main.exe"