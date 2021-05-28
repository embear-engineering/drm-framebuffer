CC      ?= gcc
CFLAGS  ?= -pedantic -Wall -I /usr/include/drm
LDFLAGS ?= -ldrm

OBJ = main.o
PROGNAME = drm-framebuffer

exec_prefix ?= /usr
bindir ?= $(exec_prefix)/bin

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(OBJ) $(LDFLAGS)

install: all
	install -d $(DESTDIR)$(bindir)
	install -m 0755 $(PROGNAME) $(DESTDIR)$(bindir)

clean: $(OBJ)
	@echo "Clean object files"
	@rm -f $<
	@rm -f $(PROGNAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $<
