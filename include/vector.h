#ifndef VECTOR_H
#define VECTOR_H 1

#include "linalg.h"
#include "matrix.h"

/* Operation was not successful due to one or more of
 * the operands' dimensions */
#define LAVEC_INCOMPATIBLE_DIM 1

/* Operation was not successful due to one of the coordinates
 * being outside the bounds of a vector. */
#define LAVEC_OOB 2


typedef struct vector vector;

/* Creates a new vector.
 * Vectors differ from matrices in that vectors try to adapt
 * to either a row or column vector, depending on the operation.
 * If data is NULL, the initial elements of v are undefined. */
int vec_new(vector **v, LINALG_SCALAR *data, int dim);

/* Allocates a vector in an invalid state.
 * Vectors created by this function can be used for function output. */
int vec_alloc(vector **v);

/* Creates a basis vector, with the 1 at index pos */
int vec_basis(vector **v, int dim, int pos);

/* Creates a null vector. */
int vec_zero(vector **v, int dim);

/* Creates a new vector with the same values as src. */
int vec_dup(vector **dst, const vector *src);

/* Copies src's values into dst. */
int vec_cpy(vector *dst, const vector *src);

/* Frees resources allocated for v. */
int vec_del(vector *v);


/* Writes v's dimension to *dim. */
int vec_dim(const vector *v, int *dim);

/* Copies v's elements into data. */
int vec_get_data(vector *v, LINALG_SCALAR *data);

/* Copies data's elements into v. */
int vec_set_data(vector *v, const LINALG_SCALAR *data);


/* Writes the element with corresponding position into out.
 * Possible errors:
 *  - LAVEC_OOB */
int vec_read(const vector *v, int i, LINALG_SCALAR *out);

/* Alternative version of vec_read for easier handling.
 * Returns the element with corresponding position in the vector.
 * No error or boundary checks are made. Useful for quick acces to values. */
LINALG_SCALAR vec_get(const vector *v, int i);

/* Writes r into the vector's corresponding position.
 * Possible errors:
 *  - LAVEC_OOB */
int vec_write(vector *v, int i, LINALG_SCALAR r);

/* Unsafe version of mat_write.
 * Writes r into the matrix's corresponding position.
 * No error checks are made. */
void vec_set(vector *v, int i, LINALG_SCALAR r);

/* Writes the result of a + b into out.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_add(const vector *a, const vector *b, vector *out);

/* Writes the result of a + b into a.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_add_(vector *a, const vector *b);

/* Writes the result of a - b into out.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_sub(const vector *a, const vector *b, vector *out);

/* Writes the result of a - b into a.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_sub_(vector *a, const vector *b);

/* Writes the dot product of a and b to *r.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_dot(const vector *a, const vector *b, LINALG_SCALAR *r);

/* Scales v by r and writes to out. */
int vec_smul(const vector *v, LINALG_SCALAR r, vector *out);

/* Scales v by r and writes back to v. */
int vec_smul_(vector *v, LINALG_SCALAR r);

/* Scales v by 1/r and writes to out. */
int vec_sdiv(const vector *v, LINALG_SCALAR r, vector *out);

/* Scales v by 1/r and writes back to v. */
int vec_sdiv_(vector *v, LINALG_SCALAR r);

/* Writes the result of v * m into out.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_mmul_l(const vector *v, const matrix *m, vector *out);

/* Writes the result of v * m into v.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_mmul_l_(vector *v, const matrix *m);

/* Writes the result of m * v into out.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_mmul_r(const vector *v, const matrix *m, vector *out);

/* Writes the result of m * v into v.
 * Possible errors:
 *  - LAVEC_INCOMPATIBLE_DIM */
int vec_mmul_r_(vector *v, const matrix *m);

#endif
