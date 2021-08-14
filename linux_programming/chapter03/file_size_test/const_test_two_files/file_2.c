#include <stdio.h>
extern char *a;
void test(void) {
    char *c = "hello";
    if (a == c) {
        printf("Yes, a == c\n");
    }
    else {
        printf("No, a != c\n");
    }
}
