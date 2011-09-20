CC := gcc
CFLAGS := `pkg-config --cflags gtk+-2.0` -I. -DG_DISABLE_DEPRECATED -DGDK_DISABLE_DEPRECATED -DGDK_PIXBUF_DISABLE_DEPRECATED -DGTK_DISABLE_DEPRECATED
LIBS :=`pkg-config --libs gtk+-2.0`
bindir ?= /usr/bin
datadir ?= /usr/share
enable_nls ?= 1

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

all: 
	make gperiodic
	make -C po/ all enable_nls=$(enable_nls)

gperiodic: gperiodic.o
	$(CC) $(CFLAGS) -o gperiodic gperiodic.o $(LIBS)
ifeq ($(strip),1)
	strip gperiodic
endif

gpdata.o: gpdata.c gperiodic.h

gperiodic.o: gperiodic.c gperiodic.h table_data.h

install:
	mkdir -p $(DESTDIR)$(bindir)
	install -m 755 gperiodic $(DESTDIR)$(bindir)
	mkdir -p $(DESTDIR)$(datadir)/applications
	install -m 644 gperiodic.desktop $(DESTDIR)$(datadir)/applications
	mkdir -p $(DESTDIR)$(datadir)/pixmaps
	install -m 644 gperiodic.png $(DESTDIR)$(datadir)/pixmaps
	install -m 644 gperiodic-crystal.png $(DESTDIR)$(datadir)/pixmaps
	make -C po/ install enable_nls=$(enable_nls) datadir=$(datadir) DESTDIR=$(DESTDIR)

uninstall:
	rm -f $(bindir)/gperiodic \
	      $(datadir)/applications/gperiodic.desktop
	      $(datadir)/pixmaps/gperiodic.png \
	      $(datadir)/pixmaps/gperiodic-crystal.png
	make -C po/ uninstall enable_nls=$(enable_nls) datadir=$(datadir) DESTDIR=$(DESTDIR)

clean:
	rm -f *.o gperiodic
	make -C po/ clean

.PHONY: install uninstall clean
