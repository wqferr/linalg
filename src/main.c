#include <stdio.h>

#include "matrix.h"


void mat_print(const matrix *m) {
    int i, j;
    int rows, cols;

    mat_dim(m, &rows, &cols);
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%f\t", mat_get(m, i, j));
        }
        printf("\n\n");
    }
}


int main(void) {
    LINALG_SCALAR ma[] = {
        1, 2, 3,
        4, 5, 6};
    LINALG_SCALAR mb[] = {
        1, 1,
        1, 1,
        1, 1};
    matrix *a;
    matrix *b;
    matrix *c;

    mat_new(&a, ma, 2, 3);
    mat_new(&b, mb, 3, 2);
    mat_alloc(&c);

    mat_transpose_(a);
    mat_add(a, b, c);
    mat_print(c);

    mat_del(a);
    mat_del(b);
    mat_del(c);
    return 0;
}
