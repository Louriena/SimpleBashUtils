# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Source files
SRCS = main.c s21_cat.c parse_args.c
OBJS = $(SRCS:.c=.o)
TARGET = s21_cat
TEST_SCRIPT = test_s21_cat.sh

# Default target
all: $(TARGET)

# Build target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run tests
test: $(TARGET)
	@if [ -f "$(TEST_SCRIPT)" ]; then \
		echo "Running tests..."; \
		chmod +x $(TEST_SCRIPT); \
		sh $(TEST_SCRIPT); \
	else \
		echo "Error: Test script $(TEST_SCRIPT) not found"; \
		exit 1; \
	fi

# Phony targets (not files)
.PHONY: all clean rebuild test