#include <stdio.h>
int* test(void) {
    int i = 10;
    return &i;
}

int main(void) {
    int *p;
    p = test();
    printf("p = %d\n", p);
    return 0;
}