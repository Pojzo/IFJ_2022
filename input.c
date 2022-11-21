const char *TOK_STR[] = {"TOK_ID", "TOK_ID_FUNCTION", "TOK_KEYWORD", "TOK_SEPARATOR", "TOK_OPERATOR", "TOK_LITERAL"};
const char whitespace = ' ';
const char operators[] = {'*', '+', '-', '=', '/', '%', '<', '>', '.'};
const char *string_operators[] = {"===", "!==", ">=", "<="};
const char *keywords[] = {"if", "else", "float", "function", "int", "null", "return", "string", "while", "void"}; 
const char separators[] = {' ', '(', ')', '{', '}', '[', ']', ';', ',', '\n', '\r'};
const int separ_len = 11;
const int oper_len = 9;
const int string_oper_len = 4;
const int keywords_len = 10;


// we have a programming languge called ifj
// it has a bunch of keywords defined above 
// also operators, separators and other stuff
// it's dyamically typed, so we don't need to define types
// variables are prefixed with $

// this is example program

/*
<?php
declare(strict_types=1);
// Program 1: Calculate the factorial (iteratively)
// Main body of the program
write("Enter a number to calculate the factorial\n");
$a = read();
if ($a === null) {
write("Error loading integer!\n");
} else {}
if ($a < 0) {
write("Factorial cannot be calculated\n");
} else {
$out = 1;
while ($a > 0) {
11
$out = $out * $a;
$a = $a - 1;
}
write("The result is: ", $result, "\n");
}
8.2 Calculating the factorial (recursively)
<?php
declare(strict_types=1);
// Program 2: Calculate the factorial (recursively)
// Main body of the program
write("Enter a number to calculate the factorial: ");
$a = read();
// Definition of the function to calculate the value of the factorial
function factorial(int $n) : int {
if ($n < 2) {
$result = 1;
} else {
$decremented_n = $n - 1;
$temp_result = factorial($decremented_n);
$result = $n * $temp_result;
}
return $result;
}
if ($a !== null) {
if ($a < 0) { // Continuation of the main body of the program
write("Factorial cannot be calculated\n");
} else {
$output = factorial($a);
write("The result is: ", $result, "\n");
}
} else {
write("Error loading integer!\n");
}
8.3 Working with strings and built-in functions
<?php
declare(strict_types=1);

$str1 = "This is some text in the IFJ22 language program";
$str2 = $str1 . "which we will enrich a little";
write($str1, "\n", $str2, "\n");
write("Position of string \"text\" in \$str2: ", 15, "\n");
write("Enter a sorted sequence of all lowercase letters a-h, ");
$str1 = reads();
if ($str1 !== null) {
12
while ($str1 !== "abcdefgh") {
write("Entered sequence failed, try again:\n");
$str1 = reads();
}
} else { }
?>

*/

function add_numbers(int $a, int $b) : int{
    return $a + $b;
}

function sub_numbers(int $a, int $b) : int{
    return $a - $b;
}



function is_even(int $a) : bool{
    return $a % 2 == 0;
}

function is_odd(int $a) : bool{
    return $a % 2 == 1;
}

function is_divisible(int $a, int $b) : bool{
    return $a % $b == 0;
}

function is_divisible_by_3(int $a) : bool{
    return is_divisible($a, 3);
}

function is_divisible_by_5(int $a) : bool{
    return is_divisible($a, 5);
}