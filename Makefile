CC = g++
WARNINGS = -Wall -Wextra -Wno-unused-result -Wshadow -Wpointer-arith -Wcast-qual
OPT_FLAGS = -O3 -flto
CFLAGS = $(WARNINGS) -Werror -std=c++14 -pedantic
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

debug: OPT_FLAGS=-ggdb
debug: all

$(OBJ_DIR):
	${MKDIR_P} $@

$(OBJ_DIR)%.o: $(SRC_DIR)%.cc
	$(CC) $(CFLAGS) $(OPT_FLAGS) -c -o $@ $<

$(EXECUTABLE): $(OBJS)
	$(CC) $(LFLAGS) $(OPT_FLAGS) $^ -o $@

