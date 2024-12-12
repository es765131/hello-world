#include <stdio.h>
#include <stdlib.h>
#include <linux/random.h>

int main() {
    int random_int;
    size_t buflen = sizeof(int);

    // Generate random bytes for an int
    if (getrandom(&random_int, buflen, 0) != buflen) {
        perror("getrandom failed");
        return 1;
    }

    printf("Random int: %d\n", random_int);

    return 0;
}