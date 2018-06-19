.PHONY: lib install uninstall clean

BUILDDIR=build
INCLUDEDIR=cnet

SRC=$(wildcard *.c)
HEADERS=$(wildcard $(INCLUDEDIR)/*.h)
CFLAGS = -Wall -Iinclude/$(INCLUDEDIR) -g
LIBNAME=libcnet.a

OBJS=$(addprefix $(BUILDDIR)/, $(SRC:.c=.o))

lib: $(OBJS) $(HEADERS)
	ar cr $(LIBNAME) $(OBJS)

install: lib
	sudo install -m 0644 $(LIBNAME) /usr/local/lib
	sudo install -d /usr/local/include/$(INCLUDEDIR)
	sudo install -m 0644 include/$(INCLUDEDIR)/* -t /usr/local/include/$(INCLUDEDIR)

uninstall:
	sudo rm -f /usr/local/lib/$(LIBNAME)
	sudo rm -r -f /usr/local/include/$(INCLUDEDIR)/

clean:
	-rm -f $(BUILDDIR)/*.o
	-rm -f $(LIBNAME)

$(BUILDDIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@