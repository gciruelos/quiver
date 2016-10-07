CC = g++
WARNINGS = -Wall -Wextra -Wno-unused-result -Wshadow -Wpointer-arith -Wcast-qual
OPT_FLAGS = -O2 -flto
CFLAGS = $(WARNINGS) -Werror -std=c++11 -pedantic
LFLAGS =
SRC_DIR = src/
OBJ_DIR = obj/
SRCS = $(wildcard $(SRC_DIR)*.cc)
OBJS = $(addprefix $(OBJ_DIR),$(notdir $(SRCS:.cc=.o)))
EXECUTABLE = quiver
MKDIR_P = mkdir -p

.PHONY: all clean debug

all: $(OBJ_DIR) $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

debug: OPT_FLAGS=-ggdb -O2 -DGLIBCXX_FORCE_NEW=1
debug: all

travis: CC=$(CXX)
travis: all

$(OBJ_DIR):
	${MKDIR_P} $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OPT_FLAGS) $^ -o $@


valgrind: clean debug
	valgrind -v --num-callers=20 --leak-check=yes --leak-resolution=high --show-reachable=yes ./$(EXECUTABLE) examples/primes.quiv
