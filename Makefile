CC = g++
WARNINGS = -Wall -Wextra -Wno-unused-result -Wcast-qual
OPT_FLAGS = -O2 -flto
CFLAGS = $(WARNINGS) -Werror -std=c++11 -pedantic
LFLAGS =
SRC_DIR = src/
OBJ_DIR = obj/
SRCS = $(wildcard $(SRC_DIR)*.cc)
OBJS = $(addprefix $(OBJ_DIR),$(notdir $(SRCS:.cc=.o)))
EXECUTABLE = quiver
MKDIR_P = mkdir -p

.PHONY: all clean debug travis

all: $(OBJ_DIR) $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

debug: OPT_FLAGS=-ggdb -O2 -DGLIBCXX_FORCE_NEW=1
debug: all

perf: OPT_FLAGS+= -fno-rtti -fno-omit-frame-pointer
perf: all

travis: CC=$(CXX)
travis: all
	./$(EXECUTABLE) examples/1to10.quiv
	./$(EXECUTABLE) examples/factorial.quiv 11
	./$(EXECUTABLE) examples/primes.quiv
	./$(EXECUTABLE) examples/hello-world.quiv
	./$(EXECUTABLE) examples/99-bottles.quiv
	./$(EXECUTABLE) examples/empty.quiv

$(OBJ_DIR):
	${MKDIR_P} $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OPT_FLAGS) $^ -o $@


valgrind: clean debug
	valgrind -v --num-callers=20 --leak-check=yes --leak-resolution=high --show-reachable=yes ./$(EXECUTABLE) examples/primes.quiv
