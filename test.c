#include <stdio.h>
#include <stdlib.h>

int *get_n_primes(int n) {
    int *primes = malloc(sizeof(int) * n);
    int i = 0;
    int j = 2;
    while (i < n) {
        int is_prime = 1;
        for (int k = 2; k < j; k++) {
            if (j % k == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            primes[i] = j;
            i++;
        }
        j++;
    }
    return primes;
}

// find first 100 prime numbers and print them

int main() {
    int *primes = get_n_primes(100);
    for (int i = 0; i < 100; i++) {
        printf("%d ", primes[i]);
    }
    

    return 0;
}