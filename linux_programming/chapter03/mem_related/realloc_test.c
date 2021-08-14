#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[], char* envp[]) {
    int input;
    int n;
    int *numbers1;
    int *numbers2;
    const int num2_size = 20;
    numbers1 = NULL;
    if ((numbers2 = (int*)malloc(num2_size * sizeof(int))) == NULL) {
        printf("malloc memory unsuccessfull");
        numbers2 = NULL;
        exit(1);
    }

    for (n = 0; n < num2_size; n++) {
        *(numbers2 + n) = n;
        printf("numbers2's data: %d\n", *(numbers2 + n));
    }
    printf("Enter an integer value you want to remalloc (enter 0 to stop)\n");
    scanf("%d", &input);
    numbers1 = (int*)realloc(numbers2, (input + num2_size) * sizeof(int));
    if (numbers1 == NULL) {
        printf("Error (re)allocting memory");
        exit(1);
    }
    for (n = 0; n < num2_size; n++) {
        printf("the numbers1s's data copy from numbers2: %d\n", *(numbers1 + n));
    }
    for (n = 0; n < num2_size; n++) {
        printf("the data still in numbers2: %d\n", *(numbers2 + n));
    }
    for (n = 0; n < input; n++) {
        *(numbers1 + num2_size + n) = n * 2;
        printf("number1's new data: %d\n", *(numbers1 + num2_size + n));
    }
    printf("\n");
    free(numbers1);
    numbers1 = NULL;
    return 0;
}