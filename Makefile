CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TEST_DIR = tests

SRCS = $(SRC_DIR)/GanttChart.cpp \
       $(SRC_DIR)/Metrics.cpp \
       $(SRC_DIR)/FCFS.cpp \
       $(SRC_DIR)/SJF.cpp \
       $(SRC_DIR)/SRTF.cpp \
       $(SRC_DIR)/RoundRobin.cpp \
       $(SRC_DIR)/PriorityScheduler.cpp

OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

TARGET = $(BIN_DIR)/cpu_scheduler
TEST_TARGET = $(BIN_DIR)/run_tests

all: $(TARGET) $(TEST_TARGET)

$(TARGET): $(OBJS) $(BUILD_DIR)/main.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TEST_TARGET): $(OBJS) $(BUILD_DIR)/test_scheduler.o | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/test_scheduler.o: $(TEST_DIR)/test_scheduler.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: $(TEST_TARGET)
	@echo "Running test suite..."
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all test clean
