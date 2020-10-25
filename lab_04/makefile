CC=gcc
INCPATH=./inc/
OBJPATH=./out/
SRCPATH=./src/
DOCSPATH=./docs/
CFLAGS=-std=c99 --coverage -ggdb -I$(INCPATH) -Wall -Wpedantic -Wextra -g -O2 -c
LFLAGS=--coverage
DIR_CR=mkdir -p
INC:=$(wildcard $(INCPATH)*.h)
SRCS := $(wildcard $(SRCPATH)*.c)
OBJS:=$(patsubst $(SRCPATH)%.c,$(OBJPATH)%.o, $(SRCS))

dep = $(OBJS:.o=.d) 
OUT := $(shell find -type f -name out)


.PHONY: clean func dirs out
.NOTPARALLEL: release debug

release: CFLAGS=-std=c99 -I$(INCPATH) -o2 -c
release: LFLAGS=-o2


debug: $(OBJPATH)debug.lastbuildstate app
release: $(OBJPATH)release.lastbuildstate app



$(OBJPATH)debug.lastbuildstate: | out
	rm -fv app
	rm -fv $(OBJPATH)*.o
	rm -fv $(OBJPATH)*.gcno
	rm -fv $(OBJPATH)*.gcda
	rm -fv $(OBJPATH)release.lastbuildstate
	touch $(OBJPATH)debug.lastbuildstate

$(OBJPATH)release.lastbuildstate: | out
	rm -fv app
	rm -fv $(OBJPATH)*.o
	rm -fv $(OBJPATH)*.gcno
	rm -fv $(OBJPATH)*.gcda
	rm -fv $(OBJPATH)debug.lastbuildstate
	touch $(OBJPATH)release.lastbuildstate


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
