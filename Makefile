WARNINGS :=	-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
		-Wwrite-strings -Wredundant-decls -Wnested-externs -Winline \
					-Wno-long-long -Wuninitialized -Wconversion
CC=gcc
CFLAGS=-c -std=c11
LDFLAGS=
PROJDIRS=src/
BINBASEDIR=bin
BINDIRS=$(patsubst %,$(BINBASEDIR)/%,$(PROJDIRS))
SRCFILES=$(shell find $(PROJDIRS) -type f -name "*.c")
HDRFILES=$(shell find $(PROJDIRS) -type f -name "*.h")
OBJFILES=$(patsubst %.c,$(BINBASEDIR)/%.o,$(SRCFILES))
DEPFILES=$(patsubst %.c,$(BINBASEDIR)/%.d,$(SRCFILES))
ALLFILES=$(SRCFILES) $(HDRFILES)
TARGET=lesyange

.PHONY: all clean dist check todolist prepare

$(BINBASEDIR)/%.o: %.c Makefile
	$(CC) $(CFLAGS) $(WARNINGS) -MMD -MP $< -o $@

all: $(TARGET)

$(TARGET): $(BINDIRS) $(OBJFILES)
	$(CC) $(LDFLAGS) $(OBJFILES) -o $(TARGET)
	
$(BINDIRS): 
	@mkdir -p -v $(BINDIRS)

todolist:
	-@echo > TODO && for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file >> TODO; done; true

clean:
	-rm -r -f -v $(BINBASEDIR)

print:
	@echo "SRCFILES=$(SRCFILES)" && echo "OBJFILES=$(OBJFILES)"
    
-include $(DEPFILES)
