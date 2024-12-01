CC = gcc
CCFLAGS = -Wall -Wextra -Iinclude/

LIBDIR = lib
LIBSRC = $(wildcard $(LIBDIR)/*.c)
LIBOBJ = $(LIBSRC:.c=.o)

SRCDIR = src
SUBDIR = $(shell find $(SRCDIR) -type d)
SRC = $(foreach dir, $(SUBDIR), $(wildcard $(dir)/*.c))
OBJ = $(SRC:.c=.o)

BINDIR = bin
BINSUBDIR = $(SUBDIR:$(SRCDIR)%=$(BINDIR)%)
BIN = $(SRC:$(SRCDIR)/%.c=$(BINDIR)/%)

DEBUG =
ifeq ($(strip $(DEBUG)),)
	CCFLAGS += -O3
else
	CCFLAGS += -g -DDEBUG
endif

.PHONY: all
all: $(BINSUBDIR) $(BIN)

$(BINSUBDIR):
	mkdir -p $@

$(BINDIR)/%: $(SRCDIR)/%.o $(LIBOBJ)
	$(CC) $(CCFLAGS) -o $@ $^

$(LIBDIR)/%.o: $(LIBDIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(LIBOBJ)
	rm -rf $(BINDIR)
