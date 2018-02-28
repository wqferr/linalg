#include "vector.h"

#include <stdlib.h>
#include <string.h>


struct vector {
    LINALG_SCALAR *data;
    int dim;
};


int vec_new(vector **out, LINALG_SCALAR *data, int dim) {
    vector *v = malloc(sizeof(*v));
    v->data = malloc(dim * sizeof(*v->data));
    if (data != NULL) {
        memcpy(v->data, data, dim * sizeof(*v->data));
    }
    v->dim = dim;

    *out = v;
    return 0;
}


int vec_alloc(vector **out) {
    vector *v = malloc(sizeof(*v));
    v->data = NULL;
    v->dim = 0;

    *out = v;
    return 0;
}


int vec_basis(vector **out, int dim, int pos) {
    int err;
    vector *v;

    err = vec_zero(&v, dim);
    if (err != 0) {
        return err;
    }
    err = vec_write(v, pos, 1);
    if (err != 0) {
        vec_del(v);
        return err;
    }

    *out = v;
    return 0;
}


int vec_zero(vector **out, int dim) {
    vector *v;

    v = malloc(sizeof(*v));
    v->data = calloc(dim, sizeof(*v->data));
    v->dim = dim;

    *out = v;
    return 0;
}


int vec_dup(vector **dst, const vector *src) {
    int err;
    vector *v;

    err = vec_new(&v, src->data, src->dim);
    if (err != 0) {
        return err;
    }

    *dst = v;
    return 0;
}


int vec_cpy(vector *dst, const vector *src) {
    dst->data = realloc(dst->data, src->dim * sizeof(*src->data));
    dst->dim = src->dim;
    memcpy(dst->data, src->data, src->dim * sizeof(*src->data));

    return 0;
}


int vec_del(vector *v) {
    free(v->data);
    free(v);
    return 0;
}


int vec_dim(const vector *v, int *dim) {
    *dim = v->dim;
    return 0;
}


int vec_get_data(vector *v, LINALG_SCALAR *data) {
    memcpy(data, v->data, v->dim * sizeof(*v->data));
    return 0;
}


int vec_set_data(vector *v, const LINALG_SCALAR *data) {
    memcpy(v->data, data, v->dim * sizeof(*v->data));
    return 0;
}


int vec_read(const vector *v, int i, LINALG_SCALAR *out) {
    if (i < 0 || i >= v->dim) {
        return LAVEC_OOB;
    }
    *out = vec_get(v, i);
    return 0;
}


LINALG_SCALAR vec_get(const vector *v, int i) {
    return v->data[i];
}


int vec_write(vector *v, int i, LINALG_SCALAR r) {
    if (i < 0 || i >= v->dim) {
        return LAVEC_OOB;
    }
    vec_set(v, i, r);
    return 0;
}


void vec_set(vector *v, int i, LINALG_SCALAR r) {
    v->data[i] = r;
}


int vec_add(const vector *a, const vector *b, vector *out) {
    int err;
    vector *tmp;

    err = vec_dup(&tmp, a);
    if (err != 0) {
        return err;
    }
    err = vec_add_(tmp, b);
    if (err != 0) {
        vec_del(tmp);
        return err;
    }
    err = vec_cpy(out, tmp);
    vec_del(tmp);
    return err;
}


int vec_add_(vector *a, const vector *b) {
    int i, dim;
    if (a-> dim != b->dim) {
        return LAVEC_INCOMPATIBLE_DIM;
    }

    dim = a->dim;
    for (i = 0; i < dim; i++) {
        a->data[i] += b->data[i];
    }
    return 0;
}


int vec_sub(const vector *a, const vector *b, vector *out) {
    int err;
    vector *tmp;

    err = vec_dup(&tmp, a);
    if (err != 0) {
        return err;
    }
    err = vec_sub_(tmp, b);
    if (err != 0) {
        vec_del(tmp);
        return err;
    }
    err = vec_cpy(out, tmp);
    vec_del(tmp);
    return err;
}


int vec_sub_(vector *a, const vector *b) {
    int i, dim;
    if (a-> dim != b->dim) {
        return LAVEC_INCOMPATIBLE_DIM;
    }

    dim = a->dim;
    for (i = 0; i < dim; i++) {
        a->data[i] -= b->data[i];
    }
    return 0;
}


int vec_dot(const vector *a, const vector *b, LINALG_SCALAR *out) {
    int i, dim;
    LINALG_SCALAR r;

    if (a->dim != b->dim) {
        return LAVEC_INCOMPATIBLE_DIM;
    }

    dim = a->dim;
    r = 0;
    for (i = 0; i < dim; i++) {
        r += vec_get(a, i) * vec_get(b, i);
    }

    *out = r;
    return 0;
}


int vec_smul(const vector *v, LINALG_SCALAR r, vector *out) {
    int err;
    vector *tmp;

    err = vec_dup(&tmp, v);
    if (err != 0) {
        return err;
    }
    err = vec_smul_(tmp, r);
    if (err != 0) {
        vec_del(tmp);
        return err;
    }
    err = vec_cpy(out, tmp);
    vec_del(tmp);
    return err;
}


int vec_smul_(vector *v, LINALG_SCALAR r) {
    int i, dim;
    
    dim = v->dim;
    for (i = 0; i < dim; i++) {
        v->data[i] *= r;
    }
    return 0;
}


int vec_sdiv(const vector *v, LINALG_SCALAR r, vector *out) {
    int err;
    vector *tmp;

    err = vec_dup(&tmp, v);
    if (err != 0) {
        return err;
    }
    err = vec_sdiv_(tmp, r);
    if (err != 0) {
        vec_del(tmp);
        return err;
    }
    err = vec_cpy(out, tmp);
    vec_del(tmp);
    return err;
}


int vec_sdiv_(vector *v, LINALG_SCALAR r) {
    int i, dim;
    
    dim = v->dim;
    for (i = 0; i < dim; i++) {
        v->data[i] /= r;
    }
    return 0;
}


int vec_mmul_l(const vector *v, const matrix *m, vector *out) {
    int err;
    vector *tmp;

    err = vec_dup(&tmp, v);
    if (err != 0) {
        return err;
    }

    err = vec_mmul_l_(tmp, m);
    if (err != 0) {
        vec_del(tmp);
        return err;
    }

    err = vec_cpy(out, tmp);
    vec_del(tmp);
    return err;
}


int vec_mmul_l_(vector *v, const matrix *m) {
    int i, j, mrows, mcols;
    int err;
    LINALG_SCALAR s;
    vector *tmp;

    mat_dim(m, &mrows, &mcols);
    if (v->dim != mrows) {
        return LAVEC_INCOMPATIBLE_DIM;
    }
    
    err = vec_new(&tmp, NULL, mrows);
    if (err != 0) {
        return err;
    }

    for (i = 0; i < mcols; i++) {
        s = 0;
        for (j = 0; j < mrows; j++) {
            s += vec_get(v, j) * mat_get(m, j, i);
        }
        vec_set(tmp, i, s);
    }

    err = vec_cpy(v, tmp);
    vec_del(tmp);
    return err;
}


int vec_mmul_r(const matrix *m, const vector *v, vector *out) {
    int err;
    vector *tmp;

    err = vec_dup(&tmp, v);
    if (err != 0) {
        return err;
    }

    err = vec_mmul_r_(m, tmp);
    if (err != 0) {
        vec_del(tmp);
        return err;
    }

    err = vec_cpy(out, tmp);
    vec_del(tmp);
    return err;
}


int vec_mmul_r_(const matrix *m, vector *v) {
    int i, j, mrows, mcols;
    int err;
    LINALG_SCALAR s;
    vector *tmp;

    mat_dim(m, &mrows, &mcols);
    if (v->dim != mcols) {
        return LAVEC_INCOMPATIBLE_DIM;
    }
    
    err = vec_new(&tmp, NULL, mrows);
    if (err != 0) {
        return err;
    }

    for (i = 0; i < mrows; i++) {
        s = 0;
        for (j = 0; j < mcols; j++) {
            s += vec_get(v, j) * mat_get(m, i, j);
        }
        vec_set(tmp, i, s);
    }

    err = vec_cpy(v, tmp);
    vec_del(tmp);
    return err;
}
