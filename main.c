#include <stdio.h>
#include "include/wrapper.h"


int main(void) {
    if (htj2k_encode()) {
        printf("Encode error.\n");
        return -1;
    }
    printf("Encode success.\n");

    if (htj2k_decode()) {
        printf("Decode error.\n");
        return -1;
    }
    printf("Decode success.\n");

    return 0;
}
