bin_objects=$(patsubst %.c, %.o, $(wildcard ../src/*.c))

include mk/*.mk

.PHONY: all static clean

all: static

static: $(bin_objects)
	$(AR) rcs libupocomputernetworks_s.a $^

clean:
	$(RM) -rf $(bin_objects) *.so *.a *.dSYM
