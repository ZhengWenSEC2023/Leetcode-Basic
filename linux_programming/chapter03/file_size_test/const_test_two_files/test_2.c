#include <stdio.h>

char *a = "hello";
int main() {
    char *b = "hello";
    printf("In main: ");
    if (a == b) {
        printf("Yes, a == b\n");
    }
    else {
        printf("No, a != b\n");
    }
    printf("In subfunction: ");
    test();
    return 0;
}