#include <stdio.h>
char *a = "hello";
void test(void) {
    printf("In subfunction:\n");
    char *c = "hello";
    if (a == c) {
        printf("Yes, a == c\n");
    }
    else {
        printf("No, a != c\n");
    }
}

int main(void) {
    printf("In function main:\n");
    char *b = "hello";
    char *d = "hello1";
    if (a == b) {
        printf("Yes, a == b\n"); 
    }
    else {
        printf("No, a != b\n");
    }
    test();
    if (a == d) {
        printf("Yes, a == d\n");
    }
    else {
        printf("No, a != d\n");
    }
    return 0;
}