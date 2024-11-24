BUILD_DIR = build
TEST_BIN = $(BUILD_DIR)/test

.PHONY: all test run_main clean

# Default target builds everything
all:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..
	@cd $(BUILD_DIR) && make

# Shortcut to run tests
test: all
	@$(TEST_BIN)/test_client

test_laser: all
	@$(TEST_BIN)/test_laser

test_laser: all
	@$(TEST_BIN)/test_imu

# Shortcut to run the main program
run_main: all
	@$(BUILD_DIR)/src/main

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)