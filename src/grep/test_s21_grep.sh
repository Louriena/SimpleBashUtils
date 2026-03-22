#!/bin/bash

# Comprehensive test script for s21_grep implementation
# Usage: ./test_s21_grep.sh

set -eo pipefail

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Paths
GREP_EXEC="./s21_grep"
SYSTEM_GREP="grep"
TEST_DIR="test_data_grep"
LOG_FILE="test_results_grep.log"

# Initialize
mkdir -p "$TEST_DIR"
rm -f "$LOG_FILE"
total_tests=0
passed_tests=0

# Test cases
declare -a BASIC_FLAGS=("" "-i" "-v" "-c" "-l" "-n")
declare -a E_FLAGS=("-e hello" "-e hello -e world")
declare -a COMBINATION_FLAGS=("-iv" "-cn" "-ln")
declare -a TEST_FILES=("text1.txt" "text2.txt" "empty.txt" "special.txt")

# Create test files
create_test_files() {
    # File 1
    echo -e "hello world\nHello World\nHELLO WORLD\n\nGoodbye world" > "$TEST_DIR/text1.txt"
    
    # File 2
    echo -e "grep test\nline with hello\nanother line\nlast line" > "$TEST_DIR/text2.txt"
    
    # Empty file
    touch "$TEST_DIR/empty.txt"
    
    # File with special chars
    echo -e "line\twith\ttabs\nline with \$ sign\n^start line" > "$TEST_DIR/special.txt"
}

# Run single test
run_test() {
    local flags=$1
    local pattern=$2
    local file=$3
    local test_num=$4
    
    echo "Test $test_num: s21_grep $flags $pattern $file" >> "$LOG_FILE"
    
    # Handle cases with -e flags (pattern is empty as it's part of flags)
    if [[ "$flags" == *-e* ]]; then
        $GREP_EXEC $flags "$TEST_DIR/$file"  > "$TEST_DIR/s21_output.txt" 2>&1
        $SYSTEM_GREP $flags "$TEST_DIR/$file"   > "$TEST_DIR/system_output.txt" 2>&1
    else
        $GREP_EXEC $flags "$pattern" "$TEST_DIR/$file"  > "$TEST_DIR/s21_output.txt" 2>&1
        $SYSTEM_GREP $flags "$pattern" "$TEST_DIR/$file"  > "$TEST_DIR/system_output.txt" 2>&1
    fi
    
    # Compare results
    if diff -u "$TEST_DIR/s21_output.txt" "$TEST_DIR/system_output.txt" >> "$LOG_FILE"; then
        printf "${GREEN}PASS${NC} Test %d: s21_grep %s %s %s\n" "$test_num" "$flags" "$pattern" "$file"
        ((passed_tests++))
        return 0
    else
        printf "${RED}FAIL${NC} Test %d: s21_grep %s %s %s\n" "$test_num" "$flags" "$pattern" "$file"
        echo "--- Differences found ---" >> "$LOG_FILE"
        return 1
    fi
}

# Main test function
run_test_suite() {
    local test_num=1
    
    echo "Starting s21_grep test suite..."
    echo "=============================="
    
    # Basic pattern tests with single patterns
    local patterns=("hello" "Hello" "world" "nonexistent")
    
    # Regular tests with basic flags
    for pattern in "${patterns[@]}"; do
        for flags in "${BASIC_FLAGS[@]}"; do
            for file in "${TEST_FILES[@]}"; do
                ((total_tests++))
                run_test "$flags" "$pattern" "$file" "$test_num" || true
                ((test_num++))
            done
        done
    done
    
    # Tests with -e flags (pattern is empty as it's part of flags)
    for flags in "${E_FLAGS[@]}"; do
        for file in "${TEST_FILES[@]}"; do
            ((total_tests++))
            run_test "$flags" "" "$file" "$test_num" || true
            ((test_num++))
        done
    done
    
    # Tests with combination flags
    for pattern in "${patterns[@]}"; do
        for flags in "${COMBINATION_FLAGS[@]}"; do
            for file in "${TEST_FILES[@]}"; do
                ((total_tests++))
                run_test "$flags" "$pattern" "$file" "$test_num" || true
                ((test_num++))
            done
        done
    done
    
    # Multiple files test
    ((total_tests++))
    echo "Test $test_num: Multiple files" >> "$LOG_FILE"
    $GREP_EXEC -n hello "$TEST_DIR/text1.txt" "$TEST_DIR/text2.txt"  > "$TEST_DIR/s21_output.txt" 2>&1
    $SYSTEM_GREP -n hello "$TEST_DIR/text1.txt" "$TEST_DIR/text2.txt" "$TEST_DIR/system_output.txt" 2>&1
    if diff -u "$TEST_DIR/s21_output.txt" "$TEST_DIR/system_output.txt" >> "$LOG_FILE"; then
        printf "${GREEN}PASS${NC} Test %d: Multiple files\n" "$test_num"
        ((passed_tests++))
    else
        printf "${RED}FAIL${NC} Test %d: Multiple files\n" "$test_num"
    fi
    ((test_num++))
    
    # результаты
    echo "=============================="
    printf "Tests completed: %d\n" "$total_tests"
    printf "${GREEN}Passed: %d${NC}\n" "$passed_tests"
    printf "${RED}Failed: %d${NC}\n" "$((total_tests - passed_tests))"
    
    if [ $passed_tests -ne $total_tests ]; then
        printf "\nDetailed differences can be found in %s\n" "$LOG_FILE"
        exit 1
    fi
}

# чистка
cleanup() {
    rm -rf "$TEST_DIR"
}

# Main execution
trap cleanup EXIT
create_test_files
run_test_suite