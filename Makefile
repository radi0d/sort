.PHONY: clean

CFLAGS ?= -g -O2 -Iinclude/ -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CC ?= gcc

SRC = src/main.c src/sort.c src/list.c
OBJ = $(patsubst src/%.c, %.o, $(SRC))
EXE = sort

all: $(EXE)

%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(EXE)
