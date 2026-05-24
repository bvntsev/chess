#!/usr/bin/make -f
# === config ===

CC = gcc
srcdir = ./src
includedir = ./include
builddir = build
DEFS =

CDEBUG = -g -Wall -Wextra -O3
CFLAGS = $(CDEBUG) -I$(includedir) -I$(srcdir) $(DEFS)
LDFLAGS = -g

OBJS = \
	$(builddir)/CHESScli.o \
	$(builddir)/CHESSgui.o \
	$(builddir)/CHESSlogging.o \
	$(builddir)/CHESSpieces.o \
	$(builddir)/CHESSsession.o \
	$(builddir)/CHESStest.o \
	$(builddir)/CHESSutil.o \
	$(builddir)/main.o

OBJSDIR = $(addprefix $(builddir)/,$(OBJS))

all: $(builddir) $(OBJS)
	$(CC) $(LDFLAGS) -o $(builddir)/chess $(OBJS)

$(builddir):
	mkdir -p $(builddir)

$(builddir)/%.o: $(srcdir)/%.c | $(builddir)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : 	clean run debug debug_all valgrind valgrind_all \
		rm_lof_files konsole konsole_all clear debug_all_konsole

clean:
	rm -f $(builddir)/*

run:
	./build/chess

debug_all: clean all debug

debug:
	gdb ./build/chess

debug_all_konsole: clean all
	konsole -e gdb ./build/chess

valgrind_all_konsole: clean all
	konsole -e valgrind ./build/chess

valgrind_all: clean all valgrind

valgrind:
	valgrind ./build/chess

rm_log_files:
	rm -f ~/.local/share/bvchess/*.log

konsole:
	konsole -e ./build/chess

konsole_all: all
	konsole -e ./build/chess

clear:
	clear
