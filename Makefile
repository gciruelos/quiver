CC = clang++
WARNINGS = -Wall -Wextra -Wno-unused-result -Wshadow -Wpointer-arith -Wcast-qual
OPT_FLAGS = -O2 -flto
CFLAGS = $(WARNINGS) -Werror -std=c++14 -pedantic
LFLAGS =
SRC_DIR = src/
OBJ_DIR = obj/
OBJS = utils.o state.o condition.o action.o parser.o program.o quiver.o
EXECUTABLE = quiver

.PHONY: all clean debug

all: $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

debug: OPT_FLAGS=-ggdb
debug: all

%.o: $(SRC_DIR)%.cc
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OPT_FLAGS) $^ -o $@

