CC = gcc
INCPATH = ./inc/
OBJPATH = ./out/
SRCPATH = ./src/
CFLAGS = -std=c99 -I$(INCPATH) -Wall -Werror -Wpedantic -g3 -c
SRCS = $(wildcard $(SRCPATH)*.c)
OBJS = $(SRCS:$(SRCPATH)%.c=$(OBJPATH)%.o)
OUT := $(shell find -type f -name out)


.PHONY: all clean out

all: app
app: $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJPATH)%.o: $(SRCPATH)%.c
	$(CC) $(CFLAGS) -o $@ $<

out:
ifneq ($(OUT),)
	rm -rf ./out
endif
	mkdir -p ./out/

dirs: $(INCPATH) $(OBJPATH) $(SRCPATH)

$(INCPATH):
	mkdir $(INCPATH)
$(OBJPATH):
	mkdir $(OBJPATH)
$(SRCPATH):
	mkdir $(SRCPATH)
clean:
	rm -rf $(OBJPATH)*.o
	rm app