WARNINGS :=	-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
		-Wwrite-strings -Wredundant-decls -Wnested-externs -Winline \
					-Wno-long-long -Wuninitialized -Wconversion
CC=gcc
CFLAGS=-c -std=c11
LDFLAGS=
PROJDIRS=src/
SRCFILES=$(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES=$(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES=$(patsubst %.c,%.o,$(SRCFILES))
DEPFILES=$(patsubst %.c,%.d,$(SRCFILES))
ALLFILES=$(SRCFILES) $(HDRFILES)
TARGET=lesyange

.PHONY: all clean dist check todolist

%.o: %.c Makefile
	$(CC) $(CFLAGS) $(WARNINGS) -MMD -MP $< -o $@

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) -o $(TARGET)

todolist:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

clean:
	-rm -f $(wildcard $(OBJFILES) $(DEPFILES))

print:
	@echo "SRCFILES=$(SRCFILES)" && echo "OBJFILES=$(OBJFILES)"

-include $(DEPFILES)
