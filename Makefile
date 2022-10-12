CC	= gcc
CFLAGS	= -Wall
LSFLAGS	= #insert link flags
TARGETS	= cdep_test

CFILES = $(filter-out $(addsuffix .c,$(TARGETS)), $(wildcard *.c))
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
HEADERS = $(wildcard *.h)

all: $(TARGETS)

objects: $(OBJECTS)

$(TARGETS): $(OBJECTS) $(addsuffix .c,$(TARGETS))
	$(CC) $(CFLAGS) $(LSFLAGS) $@.c -c
	$(CC) $(CFLAGS) $(LSFLAGS) $(OBJECTS) $@.o -o $@

$(OBJECTS): $(CFILES) $(HEADERS)
	$(CC) -c $(CFLAGS) $(LSFLAGS) $(patsubst %.o,%.c,$@)

format:
	clang-format -i $(CFILES) $(HEADERS)


clean:
	rm -rf *.o $(TARGETS)


install:
	mkdir -p ~/.bin
	cp $(TARGETS) ~/.bin/

.PHONY: clean format
