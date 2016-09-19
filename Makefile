CC = clang++
WARNINGS = -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS = $(WARNINGS) -Werror -std=c++14 -pedantic -O2 -ggdb
TARGETS = utils.o state.o condition.o action.o parser.o program.o quiver.o
LIBRARIES = -lm

all: quiver

%.o: src/%.cc
	$(CC) $(CFLAGS) -c -o $@ $<

quiver: $(TARGETS)
	$(CC) $(LIBRARIES) $(TARGETS) -o $@


clean:
	rm -f $(TARGETS) quiver

