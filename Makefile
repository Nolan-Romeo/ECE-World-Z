CC = gcc
HEADER_DIR = include
CFLAGS = -Wall -O2 -I$(HEADER_DIR)
SRCDIR = src
BINDIR = bin

LIBS = -lallegro -lallegro_font -lallegro_primitives -lallegro_image

all: $(BINDIR)/baril run

$(BINDIR)/baril: $(SRCDIR)/baril.c
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

run: 
	@./$(BINDIR)/baril

clean:
	del "$(BINDIR)\baril.exe"