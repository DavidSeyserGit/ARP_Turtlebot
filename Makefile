# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2

# Directories
SRC_DIR = src
OBJ_DIR = build
BIN_DIR = bin

# Target executable names
CLIENT_TARGET = $(BIN_DIR)/client
SERVER_TARGET = $(BIN_DIR)/server

# Source files
CLIENT_SRC = $(SRC_DIR)/client.cpp $(SRC_DIR)/TCP.cpp
SERVER_SRC = $(SRC_DIR)/server.cpp $(SRC_DIR)/TCP.cpp

# Object files
CLIENT_OBJ = $(CLIENT_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
SERVER_OBJ = $(SERVER_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
all: $(CLIENT_TARGET) $(SERVER_TARGET)

# Build the client target
$(CLIENT_TARGET): $(CLIENT_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Build the server target
$(SERVER_TARGET): $(SERVER_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the directories if they do not exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony targets
.PHONY: all clean