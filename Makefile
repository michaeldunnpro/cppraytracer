# To RUN: make scene SCENE=scenes/scene_name.cpp for custom scene source,
# or just make scene for default scene
# To TEST: make test
# To CLEAN: make clean
# Outputs ppm as image.ppm in the project root directory


CPPFLAGS = -Wall -Wextra -g -std=c++17
BIN_DIR = bin
SRC_DIR = src
SCENE ?= scenes/example_scene.cpp

SCENE_NAME := $(basename $(notdir $(SCENE)))
SCENE_BIN := $(BIN_DIR)/$(SCENE_NAME)

MAIN_SRC = $(SRC_DIR)/test.cpp
LIST = $(wildcard $(SRC_DIR)/*.cpp)
SRC_NO_MAIN = $(filter-out $(MAIN_SRC),$(LIST))
TEST_TARGET = $(BIN_DIR)/test

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

test: $(BIN_DIR) $(LIST)
	g++ $(CPPFLAGS) -o $(TEST_TARGET) $(LIST)

run: test
	./$(TEST_TARGET)

leaks: test
	leaks --atExit -- ./$(TEST_TARGET)

val-leaks: test 
	valgrind --leak-check=yes ./$(TEST_TARGET)


scene: $(BIN_DIR) $(SRC_NO_MAIN) $(SCENE)
	g++ $(CPPFLAGS) -o $(SCENE_BIN) $(SRC_NO_MAIN) $(SCENE)
	./$(SCENE_BIN)


clean:
	rm -rf $(BIN_DIR)
	rm -f image.ppm