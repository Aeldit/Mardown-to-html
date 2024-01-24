CC=gcc
CFLAGS=-std=c99 -pedantic -Werror -Wall -Wextra -Wvla -g
SANITIZE=-fsanitize=address

# Contains the C files to use for compilation
CFILES=$(wildcard src/*.c)
OBJ=$(CFILES:.c=.o)

all: LDLIBS+=$(SANITIZE)
all: CFLAGS+=$(SANITIZE)
all: md-to-html

# Generates the binary file
# $@ is the name of the rule (here, md-to-html)
# $^ is the dependencies of the rule
md-to-html: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f src/*.o
	rm md-to-html
