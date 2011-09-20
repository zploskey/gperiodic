# Makefile for locale directory

PACKAGE ?= gperiodic
MSGFMT = msgfmt
FILES_PO := $(wildcard *.po)
FILES_MO := $(FILES_PO:.po=.mo)
datadir ?= /usr/share
enable_nls ?= 1

LOCALEDIR ?= $(datadir)/locale
MSGFMT_OPT ?= -f -v -o


.SUFFIXES: .mo .po
.po.mo:
	$(MSGFMT) $(MSGFMT_OPT) $@ $<


ifeq ($(enable_nls),1)
all: mo-files
mo-files: $(FILES_MO)
else
all:
mo-files:
endif

install: 
	$(MAKE) all
	for f in $(FILES_MO) ; do \
		mkdir -p $(DESTDIR)$(LOCALEDIR)/`basename $$f .mo`/LC_MESSAGES ; \
		install -m 644 $$f $(DESTDIR)$(LOCALEDIR)/`basename $$f .mo`/LC_MESSAGES/$(PACKAGE).mo ; \
	done

uninstall:
	for f in $(FILES_MO) ; do \
		rm -f $(DESTDIR)$(LOCALEDIR)/`basename $$f .mo`/LC_MESSAGES/$(PACKAGE).mo ; \
	done

clean:
	rm -f $(FILES_MO)

.PHONY: all mo-files install uninstall clean
