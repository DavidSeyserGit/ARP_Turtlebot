# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# Directories
SRC_DIR = src
TEST_DIR = test
BUILD_DIR = build
BIN_DIR = bin

# Source files
SRC_FILES = $(SRC_DIR)/main.cpp $(SRC_DIR)/client.cpp
TEST_FILES = $(TEST_DIR)/test_client.cpp $(TEST_DIR)/test_server.cpp

# Output files
MAIN_BIN = $(BIN_DIR)/main
TEST_BIN = $(BIN_DIR)/tests

# Default target
all: $(MAIN_BIN)

# Build main program
$(MAIN_BIN): $(SRC_FILES)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build tests
test: $(TEST_FILES) $(SRC_DIR)/client.cpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $(TEST_BIN) $^
	./$(TEST_BIN)

# Clean build files
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)