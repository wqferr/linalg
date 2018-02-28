#include <stdio.h>

#include "matrix.h"
#include "vector.h"


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


void vec_print(const vector *v) {
    int i, dim;

    vec_dim(v, &dim);
    for (i = 0; i < dim; i++) {
        printf("%f\t", vec_get(v, i));
    }
    printf("\n\n");
}


int main(void) {
    LINALG_SCALAR ma[] = {
        1, 2, 3,
        4, 5, 6};
    LINALG_SCALAR vb[] = {1, 1, 1};
    LINALG_SCALAR vc[] = {1, 2};

    LINALG_SCALAR res;
    matrix *a;
    vector *b;
    vector *c;
    vector *x;

    mat_new(&a, ma, 2, 3);
    vec_new(&b, vb, 3);
    vec_new(&c, vc, 2);
    vec_alloc(&x);

    printf("%d\n", vec_mmul_r_(b, a));
    printf("%d\n", vec_dot(b, c, &res));
    printf("%d\n", vec_smul(c, res, x));
    printf("%f\n", res);
    vec_print(x);

    mat_del(a);
    vec_del(b);
    vec_del(c);
    vec_del(x);
    return 0;
}
