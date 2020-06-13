LDFLAGS+=-L../bin
LDLIBS=-lupocomputernetworks_s -lm
test_targets=

export LDFLAGS
export LDLIBS
export test_targets

include mk/*.mk

.PHONY: all clean

all: $(test_targets)

clean:
	$(RM) -rf $(test_targets) *.o *.dSYM
