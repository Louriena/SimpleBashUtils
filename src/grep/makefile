CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -I. 
RM = rm -f

TARGET = s21_grep
SRCS = main.c parse_args.c my_grep.c
OBJS = $(SRCS:.c=.o)

HEADERS = my_grep.h parse_args.h
TEST_SCRIPT = test_s21_grep.sh

all: $(TARGET)

test: $(TARGET)
	@if [ -f "$(TEST_SCRIPT)" ]; then \
		echo "Running tests..."; \
		chmod +x $(TEST_SCRIPT); \
		sh $(TEST_SCRIPT); \
	else \
		echo "Error: Test script $(TEST_SCRIPT) not found"; \
		exit 1; \
	fi


$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(OBJS) -o $(TARGET) 

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

rebuild: 
	clean all

clean:
	$(RM) $(OBJS) $(TARGET)

.PHONY: all clean
