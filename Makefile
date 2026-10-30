#############################################################################
# Makefile for Image Smoothing with Bezier Curves
# UE MAP401 - DLST - UGA - 2024/2025
#############################################################################

# Compiler and flags
CC = clang
CFLAGS = -g -Wall -Iinclude
LDFLAGS = -lm

# Directories
SRC_DIR = src
INCLUDE_DIR = include
TESTS_DIR = tests
BIN_DIR = bin
BUILD_DIR = obj

# Ensure directories exist
$(shell mkdir -p $(BIN_DIR) $(BUILD_DIR))

# Source files
CORE_SRCS = \
	$(SRC_DIR)/image.c \
	$(SRC_DIR)/geom2d.c \
	$(SRC_DIR)/calcul_contour.c \
	$(SRC_DIR)/bezier.c \
	$(SRC_DIR)/main.c

# Test source files
TEST_SRCS = \
	$(TESTS_DIR)/test_image.c \
	$(TESTS_DIR)/test-geom2d.c \
	$(TESTS_DIR)/test_contour.c \
	$(TESTS_DIR)/test_simplific_douglas.c \
	$(TESTS_DIR)/test_bezier.c \
	$(TESTS_DIR)/test_douglas_bezier2.c \
	$(TESTS_DIR)/test_bezier3.c

# Object files
CORE_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(CORE_SRCS))
TEST_OBJS = $(patsubst $(TESTS_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# Executables
MAIN_EXECUTABLE = $(BIN_DIR)/image_smoother
TEST_EXECUTABLES = \
	$(BIN_DIR)/test_image \
	$(BIN_DIR)/test-geom2d \
	$(BIN_DIR)/test_contour \
	$(BIN_DIR)/test_simplific_douglas \
	$(BIN_DIR)/test_bezier \
	$(BIN_DIR)/test_douglas_bezier2 \
	$(BIN_DIR)/test_bezier3

#############################################################################
# Rules
#############################################################################

.PHONY: all clean main tests

# Default target - build main program only
all: main

# Build main program
main: $(MAIN_EXECUTABLE)

# Build all test programs
tests: $(TEST_EXECUTABLES)

# Build everything (main + tests)
everything: main tests

# Generic rule for compiling source files to build directory
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for compiling test files to build directory
$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Main program
$(MAIN_EXECUTABLE): $(BUILD_DIR)/main.o $(BUILD_DIR)/image.o $(BUILD_DIR)/geom2d.o $(BUILD_DIR)/bezier.o $(BUILD_DIR)/calcul_contour.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

# Test executables
test_image: $(BIN_DIR)/test_image
$(BIN_DIR)/test_image: $(BUILD_DIR)/test_image.o $(BUILD_DIR)/image.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

test-geom2d: $(BIN_DIR)/test-geom2d
$(BIN_DIR)/test-geom2d: $(BUILD_DIR)/test-geom2d.o $(BUILD_DIR)/geom2d.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

test_contour: $(BIN_DIR)/test_contour
$(BIN_DIR)/test_contour: $(BUILD_DIR)/test_contour.o $(BUILD_DIR)/calcul_contour.o $(BUILD_DIR)/geom2d.o $(BUILD_DIR)/image.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

test_simplific_douglas: $(BIN_DIR)/test_simplific_douglas
$(BIN_DIR)/test_simplific_douglas: $(BUILD_DIR)/test_simplific_douglas.o $(BUILD_DIR)/calcul_contour.o $(BUILD_DIR)/geom2d.o $(BUILD_DIR)/image.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

test_bezier: $(BIN_DIR)/test_bezier
$(BIN_DIR)/test_bezier: $(BUILD_DIR)/test_bezier.o $(BUILD_DIR)/image.o $(BUILD_DIR)/geom2d.o $(BUILD_DIR)/bezier.o $(BUILD_DIR)/calcul_contour.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

test_douglas_bezier2: $(BIN_DIR)/test_douglas_bezier2
$(BIN_DIR)/test_douglas_bezier2: $(BUILD_DIR)/test_douglas_bezier2.o $(BUILD_DIR)/image.o $(BUILD_DIR)/geom2d.o $(BUILD_DIR)/bezier.o $(BUILD_DIR)/calcul_contour.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

test_bezier3: $(BIN_DIR)/test_bezier3
$(BIN_DIR)/test_bezier3: $(BUILD_DIR)/test_bezier3.o $(BUILD_DIR)/image.o $(BUILD_DIR)/geom2d.o $(BUILD_DIR)/bezier.o $(BUILD_DIR)/calcul_contour.o
	@echo "Linking $@..."
	$(CC) $^ $(LDFLAGS) -o $@

# Clean up
clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)

# Run all tests
.PHONY: test
test: tests
	@echo "Running tests..."
	$(BIN_DIR)/test_image samples/test1.pbm
	$(BIN_DIR)/test-geom2d
	$(BIN_DIR)/test_contour samples/test1.pbm
	$(BIN_DIR)/test_simplific_douglas samples/test1.pbm 0.5
	$(BIN_DIR)/test_bezier samples/test1.pbm stroke 2
	$(BIN_DIR)/test_douglas_bezier2 samples/test1.pbm stroke 2
	$(BIN_DIR)/test_bezier3 samples/test1.pbm stroke 2

# Show help
.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all          - Build main program (default)"
	@echo "  main         - Build main program only"
	@echo "  tests        - Build all test programs"
	@echo "  everything   - Build main program and all tests"
	@echo "  clean        - Remove all build artifacts"
	@echo "  test         - Build and run all tests"
	@echo "  help         - Show this help message"
