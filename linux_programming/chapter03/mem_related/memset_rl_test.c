#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void bzero(void *__s, size_t __n);

int main(void) {
    char *p2 = (char *)malloc(4 * sizeof(char));
    memset(p2, '\0', 4);
    printf("memset: memory is %s\n", (char *)memset(p2, 'a', 3));
    bzero(p2 + 2, 2);
    printf("bzero: memory is %s\n", p2);
    return 0;
}