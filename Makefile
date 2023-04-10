CC = gcc
HEADER_DIR = include
CFLAGS = -Wall -O2 -I$(HEADER_DIR)
SRCDIR = src
BINDIR = bin

LIBS = -lallegro -lallegro_font -lallegro_primitives -lallegro_image

all: $(BINDIR)/laby run

$(BINDIR)/laby: $(SRCDIR)/laby.c
	@$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

run: 
	@./$(BINDIR)/laby

clean:
	del "$(BINDIR)\laby.exe"