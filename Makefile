BUILD_DIR = build
TEST_BIN = $(BUILD_DIR)/test
ODOM_BIN = $(BUILD_DIR)/src/odom
CMD_VEL_BIN = $(BUILD_DIR)/src/cmd_vel

.PHONY: all test run_main run_all stop clean

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

test_imu: all
	@$(TEST_BIN)/test_imu

test_cmd_vel: all
	@$(TEST_BIN)/test_cmd_vel

# Shortcut to run the main program
run_main: all
	@$(MAIN_BIN)

# Run odom first, then main
run_all: all
	@echo "Running odom..."
	@$(ODOM_BIN) & echo $$! > odom.pid
	@echo "Running main..."
	@$(CMD_VEL_BIN) & echo $$! > cmd_vel.pid
	@echo "Both odom and main are running. Use 'make stop' to terminate."

# Stop both odom and main
stop:
	@echo "Stopping processes..."
	@if [ -f odom.pid ]; then kill -9 $$(cat odom.pid) && rm -f odom.pid; fi
	@if [ -f cmd_vel.pid ]; then kill -9 $$(cat cmd_vel.pid) && rm -f cmd_vel.pid; fi
	@echo "Processes terminated."

# Clean the build directory
clean:
	rm -rf $(BUILD_DIR)
	rm -f odom.pid cmd_vel.pid