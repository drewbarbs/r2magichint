CFLAGS += -g -O3
CFLAGS += $(shell pkg-config --cflags r_core r_util r_anal)
LDFLAGS += $(shell pkg-config --libs r_core r_util r_anal)
LIBEXT = $(shell r2 -H LIBEXT)
PLUGDIR = $(shell r2 -H R2_USER_PLUGINS)

all: core_ahm.c
	$(CC) $(CFLAGS) $(LDFLAGS) -shared -fPIC core_ahm.c -o core_ahm.$(LIBEXT)

install:
	mkdir -p $(PLUGDIR)
	rm -rf $(PLUGDIR)/core_ahm.$(LIBEXT)*
	cp -rf core_ahm.$(LIBEXT)* $(PLUGDIR)

uninstall:
	rm -rf $(PLUGDIR)/core_ahm.$(LIBEXT)*

clean:
	rm -rf core_ahm.$(LIBEXT)*
