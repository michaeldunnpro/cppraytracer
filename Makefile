# To RUN: make scene SCENE=scenes/scene_name.cpp for custom scene source,
# or just make scene for default scene
# To DEBUG: make debug SCENE=scenes/scene_name.cpp for custom scene source,
# or just make debug for default scene
# To TEST: make test
# To CLEAN: make clean
# Outputs ppm as image.ppm in the project root directory

# Detect if Mac or Linux and set the appropriate compiler
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	CXX = g++
else ifeq ($(UNAME_S),Darwin)
	CXX = g++-15
endif

CPPFLAGS = -Wall -Wextra -std=c++17 -fopenmp
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O3 -march=native # Use -march=native to enable all CPU-specific optimizations for the current machine
BIN_DIR = bin
SRC_DIR = src
SCENE ?= scenes/example_scene.cpp
THREADS ?= 4

SCENE_NAME := $(basename $(notdir $(SCENE)))
SCENE_BIN := $(BIN_DIR)/$(SCENE_NAME)

MAIN_SRC = $(SRC_DIR)/test.cpp
LIST = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)
SRC_NO_MAIN = $(filter-out $(MAIN_SRC),$(LIST))
TEST_TARGET = $(BIN_DIR)/test

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

help:
	@echo "Usage:"
	@echo "  make scene SCENE=scenes/scene_name.cpp THREADS=4 - Compile and run a specific scene"
	@echo "  make debug SCENE=scenes/scene_name.cpp THREADS=4 - Compile and run a specific scene with debug flags"
	@echo "  make test - Compile and run tests"
	@echo "  make leaks - Run tests with memory leak detection (leaks on Mac, valgrind on Linux)"
	@echo "  make clean - Remove compiled binaries and output image"

test: $(BIN_DIR) $(LIST)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -o $(TEST_TARGET) $(LIST)

run: test
	OMP_NUM_THREADS=$(THREADS) ./$(TEST_TARGET)

# Automate leaks detection on Mac and valgrind on Linux after running tests, absorb into single command
leaks: test
ifeq ($(UNAME_S),Darwin)
	OMP_NUM_THREADS=$(THREADS) leaks --atExit -- ./$(TEST_TARGET)
else ifeq ($(UNAME_S),Linux)
	OMP_NUM_THREADS=$(THREADS) valgrind --leak-check=yes ./$(TEST_TARGET)
endif

debug: $(BIN_DIR) $(SRC_NO_MAIN) $(SCENE)
	$(CXX) $(CPPFLAGS) $(DEBUG_FLAGS) -o $(SCENE_BIN) $(SRC_NO_MAIN) $(SCENE)
	OMP_NUM_THREADS=$(THREADS) ./$(SCENE_BIN)

scene: $(BIN_DIR) $(SRC_NO_MAIN) $(SCENE)
	$(CXX) $(CPPFLAGS) $(RELEASE_FLAGS) -o $(SCENE_BIN) $(SRC_NO_MAIN) $(SCENE)
	OMP_NUM_THREADS=$(THREADS) ./$(SCENE_BIN)

clean:
	rm -rf $(BIN_DIR)
	rm -f image.ppm