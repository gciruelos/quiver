CC = g++
WARNINGS = -Wall -Wextra -Wno-unused-result -Wcast-qual
OPT_FLAGS = -O2 -flto
CFLAGS = $(WARNINGS) -Werror -std=c++11 -pedantic
LFLAGS =
SRC_DIR = src/
OBJ_DIR = obj/
EMSCRIPTEN_SRC = $(SRC_DIR)program_js.cc
SRCS = $(filter-out $(EMSCRIPTEN_SRC), $(wildcard $(SRC_DIR)*.cc))
OBJS = $(addprefix $(OBJ_DIR),$(notdir $(SRCS:.cc=.o)))
EXECUTABLE = quiver
MKDIR_P = mkdir -p

.PHONY: all clean debug test travis

all: $(OBJ_DIR) $(EXECUTABLE)

clean:
	rm -f $(OBJS) $(EXECUTABLE)

debug: OPT_FLAGS=-ggdb -O0
debug: all

perf: OPT_FLAGS+= -ggdb -fno-rtti -fno-omit-frame-pointer
perf: all

travis: CC=$(CXX)
travis: | all test

$(OBJ_DIR):
	${MKDIR_P} $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OPT_FLAGS) $^ -o $@

test:
	cd test && make all

valgrind: clean debug
	valgrind -v --num-callers=20 --leak-check=yes --leak-resolution=high --show-reachable=yes ./$(EXECUTABLE) examples/primes.quiv

javascript: $(SRCS) $(EMSCRIPTEN_SRC)
	em++ -s EXPORTED_FUNCTIONS="['_runProgramFromString']" -O1 $(CFLAGS) $(SRCS) $(EMSCRIPTEN_SRC) -o doc/quiver.js
