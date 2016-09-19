CC = clang++
WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS = $(WARNINGS) -Werror -std=c++14 -pedantic -O2 -ggdb
TARGETS = program.o quiver.o parser.o condition.o action.o
LIBRARIES = -lm

all: quiver

%.o: src/%.cc
	$(CC) $(CFLAGS) -c -o $@ $<

quiver: $(TARGETS)
	$(CC) $(LIBRARIES) $(TARGETS) -o $@


clean:
	rm -f $(TARGETS) quiver

