#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char *p1 = "abcdefabcdef";
    char *p2 = NULL;
    char *p3 = NULL;

    p2 = memchr(p1, 'c', 10);
    printf("search in 10, p2 - p1 = %ld\n", p2 - p1);
    p3 = memchr(p1, 'c', 2);
    printf("search in 2, p3 - p1 = %ld\n", p3 - p1);

    return 0;
}