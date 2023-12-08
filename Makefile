CC = clang
DEFAULT = -lreadline -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600 -fuse-ld=lld 
ARGS = -Wall -Werror -Wextra -pedantic -std=c11
ARGS2 = -O3 -flto=thin -pipe
TARGETS = $(wildcard *.c) 

build:
	$(CC) $(DEFAULT) $(ARGS) $(ARGS) $(TARGETS) -o blsh
