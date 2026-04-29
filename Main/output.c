#include <stdio.h>

int main(void) {
    int X;
    scanf("%d", &X);
    if ((X < 20)) {
        printf("%d\n", 2);
    } else if ((X < 10)) {
    printf("%d\n", 1);
} else {
    printf("%d\n", 0);
}
    return 0;
}
