#include "matrix.h"

#include <stdlib.h>
#include <string.h>

struct matrix {
    LINALG_REAL *data;
    int rows;
    int cols;
};


int mat_new(matrix **out, const LINALG_REAL *data, int rows, int cols) {
    int bytelen = rows * cols * sizeof(*data);

    matrix *m = malloc(sizeof(*m));
    m->data = malloc(bytelen);
    if (data != NULL) {
        memcpy(m->data, data, bytelen);
    }
    m->rows = rows;
    m->cols = cols;

    *out = m;
    return 0;
}


int mat_alloc(matrix **out) {
    matrix *m = malloc(sizeof(*m));
    m->data = NULL;
    m->rows = 0;
    m->cols = 0;
    *out = m;
    return 0;
}


int mat_id(matrix **out, int order) {
    int err, i;
    matrix *m;

    err = mat_zero(&m, order, order);
    if (err != 0) {
        return err;
    }

    i = 0;
    while (err == 0 && i < order) {
        err = mat_write(m, i, i, 1);
        i++;
    }

    if (err != 0) {
        mat_del(m);
        return err;
    }

    *out = m;
    return 0;
}


int mat_zero(matrix **out, int rows, int cols) {
    int err;
    matrix *m;

    err = mat_new(&m, NULL, rows, cols);
    if (err != 0) {
        return err;
    }

    memset(m->data, 0, rows * cols * sizeof(*m->data));

    *out = m;
    return 0;
}


int mat_dup(matrix **dst, const matrix *src) {
    int err;
    matrix *m;

    err = mat_new(&m, src->data, src->rows, src->cols);
    if (err != 0) {
        return err;
    }

    *dst = m;
    return 0;
}


int mat_cpy(matrix *dst, const matrix *src) {
    int bytelen;

    bytelen = src->rows * src->cols * sizeof(*src->data);
    dst->data = realloc(dst->data, bytelen);
    memcpy(dst->data, src->data, bytelen);
    dst->rows = src->rows;
    dst->cols = src->cols;
    return 0;
}


int mat_del(matrix *m) {
    free(m->data);
    free(m);
    return 0;
}


int mat_dim(const matrix *m, int *rows, int *cols) {
    if (rows != NULL) {
        *rows = m->rows;
    }
    if (cols != NULL) {
        *cols = m->cols;
    }
    return 0;
}


int mat_get_data(matrix *m, LINALG_REAL *out) {
    memcpy(data, m->data, m->rows * m->cols * sizeof(*data));
    return 0;
}


int mat_set_data(matrix *m, LINALG_REAL *data) {
    memcpy(m->data, data, m->rows * m->cols * sizeof(*data));
    return 0;
}


int mat_read(const matrix *m, int row, int col, LINALG_REAL *out) {
    if (row < 0 || row >= m->rows
            || col < 0 || col >= m->cols) {
        return LAMAT_OOB;
    }
    *out = mat_get(m, row, col);
    return 0;
}


LINALG_REAL mat_get(const matrix *m, int row, int col) {
    return m->data[row*m->cols + col];
}


int mat_write(matrix *m, int row, int col, LINALG_REAL r) {
    if (row < 0 || row >= m->rows
            || col < 0 || col >= m->cols) {
        return LAMAT_OOB;
    }
    mat_set(m, row, col, r);
    return 0;
}


void mat_set(matrix *m, int row, int col, LINALG_REAL r) {
    m->data[row*m->cols + col] = r;
}


int mat_add(const matrix *a, const matrix *b, matrix *out) {
    int err;
    matrix *tmp;

    err = mat_dup(&tmp, a);
    if (err != 0) {
        return err;
    }
    err = mat_add_(tmp, b);
    if (err != 0) {
        mat_del(tmp);
        return err;
    }
    mat_cpy(out, tmp);
    mat_del(tmp);
    return 0;
}


int mat_add_(matrix *a, const matrix *b) {
    int i, len;

    if (a->rows != b->rows || a->cols != b->cols) {
        return LAMAT_INCOMPATIBLE_DIM;
    }
    len = a->rows * a->cols;
    for (i = 0; i < len; i++) {
        a->data[i] += b->data[i];
    }
    return 0;
}


int mat_sub(const matrix *a, const matrix *b, matrix *out) {
    int err;
    matrix *tmp;

    err = mat_dup(&tmp, a);
    if (err != 0) {
        return err;
    }
    err = mat_sub_(tmp, b);
    if (err != 0) {
        mat_del(tmp);
        return err;
    }
    mat_cpy(out, tmp);
    mat_del(tmp);
    return 0;
}


int mat_sub_(matrix *a, const matrix *b) {
    int i, len;

    if (a->rows != b->rows || a->cols != b->cols) {
        return LAMAT_INCOMPATIBLE_DIM;
    }
    len = a->rows * a->cols;
    for (i = 0; i < len; i++) {
        a->data[i] -= b->data[i];
    }
    return 0;
}


int mat_mul(const matrix *a, const matrix *b, matrix *out) {
    int i, j, k;
    LINALG_REAL s;

    if (a->cols != b->rows) {
        return LAMAT_INCOMPATIBLE_DIM;
    }
    
    out->rows = a->rows;
    out->cols = b->cols;
    out->data = realloc(out->data, out->rows * out->cols * sizeof(*out->data));

    for (i = 0; i < out->rows; i++) {
        for (j = 0; j < out->cols; j++) {
            s = 0;
            for (k = 0; k < a->cols; k++) {
                s += mat_get(a, i, k) * mat_get(b, k, j);
            }
            mat_set(out, i, j, s);
        }
    }
    return 0;
}


int mat_mul_(matrix *a, const matrix *b) {
    int err;
    matrix *aux;

    err = mat_new(&aux, NULL, a->rows, a->cols);
    if (err != 0) {
        return err;
    }

    err = mat_mul(a, b, aux);
    if (err != 0) {
        mat_del(aux);
        return err;
    }

    err = mat_cpy(a, aux);
    mat_del(aux);
    return err;
}


int mat_smul(const matrix *m, LINALG_REAL s, matrix *out) {
    int err;
    matrix *tmp;
    err = mat_dup(&tmp, m);
    if (err != 0) {
        return err;
    }

    err = mat_smul_(tmp, s);
    if (err != 0) {
        mat_del(tmp);
        return err;
    }
    err = mat_cpy(out, tmp);
    mat_del(tmp);
    return err;
}


int mat_smul_(matrix *m, LINALG_REAL s) {
    int i, len;

    len = m->rows * m->cols;
    for (i = 0; i < len; i++) {
        m->data[i] *= s;
    }
    return 0;
}


int mat_sdiv(const matrix *m, LINALG_REAL s, matrix *out) {
    int err;
    matrix *tmp;

    err = mat_dup(&tmp, m);
    if (err != 0) {
        return err;
    }

    err = mat_sdiv_(tmp, s);
    if (err != 0) {
        mat_del(tmp);
        return err;
    }
    err = mat_cpy(out, tmp);
    mat_del(tmp);
    return err;
}


int mat_sdiv_(matrix *m, LINALG_REAL s) {
    int i, len;

    len = m->rows * m->cols;
    for (i = 0; i < len; i++) {
        m->data[i] /= s;
    }
    return 0;
}


int mat_transpose(const matrix *m, matrix *out) {
    int err;
    matrix *tmp;

    err = mat_dup(&tmp, m);
    if (err != 0) {
        return err;
    }
    err = mat_transpose_(tmp);
    if (err != 0) {
        mat_del(tmp);
        return err;
    }
    err = mat_cpy(out, tmp);
    mat_del(tmp);
    return err;
}


int mat_transpose_(matrix *m) {
    int err;
    int i, j;
    matrix *aux;

    err = mat_new(&aux, NULL, m->cols, m->rows);
    if (err != 0) {
        return err;
    }
    for (i = 0; i < m->rows; i++) {
        for (j = 0; j < m->cols; j++) {
            mat_set(aux, j, i, mat_get(m, i, j));
        }
    }
    err = mat_cpy(m, aux);
    mat_del(aux);
    return err;
}
