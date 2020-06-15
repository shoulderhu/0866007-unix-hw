SUBDIRS = countme/ syscall/ no_more_traps

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(MAKE) -C clean

.PHONY: all $(SUBDIRS) clean