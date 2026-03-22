#!/bin/bash

# Comprehensive test script for s21_cat implementation
# Usage: ./test_s21_cat.sh

set -eo pipefail

# Colors
RED="\033[0;31m"
GREEN="\033[0;32m"
NC="\033[0m"

# Paths
CAT_EXEC="./s21_cat"
SYSTEM_CAT="/bin/cat"
TEST_DIR="test_data"
LOG_FILE="test_results.log"

# Initialize
mkdir -p "$TEST_DIR"
rm -f "$LOG_FILE"
total_tests=0
passed_tests=0

# Test cases
declare -a FLAGS=("" "-b" "-e" "-n" "-s" "-t" "-v" "-be" "-ben" "-bens" "-bent" "-benv" "-bentv" "-E" "--number-nonblank" "--number" "--squeeze-blank" "-T" "-ET" "--number --squeeze-blank" "--number-nonblank -T")
declare -a TEST_FILES=("ascii.txt" "text.txt" "empty.txt" "special.txt")

# Create test files
create_test_files() {
    # Regular ASCII file
     echo "Line 1\nLine 2\n\nLine 4" > "$TEST_DIR/ascii.txt"
    
    # Text with special characters
     echo "Line\t1\nLine\x02\n\nLine 4$" > "$TEST_DIR/special.txt"
    
    # Empty file
    touch "$TEST_DIR/empty.txt"
    
    # Multi-line text
    seq 1 10 | sed 's/^/Line /' > "$TEST_DIR/text.txt"

}

# Run single test
run_test() {
    local flags=$1
    local file=$2
    local test_num=$3
    
    echo "Test $test_num: s21_cat $flags $file" >> "$LOG_FILE"
    
    # Run both implementations
    $CAT_EXEC $flags "$TEST_DIR/$file"  > "$TEST_DIR/s21_output.txt" 2>&1
    $SYSTEM_CAT $flags "$TEST_DIR/$file"  > "$TEST_DIR/system_output.txt" 2>&1
    
    # Compare results
    if diff -u "$TEST_DIR/s21_output.txt" "$TEST_DIR/system_output.txt" >> "$LOG_FILE"; then
         echo "${GREEN}PASS${NC} Test $test_num: s21_cat $flags $file"
        ((passed_tests++))
        return 0
    else
         echo "${RED}FAIL${NC} Test $test_num: s21_cat $flags $file"
        echo "--- Differences found ---" >> "$LOG_FILE"
        return 1
    fi
}

# Main test function
run_test_suite() {
    local test_num=1
    
    echo "Starting s21_cat test suite..."
    echo "============================="
    
    # Regular tests
    for flags in "${FLAGS[@]}"; do
        for file in "${TEST_FILES[@]}"; do
            ((total_tests++))
            run_test "$flags" "$file" "$test_num" || true
            ((test_num++))
        done
    done
    
    # Additional special cases
    ((total_tests++))
    echo "Test $test_num: Multiple files" >> "$LOG_FILE"
    $CAT_EXEC -n "$TEST_DIR/ascii.txt" "$TEST_DIR/text.txt"  > "$TEST_DIR/s21_output.txt" 2>&1
    $SYSTEM_CAT -n "$TEST_DIR/ascii.txt" "$TEST_DIR/text.txt"  > "$TEST_DIR/system_output.txt" 2>&1
    if diff -u "$TEST_DIR/s21_output.txt" "$TEST_DIR/system_output.txt" >> "$LOG_FILE"; then
         echo "${GREEN}PASS${NC} Test $test_num: Multiple files"
        ((passed_tests++))
    else
         echo "${RED}FAIL${NC} Test $test_num: Multiple files"
    fi
    ((test_num++))
    
    # Stdin test
    ((total_tests++))
    echo "Test $test_num: Stdin input" >> "$LOG_FILE"
    echo "Test input" | $CAT_EXEC -n > "$TEST_DIR/s21_output.txt" 2>&1
    echo "Test input" | $SYSTEM_CAT -n > "$TEST_DIR/system_output.txt" 2>&1
    if diff -u "$TEST_DIR/s21_output.txt" "$TEST_DIR/system_output.txt" >> "$LOG_FILE"; then
         echo "${GREEN}PASS${NC} Test $test_num: Stdin input"
        ((passed_tests++))
    else
         echo "${RED}FAIL${NC} Test $test_num: Stdin input"
    fi
    
    # Summary
    echo "============================="
    echo "Tests completed: $total_tests"
     echo "${GREEN}Passed: $passed_tests${NC}"
     echo "${RED}Failed: $((total_tests - passed_tests))${NC}"
    
    if [ $passed_tests -ne $total_tests ]; then
         echo "\nDetailed differences can be found in $LOG_FILE"
        exit 1
    fi
}

# Cleanup
cleanup() {
    rm -rf "$TEST_DIR"
}

# Main execution
trap cleanup EXIT
create_test_files
run_test_suite