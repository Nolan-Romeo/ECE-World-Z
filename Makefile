CC = gcc
HEADER_DIR = include
CFLAGS = -Wall -O2 -I$(HEADER_DIR)
SRCDIR = src
BINDIR = bin

LIBS = -lallegro -lallegro_font -lallegro_primitives -lallegro_image

all: $(BINDIR)/jeu run

$(BINDIR)/jeu: $(SRCDIR)/jeu.c
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

run: 
	@./$(BINDIR)/jeu

clean:
	del "$(BINDIR)\jeu.exe"