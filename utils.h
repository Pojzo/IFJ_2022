/*
 * IFJ2022
 * autori:
 * xgazdi04 - Matus Gazdík
 * xjokay00 - David Jokay
 * xkovac66 - Peter Kováč
 * xbuchm02 - Marek Buch
 */
#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>

bool is_digit(char c);
bool is_alpha(char c);
bool is_lower(char c);
bool is_upper(char c);

bool arr_contains_str(const char **arr, const char *element, int arr_len);
bool arr_contains_char(const char *arr, char c, int arr_len);

void debug_print_state(const char *state_name, const char *string_ptr, int string_length);
#endif
