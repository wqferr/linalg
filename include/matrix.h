#ifndef MATRIX_H
#define MATRIX_H

/*
 * In-place operations are marked with a trailing underscore.
 */

#define LINALG_REAL float

/* Operation was not successful due to one or more of
 * the operands' dimensions */
#define LAMAT_INCOMPATIBLE_DIM 1

/* Operation was not successful due to one of the coordinates
 * being outside the bounds of a matrix. */
#define LAMAT_OOB 2


typedef struct matrix matrix;

/* Creates a new matrix whose elements are in data,
 * ordered by rows.
 * If data is NULL, the new matrix's elements are undefined. */
int mat_new(matrix **m, const LINALG_REAL *data, int rows, int cols);

/* Creates an identity matrix of the given order. */
int mat_id(matrix **m, int order);

/* Creates a null matrix with the given dimensions. */
int mat_zero(matrix **m, int rows, int cols);

/* Creates a new matrix with same values as src. */
int mat_dup(matrix **dst, const matrix *src);

/* Copies src's values into dst.
 * Possible errors:
 *  - LAMAT_INCOMPATIBLE_DIM */
int mat_cpy(matrix *dst, const matrix *src);

/* Frees resources allocated for m */
int mat_del(matrix *m);

/* Writes to *rows and *cols the dimensions of m.
 * If any of the int pointers are NULL, it is left untouched. */
int mat_dim(const matrix *m, int *rows, int *cols);


/* Writes the element with corresponding position into out.
 * Possible errors:
 *  - LAMAT_OOB */
int mat_read(const matrix *m, int row, int col, LINALG_REAL *out);

/* Alternative version of mat_read for easier handling.
 * Returns the element with corresponding position in the matrix.
 * If err is not NULL, it is set to the corresponding error, or 0 if none.
 */
LINALG_REAL mat_get(const matrix *m, int row, int col, int *err);

/* Equivalent to mat_set.
 * Writes r into the matrix's corresponding position.
 * Possible errors:
 *  - LAMAT_OOB */
int mat_write(matrix *m, int row, int col, LINALG_REAL r);

/* Equivalent to mat_write.
 * Writes r into the matrix's corresponding position.
 * Possible errors:
 *  - LAMAT_OOB */
int mat_set(matrix *m, int row, int col, LINALG_REAL r);


/* Writes the result of a + b into out.
 * Possible errors:
 *  - LAMAT_INCOMPATIBLE_DIM */
int mat_add(const matrix *a, const matrix *b, matrix *out);

/* Writes the result of a + b into a.
 * Possible errors:
 *  - LAMAT_INCOMPATIBLE_DIM */
int mat_add_(matrix *a, const matrix *b);

/* Writes the result of a - b into out.
 * Possible errors:
 *  - LAMAT_INCOMPATIBLE_DIM */
int mat_sub(const matrix *a, const matrix *b, matrix *out);

/* Writes the result of a - b into a.
 * Possible errors:
 *  - LAMAT_INCOMPATIBLE_DIM */
int mat_sub_(matrix *a, const matrix *b);

/* Writes the result of a * b into out.
 * Possible errors:
 *  - LAMAT_INCOMPATIBLE_DIM */
int mat_mul(const matrix *a, const matrix *b, matrix *out);

/* Writes the result of s * m into out. */
int mat_smul(const matrix *m, LINALG_REAL s, matrix *out);

/* Writes the result of s * m into m. */
int mat_smul_(matrix *m, LINALG_REAL s);


/* Transposes m. */
int mat_transpose_(matrix *m);

#endif