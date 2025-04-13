INCLUDE_DIR := includes
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin

FILES := Bus CrewMember DatabaseManager ReportService Route Trip User
SRCS := $(patsubst %,$(SRC_DIR)/%.cpp,$(FILES))
OBJS := $(patsubst %,$(BUILD_DIR)/%.o,$(FILES))

.PRECIOUS: $(OBJS)

all: $(BIN_DIR)/main $(BIN_DIR)/demo

$(BIN_DIR)/%: $(BUILD_DIR)/%.o $(OBJS) $(BUILD_DIR)/sqlite3.o | $(BIN_DIR)
	g++ $< $(OBJS) $(BUILD_DIR)/sqlite3.o -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	g++ -c $< -o $@

$(BUILD_DIR)/sqlite3.o: $(INCLUDE_DIR)/sqlite/sqlite3.c | $(BUILD_DIR)
	gcc -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

rebuild: clean all
