CFLAGS+=-Wall -Wextra -std=c11 -pedantic -g
SOURCE=./
BIN=./bin
PROG=client server
LIST=$(addprefix $(BIN)/, $(PROG))

all: $(BIN) $(LIST)

$(BIN)/%: $(SOURCE)%.c
	$(CC) $(INC) $< $(CFLAGS) -o $@ $(LIBS)

$(BIN):
	mkdir $(BIN)

clean:
	$(RM) -r $(BIN)