#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utils.h"


// write a test for each function in utils.c

bool test_is_digit() {
    return is_digit('0') && is_digit('1') && is_digit('2') && is_digit('3') && is_digit('4') && is_digit('5') && is_digit('6') && is_digit('7') && is_digit('8') && is_digit('9');
}

bool test_is_alpha() {
    return is_alpha('a') && is_alpha('b') && is_alpha('c') && is_alpha('d') && is_alpha('e') && is_alpha('f') && is_alpha('g') && is_alpha('h') && is_alpha('i') && is_alpha('j') && is_alpha('k') && is_alpha('l') && is_alpha('m') && is_alpha('n') && is_alpha('o') && is_alpha('p') && is_alpha('q') && is_alpha('r') && is_alpha('s') && is_alpha('t') && is_alpha('u') && is_alpha('v') && is_alpha('w') && is_alpha('x') && is_alpha('y') && is_alpha('z') && is_alpha('A') && is_alpha('B') && is_alpha('C') && is_alpha('D') && is_alpha('E') && is_alpha('F') && is_alpha('G') && is_alpha('H') && is_alpha('I') && is_alpha('J') && is_alpha('K') && is_alpha('L') && is_alpha('M') && is_alpha('N') && is_alpha('O') && is_alpha('P') && is_alpha('Q') && is_alpha('R') && is_alpha('S') && is_alpha('T') && is_alpha('U') && is_alpha('V') && is_alpha('W') && is_alpha('X') && is_alpha('Y') && is_alpha('Z');
}

bool test_is_lower() {
    return is_lower('a') && is_lower('b') && is_lower('c') && is_lower('d') && is_lower('e') && is_lower('f') && is_lower('g') && is_lower('h') && is_lower('i') && is_lower('j') && is_lower('k') && is_lower('l') && is_lower('m') && is_lower('n') && is_lower('o') && is_lower('p') && is_lower('q') && is_lower('r') && is_lower('s') && is_lower('t') && is_lower('u') && is_lower('v') && is_lower('w') && is_lower('x') && is_lower('y') && is_lower('z');
}

bool test_is_upper() {
    return is_upper('A') && is_upper('B') && is_upper('C') && is_upper('D') && is_upper('E') && is_upper('F') && is_upper('G') && is_upper('H') && is_upper('I') && is_upper('J') && is_upper('K') && is_upper('L') && is_upper('M') && is_upper('N') && is_upper('O') && is_upper('P') && is_upper('Q') && is_upper('R') && is_upper('S') && is_upper('T') && is_upper('U') && is_upper('V') && is_upper('W') && is_upper('X') && is_upper('Y') && is_upper('Z');
}

bool test_arr_contains_str() {
    const char *arr[] = {"hello", "world", "this", "is", "a", "test"};
    return arr_contains_str(arr, "hello", 6) && arr_contains_str(arr, "world", 6) && arr_contains_str(arr, "this", 6) && arr_contains_str(arr, "is", 6) && arr_contains_str(arr, "a", 6) && arr_contains_str(arr, "test", 6);
}

bool test_arr_contains_char() {
    const char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    return arr_contains_char(arr, 'a', 26) && arr_contains_char(arr, 'b', 26) && arr_contains_char(arr, 'c', 26) && arr_contains_char(arr, 'd', 26) && arr_contains_char(arr, 'e', 26) && arr_contains_char(arr, 'f', 26) && arr_contains_char(arr, 'g', 26) && arr_contains_char(arr, 'h', 26) && arr_contains_char(arr, 'i', 26) && arr_contains_char(arr, 'j', 26) && arr_contains_char(arr, 'k', 26) && arr_contains_char(arr, 'l', 26) && arr_contains_char(arr, 'm', 26) && arr_contains_char(arr, 'n', 26) && arr_contains_char(arr, 'o', 26) && arr_contains_char(arr, 'p', 26) && arr_contains_char(arr, 'q', 26) && arr_contains_char(arr, 'r', 26) && arr_contains_char(arr, 's', 26) && arr_contains_char(arr, 't', 26) && arr_contains_char(arr, 'u', 26) && arr_contains_char(arr, 'v', 26) && arr_contains_char(arr, 'w', 26) && arr_contains_char(arr, 'x', 26) && arr_contains_char(arr, 'y', 26) && arr_contains_char(arr, 'z', 26);
}