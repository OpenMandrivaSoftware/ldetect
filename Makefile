NAME = ldetect
LIB_MAJOR = 0.7
LIB_MINOR = 3
VERSION=$(LIB_MAJOR).$(LIB_MINOR)

prefix = /usr
bindir = $(prefix)/bin
libdir = $(prefix)/lib
includedir = $(prefix)/include

binaries = lspcidrake
lib_major = libldetect.so.$(LIB_MAJOR)
libraries = libldetect.so $(lib_major) $(lib_major).$(LIB_MINOR)
CFLAGS = -Wall -W -Wstrict-prototypes -Os -fPIC

RPM ?= $(HOME)/rpm

build: $(binaries) $(libraries)

lspcidrake: lspcidrake.c libldetect.so

$(lib_major).$(LIB_MINOR): common.o pciusb.o pci.o usb.o pciclass.o usbclass.o dmi.o
	$(CC) -shared -Wl,-soname,$(lib_major) -o $@ $^ -lpci -lmodprobe -lz
$(lib_major): $(lib_major).$(LIB_MINOR)
	ln -sf $< $@
libldetect.so: $(lib_major)
	ln -sf $< $@

pciclass.c: /usr/include/linux/pci.h /usr/include/linux/pci_ids.h
	rm -f $@
	perl generate_pciclass.pl $^ > $@
	chmod a-w $@

usbclass.c: /usr/share/usb.ids
	rm -f $@
	perl generate_usbclass.pl $^ > $@
	chmod a-w $@

pciusb.o:	pciusb.c libldetect.h common.h
pci.o:	pci.c libldetect.h common.h
usb.o:	usb.c libldetect.h common.h
dmi.o:	dmi.c libldetect.h common.h

clean:
	rm -f *~ *.o pciclass.c usbclass.c $(binaries) $(libraries)

install: build
	install -d $(bindir) $(libdir) $(includedir)
	install $(binaries) $(bindir)
	cp -a $(libraries) $(libdir)
	install libldetect.h $(includedir)

dis ../$(NAME)-$(VERSION).tar.bz2: clean
	svn export -q . $(NAME)-$(VERSION)
	tar cfj ../$(NAME)-$(VERSION).tar.bz2 $(NAME)-$(VERSION)
	rm -rf $(NAME)-$(VERSION)

log:
	svn2cl --authors ../common/username.xml --accum
