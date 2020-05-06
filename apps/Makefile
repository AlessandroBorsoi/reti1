LDFLAGS+=-L../bin
LDLIBS=-lupocomputernetworks_s -lm
apps_targets=

export LDFLAGS
export LDLIBS
export apps_targets

include mk/*.mk

.PHONY: all clean

all: $(apps_targets)

clean:
	$(RM) -rf $(apps_targets) *.o *.dSYM
