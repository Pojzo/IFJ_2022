#include <stdio.h>
#include "utils.h"
#include <string.h> 
bool is_digit(char c) {
    return c >= 48 && c <= 57;
}

bool is_alpha(char c) {
    return is_lower(c) || is_upper(c);
}

bool is_lower(char c) {
    return (c >= 97 && c <= 122);
}

bool is_upper(char c) {
    return (c >= 65 && c <= 90);
}

bool arr_contains_str(const char **arr, const char *element, int arr_len) {
    for (int i = 0; i < arr_len; i++) {
        if (strcmp(arr[i], element) == 0) {
            return true;
        }
    }
    return false;
}

bool arr_contains_char(const char *arr, char c, int arr_len) {
    for (int i = 0; i < arr_len; i++) {
        if (arr[i] == c) {
            return true;
        }
    }
    return false;
}
