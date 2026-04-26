# === config ===
include config.mk

# get source files
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

all: clean $(BIN) run

all_clear: clean $(BIN) clear run

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN)

run:
	./$(BIN)

debug_all: clean $(BIN) debug

debug:
	gdb $(BIN)

valgrind_all: clean $(BIN) valgrind

valgrind:
	valgrind $(BIN)

rm_log_files:
	rm -f ~/.local/share/bvchess/*.log

clear:
	clear

.PHONY: all clean run debug debug_all valgrind valgrind_all clear
