#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int main() {

    float ans = 37.520;
    if (fabs(ans - 37.520) < 0.001) {
        printf("wtf");
        printf("%.2f\n", 37.52);
    } else {
        printf("asdfsdf");
        printf("%.3f\n", ans);
    }

}