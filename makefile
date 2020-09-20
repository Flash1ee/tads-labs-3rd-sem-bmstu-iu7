OBJPATH := ./out/
INCPATH := ./inc/
SRCPATH := ./src/
CC := gcc
CFLAGS := -std=c99 -I$(INCPATH) -Wall -Wextra -Wpedantic
GCOV := --coverage
DEBUG := $(CFLAGS) -g3
ALL := $(OBJPATH)main.o $(OBJPATH)io.o $(OBJPATH)parse.o

.PHONY: clean

app.exe: $(ALL)
	$(CC) -o app.exe $(ALL)
$(OBJPATH)main.o: $(SRCPATH)main.c
	$(CC) $(CFLAGS) -c $(SRCPATH)main.c -o $(OBJPATH)main.o
$(OBJPATH)parse.o: $(SRCPATH)parse.c
	$(CC) $(CFLAGS) -c $(SRCPATH)parse.c -o $(OBJPATH)parse.o
$(OBJPATH)io.o: $(SRCPATH)io.c
	$(CC) $(CFLAGS) -c $(SRCPATH)io.c -o $(OBJPATH)io.o


clean:
	rm -rf $(OBJPATH)*.o $(OBJPATH)*.gcno 
	rm app.exe