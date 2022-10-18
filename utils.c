#include <stdio.h>
#include "utils.h"

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
