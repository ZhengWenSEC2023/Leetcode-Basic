#include <stdio.h>
void afunc(void) {
    static int long level = 0;
    int stack_var;
    if (++level == 5) {
        return;
    }
    printf("stack_var is at: %p\n", &stack_var);
    afunc();
}