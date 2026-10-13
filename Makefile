CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2 -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Sources
CORE_SRCS = $(SRC_DIR)/core/BrowserHistory.cpp
PRES_SRCS = $(SRC_DIR)/presentation/BrowserVisualizer.cpp $(SRC_DIR)/presentation/BrowserCLI.cpp
MAIN_SRC = $(SRC_DIR)/main.cpp
TEST_SRC = $(TEST_DIR)/test_runner.cpp

# Object files
CORE_OBJS = $(OBJ_DIR)/BrowserHistory.o
PRES_OBJS = $(OBJ_DIR)/BrowserVisualizer.o $(OBJ_DIR)/BrowserCLI.o
MAIN_OBJ = $(OBJ_DIR)/main.o
TEST_OBJ = $(OBJ_DIR)/test_runner.o

# Target Executables
TARGET = $(BIN_DIR)/browser_simulator
TEST_TARGET = $(BIN_DIR)/run_tests

.PHONY: all clean run test directories

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link Application
$(TARGET): directories $(CORE_OBJS) $(PRES_OBJS) $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(CORE_OBJS) $(PRES_OBJS) $(MAIN_OBJ) -o $@

# Link Test Runner
$(TEST_TARGET): directories $(CORE_OBJS) $(PRES_OBJS) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(CORE_OBJS) $(PRES_OBJS) $(TEST_OBJ) -o $@

# Compile Core Sources
$(OBJ_DIR)/BrowserHistory.o: $(SRC_DIR)/core/BrowserHistory.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Presentation Sources
$(OBJ_DIR)/BrowserVisualizer.o: $(SRC_DIR)/presentation/BrowserVisualizer.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/BrowserCLI.o: $(SRC_DIR)/presentation/BrowserCLI.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Main Source
$(OBJ_DIR)/main.o: $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Test Source
$(OBJ_DIR)/test_runner.o: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run Application
run: all
	@./$(TARGET)

# Build and Run Tests
test: directories $(TEST_TARGET)
	@./$(TEST_TARGET)

# Clean Build Artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
