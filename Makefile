PNAME=hedit
IDIR=include
SDIR=src
ODIR=bin

DEPS   := $(shell find $(IDIR) -name "*.h")
SFILES := $(shell find $(SDIR) -name "*.c")
FREETYPE := $(shell pkg-config --cflags freetype2)
OBJ=$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SFILES))

CC=gcc
CFLAGS=-lglfw -lGL -lGLEW -lm -lfreetype -L/usr/local/lib -I$(IDIR) $(FREETYPE) -ggdb

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	$(CC) $^ -o $(PNAME) $(CFLAGS)

clean:
	rm -f $(ODIR)/*.o
