#include <stdio.h>
#include <omp.h>

int main() {
    int n = 16 * 100;
    #pragma omp parallel for
    for (int j = 0; j < n; ++j) {
        printf("%d\n", j);
    }
}