CC = clang
CFLAGS = -std=c99 -Wall -Wpedantic -Wextra -ggdb
VFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes

.PHONY = run clean

run: stash
	./stash

valgrind: stash
	valgrind $(VFLAGS) ./stash

stash: stash.c
	$(CC) $(CFLAGS) stash.c -o stash

clean:
	rm -rf stash
