# Specific part of the Makefile
EXEC = stack_ex2

# Begin generic part of the Makefile
CC=gcc
CFLAGS = -std=c99 -Werror -Wextra -Wall -pedantic
LDFLAGS=
SRC= $(wildcard *.c) 
OBJ= $(SRC:.c=.o) #prendre les .c mais on remplace le .c par .o

ifeq ($(DEBUG),yes)
	CFLAGS += -g
	LDFLAGS =
else
	CFLAGS += -O3 -DNDEBUG
	LDFLAGS =
endif

all: $(EXEC)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean mrproper

doc: stack.h
	@doxygen documentation

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)

# End generic part of the makefile
# List of target dependencies
main.o: stack.h