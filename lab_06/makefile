CC=gcc
INCPATH=./inc/
OBJPATH=./out/
SRCPATH=./src/
DOCSPATH=./docs/
CFLAGS=-std=c99 -I$(INCPATH) -D NDEBUG -O3 -c
LFLAGS=--coverage -lm
DIR_CR=mkdir -p
INC:=$(wildcard $(INCPATH)*.h)
SRCS := $(wildcard $(SRCPATH)*.c)
OBJS:=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o, $(SRCS))

dep = $(OBJS:.o=.d) 
OUT := $(shell find -type f -name out)

test: CFLAGS=-std=c99 -I$(INCPATH) -Wall -Wpedantic -Wextra -ggdb -O0 -c
test: app

.PHONY: clean func dirs out

app: $(OBJS) | out
	$(CC) -o $@ $(OBJS) $(LFLAGS)

$(OBJPATH)%.o: $(SRCPATH)%.c | out
	$(CC) $(CFLAGS) -o $@ $<

$(OBJPATH)%.d : $(SRCPATH)%.c | out
	$(CC) -I$(INCPATH) -MM -MT $(patsubst %.d,%.o,$@) $< > $@

ifneq ($(MAKECMDGOALS), clean)
-include $(dep)
endif

out:
ifneq ($(OUT),)
	rm -rf ./out
endif
	$(DIR_CR) $(OBJPATH)

dirs: $(OBJPATH) $(INCPATH) $(SRCPATH) $(DOCSPATH)
$(OBJPATH):
	$(DIR_CR) $@
$(INCPATH):
	$(DIR_CR) $@
$(SRCPATH):
	$(DIR_CR) $@
$(DOCSPATH):
	$(DIR_CR) $@
clean: 
	rm -fv app
	rm -fv $(OBJPATH)*.o
	rm -fv $(OBJPATH)*.d
	rm -fv $(OBJPATH)*.gcno
	rm -fv $(OBJPATH)*.gcda
	rm -fv $(OBJPATH)*.lastbuildstate
	rm ./data/*.gv
