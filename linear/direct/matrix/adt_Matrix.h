#ifndef MATRIX_H
#define MATRIX_H

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @struct Matrix
 * @brief Represents a two-dimensional matrix of double-precision floating-point numbers.
 * @var grid A double pointer to the 2D array holding the matrix elements.
 * @var rows The number of rows in the matrix.
 * @var columns The number of columns in the matrix.
 */
typedef struct
{
    double **grid;
    int rows;
    int columns;
} Matrix;

/**
 * @brief Initializes and allocates memory for a new matrix.
 * @param rows The number of rows for the new matrix. Must be greater than 0.
 * @param columns The number of columns for the new matrix. Must be greater than 0.
 * @return A pointer to the newly created Matrix, or NULL if allocation fails or dimensions are invalid.
 */
Matrix *init(const int rows, const int columns)
{
    if (rows <= 0 || columns <= 0)
        return NULL;

    Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
    if (matrix == NULL)
        return NULL;

    matrix->grid = (double **)malloc(rows * sizeof(double *));
    if (matrix->grid == NULL)
    {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < rows; i++)
    {
        matrix->grid[i] = (double *)calloc(columns, sizeof(double));
        if (matrix->grid[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(matrix->grid[j]);
            free(matrix->grid);
            free(matrix);
            return NULL;
        }
    }

    matrix->rows = rows;
    matrix->columns = columns;
    return matrix;
}

/**
 * @brief Frees the memory allocated for a matrix.
 * @param matrix The matrix to be destroyed. If NULL, the function does nothing.
 */
void destroy(Matrix *matrix)
{
    if (matrix == NULL)
        return;
    for (int i = 0; i < matrix->rows; i++)
        free(matrix->grid[i]);
    free(matrix->grid);
    free(matrix);
}

/**
 * @brief Creates a deep copy of an existing matrix.
 * @param matrix The source matrix to copy.
 * @return A pointer to the new, copied matrix, or NULL if the source is invalid or allocation fails.
 */
Matrix *copy(const Matrix *matrix)
{
    if (matrix == NULL)
        return NULL;

    Matrix *copied = init(matrix->rows, matrix->columns);
    if (copied == NULL)
        return NULL;

    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
            copied->grid[i][j] = matrix->grid[i][j];

    return copied;
}

/**
 * @brief Extracts a submatrix from a larger matrix.
 * @param matrix The source matrix to slice.
 * @param fromRow The starting row index (inclusive).
 * @param toRow The ending row index (exclusive).
 * @param fromColumn The starting column index (inclusive).
 * @param toColumn The ending column index (exclusive).
 * @return A pointer to the new submatrix, or NULL if parameters are invalid.
 */
Matrix *slice(const Matrix *matrix, const int fromRow, const int toRow, const int fromColumn, const int toColumn)
{
    if (matrix == NULL || fromRow < 0 || toRow > matrix->rows || fromColumn < 0 || toColumn > matrix->columns || fromRow >= toRow || fromColumn >= toColumn)
        return NULL;

    Matrix *sliced = init(toRow - fromRow, toColumn - fromColumn);
    if (sliced == NULL)
        return NULL;

    for (int i = fromRow; i < toRow; i++)
        for (int j = fromColumn; j < toColumn; j++)
            sliced->grid[i - fromRow][j - fromColumn] = matrix->grid[i][j];

    return sliced;
}

/**
 * @brief Joins two matrices either horizontally (column-wise) or vertically (row-wise).
 * @param matrix1 The first matrix.
 * @param matrix2 The second matrix.
 * @param axis A boolean flag: true for horizontal join, false for vertical join.
 * @return A pointer to the new joined matrix, or NULL if matrices are incompatible or allocation fails.
 */
Matrix *join(const Matrix *matrix1, const Matrix *matrix2, const bool axis)
{
    if (matrix1 == NULL || matrix2 == NULL)
        return NULL;

    if (axis) // join horizontally
    {
        if (matrix1->rows != matrix2->rows)
            return NULL;

        Matrix *joined = init(matrix1->rows, matrix1->columns + matrix2->columns);
        if (joined == NULL)
            return NULL;

        for (int i = 0; i < matrix1->rows; i++)
        {
            for (int j = 0; j < matrix1->columns; j++)
                joined->grid[i][j] = matrix1->grid[i][j];
            for (int j = 0; j < matrix2->columns; j++)
                joined->grid[i][matrix1->columns + j] = matrix2->grid[i][j];
        }

        return joined;
    }
    else // join vertically
    {
        if (matrix1->columns != matrix2->columns)
            return NULL;

        Matrix *joined = init(matrix1->rows + matrix2->rows, matrix1->columns);
        if (joined == NULL)
            return NULL;

        for (int j = 0; j < matrix1->columns; j++)
        {
            for (int i = 0; i < matrix1->rows; i++)
                joined->grid[i][j] = matrix1->grid[i][j];
            for (int i = 0; i < matrix2->rows; i++)
                joined->grid[matrix1->rows + i][j] = matrix2->grid[i][j];
        }

        return joined;
    }
}

/**
 * @brief Populates a new matrix with elements from a 1D array.
 * @param rows The number of rows for the new matrix.
 * @param columns The number of columns for the new matrix.
 * @param array The 1D array of values to populate the matrix. The size of the array should be rows * columns.
 * @return A pointer to the newly created and populated matrix, or NULL on failure.
 */
Matrix *populate(const int rows, const int columns, const double *array)
{
    if (array == NULL)
        return NULL;

    Matrix *matrix = init(rows, columns);
    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            matrix->grid[i][j] = array[i * columns + j];

    return matrix;
}

/**
 * @brief Converts a matrix into a 1D dynamically allocated array.
 * @param matrix The matrix to flatten.
 * @return A pointer to the new 1D array, or NULL if the matrix is invalid. The caller is responsible for freeing this memory.
 */
double *flatten(const Matrix *matrix)
{
    if (matrix == NULL)
        return NULL;
    double *array = (double *)malloc(matrix->rows * matrix->columns * sizeof(double));
    if (array == NULL)
        return NULL;
    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
            array[i * matrix->columns + j] = matrix->grid[i][j];
    return array;
}

/**
 * @brief Reshapes an existing matrix into new dimensions without changing its elements.
 * @param matrix The matrix to reshape.
 * @param newRows The new number of rows.
 * @param newColumns The new number of columns.
 * @return A pointer to the newly reshaped matrix, or NULL if the new dimensions are incompatible with the original number of elements.
 */
Matrix *reshape(const Matrix *matrix, const int newRows, const int newColumns)
{
    if (matrix == NULL || matrix->rows * matrix->columns != newRows * newColumns)
        return NULL;

    Matrix *reshaped = init(newRows, newColumns);
    if (reshaped == NULL)
        return NULL;

    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
        {
            int index = i * matrix->columns + j;
            int newRow = index / newColumns;
            int newCol = index % newColumns;
            reshaped->grid[newRow][newCol] = matrix->grid[i][j];
        }

    return reshaped;
}

/**
 * @brief Prints the contents and dimensions of a matrix to the console.
 * @param matrix The matrix to print.
 */
void traverse(const Matrix *matrix)
{
    if (matrix == NULL)
    {
        printf("Matrix is empty\n");
        return;
    }

    for (int i = 0; i < matrix->rows; i++)
    {
        printf("[ ");
        for (int j = 0; j < matrix->columns; j++)
            printf("%6.2f ", matrix->grid[i][j]);
        printf("]\n");
    }
    printf("Dim: %dx%d\n", matrix->rows, matrix->columns);
}

/**
 * @brief Creates a new matrix and fills all its elements with a single value.
 * @param rows The number of rows for the new matrix.
 * @param columns The number of columns for the new matrix.
 * @param value The value to fill the matrix with.
 * @return A pointer to the newly created and filled matrix, or NULL on failure.
 */
Matrix *fill(const int rows, const int columns, const double value)
{
    Matrix *matrix = init(rows, columns);
    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            matrix->grid[i][j] = value;

    return matrix;
}

/**
 * @brief Seeds the random number generator. It is called once automatically by functions that require random numbers.
 */
void __seeding__()
{
    static int seeded = 0;
    if (!seeded)
    {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
}

/**
 * @brief Creates a new matrix with elements filled with random values within a specified range.
 * @param rows The number of rows for the new matrix.
 * @param columns The number of columns for the new matrix.
 * @param min The minimum value for the random numbers (inclusive).
 * @param max The maximum value for the random numbers (exclusive).
 * @return A pointer to the new matrix with random values, or NULL if parameters are invalid or allocation fails.
 */
Matrix *random(const int rows, const int columns, const double min, const double max)
{
    if (min >= max)
        return NULL;

    __seeding__();

    Matrix *matrix = init(rows, columns);
    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            matrix->grid[i][j] = ((double)rand() / RAND_MAX) * (max - min) + min;

    return matrix;
}

/**
 * @brief Creates a new identity matrix of a given dimension.
 * @param dimensions The number of rows and columns for the square identity matrix.
 * @return A pointer to the new identity matrix, or NULL on failure.
 */
Matrix *identity(const int dimensions)
{
    Matrix *matrix = init(dimensions, dimensions);
    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < dimensions; i++)
        matrix->grid[i][i] = 1.0;

    return matrix;
}

/**
 * @brief Creates a new matrix by applying a function to each row and column index.
 * @param rows The number of rows.
 * @param columns The number of columns.
 * @param func A function pointer that takes a row and column index and returns a double value.
 * @return A pointer to the newly created matrix, or NULL if the function pointer is NULL or allocation fails.
 */
Matrix *meshgrid(const int rows, const int columns, double (*func)(double, double))
{
    if (func == NULL)
        return NULL;

    Matrix *matrix = init(rows, columns);
    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            matrix->grid[i][j] = func(i, j);

    return matrix;
}

/**
 * @brief Retrieves the value of a specific element at a given row and column.
 * @param matrix The matrix to get the value from.
 * @param row The row index.
 * @param column The column index.
 * @return The value of the element, or -1 if indices are invalid or the matrix is NULL.
 */
double get(const Matrix *matrix, const int row, const int column)
{
    if (matrix == NULL || row < 0 || row >= matrix->rows || column < 0 || column >= matrix->columns)
        return -1;

    return matrix->grid[row][column];
}

/**
 * @brief Retrieves a specific row or column as a 1D dynamically allocated array.
 * @param matrix The matrix to get the field from.
 * @param index The index of the row or column to retrieve.
 * @param axis A boolean flag: true for a column, false for a row.
 * @return A pointer to the new 1D array, or NULL if parameters are invalid. The caller is responsible for freeing this memory.
 */
double *getField(const Matrix *matrix, const int index, const bool axis)
{
    if (matrix == NULL)
        return NULL;

    double *array;
    if (axis) // get column
    {
        if (index < 0 || index >= matrix->columns)
            return NULL;

        array = (double *)malloc(matrix->rows * sizeof(double));
        if (array == NULL)
            return NULL;

        for (int i = 0; i < matrix->rows; i++)
            array[i] = matrix->grid[i][index];
    }
    else // get row
    {
        if (index < 0 || index >= matrix->rows)
            return NULL;

        array = (double *)malloc(matrix->columns * sizeof(double));
        if (array == NULL)
            return NULL;

        for (int j = 0; j < matrix->columns; j++)
            array[j] = matrix->grid[index][j];
    }
    return array;
}

/**
 * @brief Sets the value of a specific element at a given row and column.
 * @param matrix The matrix to modify.
 * @param row The row index.
 * @param column The column index.
 * @param value The new value to set.
 */
void set(Matrix *matrix, const int row, const int column, const double value)
{
    if (matrix == NULL || row < 0 || row >= matrix->rows || column < 0 || column >= matrix->columns)
        return;

    matrix->grid[row][column] = value;
}

/**
 * @brief Sets the values of an entire row or column from a 1D array.
 * @param matrix The matrix to modify.
 * @param index The index of the row or column to set.
 * @param axis A boolean flag: true for a column, false for a row.
 * @param array The 1D array of values to set.
 */
void setField(Matrix *matrix, const int index, const bool axis, const double *array)
{
    if (matrix == NULL || array == NULL)
        return;

    if (axis) // set column
    {
        if (index < 0 || index >= matrix->columns)
            return;

        for (int i = 0; i < matrix->rows; i++)
            matrix->grid[i][index] = array[i];
    }
    else // set row
    {
        if (index < 0 || index >= matrix->rows)
            return;

        for (int j = 0; j < matrix->columns; j++)
            matrix->grid[index][j] = array[j];
    }
}

/**
 * @brief Swaps two entire rows or columns within a matrix.
 * @param matrix The matrix to modify.
 * @param index1 The index of the first row/column.
 * @param index2 The index of the second row/column.
 * @param axis A boolean flag: true to swap columns, false to swap rows.
 */
void swapField(Matrix *matrix, const int index1, const int index2, const bool axis)
{
    if (matrix == NULL)
        return;

    if (axis) // swap columns
    {
        if (index1 < 0 || index1 >= matrix->columns || index2 < 0 || index2 >= matrix->columns)
            return;

        for (int i = 0; i < matrix->rows; i++)
            __swap__(&matrix->grid[i][index1], &matrix->grid[i][index2]);
    }
    else // swap rows
    {
        if (index1 < 0 || index1 >= matrix->rows || index2 < 0 || index2 >= matrix->rows)
            return;

        for (int j = 0; j < matrix->columns; j++)
            __swap__(&matrix->grid[index1][j], &matrix->grid[index2][j]);
    }
}

/**
 * @brief Inserts a new row or column into a matrix at a specified index.
 * @param matrix The source matrix.
 * @param index The index at which to insert the new field.
 * @param axis A boolean flag: true to insert a column, false to insert a row.
 * @param array The 1D array of values for the new field.
 * @return A pointer to the new matrix with the inserted field, or NULL on failure. The original matrix is not modified.
 */
Matrix *insertField(const Matrix *matrix, int index, const bool axis, const double *array)
{
    if (matrix == NULL || array == NULL)
        return NULL;

    Matrix *expanded;
    if (axis) // insert column
    {
        index = index < 0 ? 0 : index;
        index = index > matrix->columns ? matrix->columns : index;

        expanded = init(matrix->rows, matrix->columns + 1);
        for (int i = 0; i < matrix->rows; i++)
        {
            for (int j = 0; j < index; j++)
                expanded->grid[i][j] = matrix->grid[i][j];

            expanded->grid[i][index] = array[i];

            for (int j = index; j < matrix->columns; j++)
                expanded->grid[i][j + 1] = matrix->grid[i][j];
        }
    }
    else // insert row
    {
        index = index < 0 ? 0 : index;
        index = index > matrix->rows ? matrix->rows : index;

        expanded = init(matrix->rows + 1, matrix->columns);
        for (int j = 0; j < matrix->columns; j++)
        {
            for (int i = 0; i < index; i++)
                expanded->grid[i][j] = matrix->grid[i][j];

            expanded->grid[index][j] = array[j];

            for (int i = index; i < matrix->rows; i++)
                expanded->grid[i + 1][j] = matrix->grid[i][j];
        }
    }
    return expanded;
}

/**
 * @brief Discards a row or column from a matrix at a specified index.
 * @param matrix The source matrix.
 * @param index The index of the row or column to discard.
 * @param axis A boolean flag: true to discard a column, false to discard a row.
 * @return A pointer to the new, smaller matrix, or NULL on failure. The original matrix is not modified.
 */
Matrix *discardField(const Matrix *matrix, int index, const bool axis)
{
    if (matrix == NULL)
        return NULL;

    Matrix *reduced;
    if (axis) // discard column
    {
        index = index < 0 ? 0 : index;
        index = index >= matrix->columns ? matrix->columns - 1 : index;

        reduced = init(matrix->rows, matrix->columns - 1);
        for (int i = 0; i < matrix->rows; i++)
        {
            for (int j = 0; j < index; j++)
                reduced->grid[i][j] = matrix->grid[i][j];

            for (int j = index + 1; j < matrix->columns; j++)
                reduced->grid[i][j - 1] = matrix->grid[i][j];
        }
    }
    else // discard row
    {
        index = index < 0 ? 0 : index;
        index = index >= matrix->rows ? matrix->rows - 1 : index;

        reduced = init(matrix->rows - 1, matrix->columns);
        for (int j = 0; j < matrix->columns; j++)
        {
            for (int i = 0; i < index; i++)
                reduced->grid[i][j] = matrix->grid[i][j];

            for (int i = index + 1; i < matrix->rows; i++)
                reduced->grid[i - 1][j] = matrix->grid[i][j];
        }
    }
    return reduced;
}

/**
 * @brief Transposes a matrix. The rows become columns and the columns become rows.
 * @param matrix The matrix to transpose.
 * @return A pointer to the new transposed matrix, or NULL on failure.
 */
Matrix *transpose(const Matrix *matrix)
{
    if (matrix == NULL)
        return NULL;

    Matrix *transposed = init(matrix->columns, matrix->rows);
    if (transposed == NULL)
        return NULL;

    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
            transposed->grid[j][i] = matrix->grid[i][j];

    return transposed;
}

/**
 * @brief Swaps the values of two double-precision floating-point numbers.
 * @param a Pointer to the first number.
 * @param b Pointer to the second number.
 */
void __swap__(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief Creates a new matrix with its elements randomly shuffled.
 * @param matrix The matrix to shuffle.
 * @return A pointer to the new shuffled matrix, or NULL on failure.
 */
Matrix *shuffle(const Matrix *matrix)
{
    if (matrix == NULL)
        return NULL;

    Matrix *shuffled = copy(matrix);
    if (matrix == NULL || matrix->rows * matrix->columns <= 1)
        return shuffled;

    __seeding__();

    for (int i = shuffled->rows * shuffled->columns - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int row1 = i / shuffled->columns;
        int col1 = i % shuffled->columns;
        int row2 = j / shuffled->columns;
        int col2 = j % shuffled->columns;
        __swap__(&shuffled->grid[row1][col1], &shuffled->grid[row2][col2]);
    }

    return shuffled;
}

/**
 * @brief Applies a unary function to every element of a matrix and returns a new matrix with the results.
 * @param matrix The source matrix.
 * @param func A function pointer that takes one double and returns a double.
 * @return A pointer to the new result matrix, or NULL on failure.
 */
Matrix *scalar(const Matrix *matrix, double (*func)(double))
{
    if (matrix == NULL || func == NULL)
        return NULL;

    Matrix *result = init(matrix->rows, matrix->columns);
    for (int i = 0; i < matrix->rows; i++)
        for (int j = 0; j < matrix->columns; j++)
            result->grid[i][j] = func(matrix->grid[i][j]);

    return result;
}

/**
 * @brief Applies a unary function to a specific row or column and returns the result as a new 1D array.
 * @param matrix The source matrix.
 * @param index The index of the row or column.
 * @param axis A boolean flag: true for a column, false for a row.
 * @param func A function pointer that takes one double and returns a double.
 * @return A pointer to the new result array, or NULL on failure. The caller is responsible for freeing this memory.
 */
double *scalarField(const Matrix *matrix, const int index, const bool axis, double (*func)(double))
{
    if (matrix == NULL || func == NULL)
        return NULL;

    double *array;
    if (axis) // apply to column
    {
        if (index < 0 || index >= matrix->columns)
            return NULL;

        array = (double *)malloc(matrix->rows * sizeof(double));
        if (array == NULL)
            return NULL;

        for (int i = 0; i < matrix->rows; i++)
            array[i] = func(matrix->grid[i][index]);
    }
    else // apply to row
    {
        if (index < 0 || index >= matrix->rows)
            return NULL;

        array = (double *)malloc(matrix->columns * sizeof(double));
        if (array == NULL)
            return NULL;

        for (int j = 0; j < matrix->columns; j++)
            array[j] = func(matrix->grid[index][j]);
    }
    return array;
}

/**
 * @brief Applies a binary function to corresponding elements of two matrices and returns a new matrix with the results.
 * @param matrix1 The first source matrix.
 * @param matrix2 The second source matrix.
 * @param func A function pointer that takes two doubles and returns a double.
 * @return A pointer to the new result matrix, or NULL if matrices are incompatible or allocation fails.
 */
Matrix *elementwise(const Matrix *matrix1, const Matrix *matrix2, double (*func)(double, double))
{
    if (matrix1 == NULL || matrix2 == NULL || func == NULL || matrix1->rows != matrix2->rows || matrix1->columns != matrix2->columns)
        return NULL;

    Matrix *result = init(matrix1->rows, matrix1->columns);
    for (int i = 0; i < result->rows; i++)
        for (int j = 0; j < result->columns; j++)
            result->grid[i][j] = func(matrix1->grid[i][j], matrix2->grid[i][j]);

    return result;
}

/**
 * @brief Applies a binary function to a row or column of a matrix and a 1D array, returning the result as a new 1D array.
 * @param matrix The source matrix.
 * @param index The index of the row or column.
 * @param axis A boolean flag: true for a column, false for a row.
 * @param array The 1D array of values.
 * @param func A function pointer that takes two doubles and returns a double.
 * @return A pointer to the new result array, or NULL on failure. The caller is responsible for freeing this memory.
 */
double *elementwiseField(const Matrix *matrix, const int index, const bool axis, const double *array, double (*func)(double, double))
{
    if (matrix == NULL || array == NULL || func == NULL)
        return NULL;

    double *result;
    if (axis) // apply to column
    {
        if (index < 0 || index >= matrix->columns)
            return NULL;

        result = (double *)malloc(matrix->rows * sizeof(double));
        if (result == NULL)
            return NULL;

        for (int i = 0; i < matrix->rows; i++)
            result[i] = func(matrix->grid[i][index], array[i]);
    }
    else // apply to row
    {
        if (index < 0 || index >= matrix->rows)
            return NULL;

        result = (double *)malloc(matrix->columns * sizeof(double));
        if (result == NULL)
            return NULL;

        for (int j = 0; j < matrix->columns; j++)
            result[j] = func(matrix->grid[index][j], array[j]);
    }
    return result;
}

/**
 * @brief Performs matrix multiplication (dot product) of two matrices.
 * @param matrix1 The first matrix.
 * @param matrix2 The second matrix.
 * @return A pointer to the resulting matrix, or NULL if the matrices are not compatible for multiplication.
 */
Matrix *dot(const Matrix *matrix1, const Matrix *matrix2)
{
    if (matrix1 == NULL || matrix2 == NULL || matrix1->columns != matrix2->rows)
        return NULL;

    Matrix *result = init(matrix1->rows, matrix2->columns);
    if (result == NULL)
        return NULL;

    for (int i = 0; i < result->rows; i++)
        for (int j = 0; j < result->columns; j++)
        {
            result->grid[i][j] = 0.0;
            for (int k = 0; k < matrix1->columns; k++)
                result->grid[i][j] += matrix1->grid[i][k] * matrix2->grid[k][j];
        }

    return result;
}

/**
 * @brief Calculates the determinant of a square matrix.
 * @param matrix The square matrix.
 * @return The determinant value, or 0.0 if the matrix is not square or a singular matrix.
 */
double determinant(const Matrix *matrix)
{
    if (matrix == NULL || matrix->rows != matrix->columns)
        return 0.0;

    int n = matrix->rows;
    Matrix *A = copy(matrix);
    if (A == NULL)
        return 0.0;

    double det = 1.0;
    int sign = 1;

    for (int i = 0; i < n; i++)
    {
        // Partial pivoting (find max in column)
        int pivot = i;
        for (int j = i + 1; j < n; j++)
        {
            if (fabs(A->grid[j][i]) > fabs(A->grid[pivot][i]))
                pivot = j;
        }

        // If pivot is zero -> determinant is zero
        if (fabs(A->grid[pivot][i]) < 1e-12)
        {
            destroy(A);
            return 0.0;
        }

        // Swap rows if needed
        if (pivot != i)
        {
            double *tmp = A->grid[i];
            A->grid[i] = A->grid[pivot];
            A->grid[pivot] = tmp;
            sign = -sign;
        }

        // Eliminate below pivot
        for (int j = i + 1; j < n; j++)
        {
            double factor = A->grid[j][i] / A->grid[i][i];
            for (int k = i; k < n; k++)
                A->grid[j][k] -= factor * A->grid[i][k];
        }

        det *= A->grid[i][i]; // product of U diagonal
    }

    destroy(A);
    return sign * det;
}

/**
 * @brief Calculates the inverse of a square matrix.
 * @param matrix The square matrix to invert.
 * @return A pointer to the inverse matrix, or NULL if the matrix is singular or not square.
 */
Matrix *inverse(const Matrix *matrix)
{
    if (matrix == NULL || matrix->rows != matrix->columns)
        return NULL;

    int n = matrix->rows;
    Matrix *A = copy(matrix);
    if (A == NULL)
        return NULL;

    int *pivot = (int *)malloc(n * sizeof(int));
    if (pivot == NULL)
    {
        destroy(A);
        return NULL;
    }
    for (int i = 0; i < n; i++)
        pivot[i] = i;

    // LU decomposition with partial pivoting
    for (int i = 0; i < n; i++)
    {
        // Find pivot row
        int maxRow = i;
        for (int k = i + 1; k < n; k++)
            if (fabs(A->grid[k][i]) > fabs(A->grid[maxRow][i]))
                maxRow = k;

        // Singular matrix check
        if (fabs(A->grid[maxRow][i]) < 1e-12)
        {
            free(pivot);
            destroy(A);
            return NULL;
        }

        // Row swap if necessary
        if (maxRow != i)
        {
            double *tmp = A->grid[i];
            A->grid[i] = A->grid[maxRow];
            A->grid[maxRow] = tmp;

            int tmpIdx = pivot[i];
            pivot[i] = pivot[maxRow];
            pivot[maxRow] = tmpIdx;
        }

        // Eliminate below pivot
        for (int k = i + 1; k < n; k++)
        {
            A->grid[k][i] /= A->grid[i][i];
            for (int j = i + 1; j < n; j++)
                A->grid[k][j] -= A->grid[k][i] * A->grid[i][j];
        }
    }

    // Allocate inverse matrix
    Matrix *inv = init(n, n);
    if (inv == NULL)
    {
        free(pivot);
        destroy(A);
        return NULL;
    }

    // Solve for each column of inverse
    for (int col = 0; col < n; col++)
    {
        double *y = (double *)calloc(n, sizeof(double));
        double *x = (double *)calloc(n, sizeof(double));
        if (y == NULL || x == NULL)
        {
            free(y);
            free(x);
            destroy(inv);
            free(pivot);
            destroy(A);
            return NULL;
        }

        // Forward substitution (Ly = P*e_col)
        for (int i = 0; i < n; i++)
        {
            y[i] = (pivot[i] == col) ? 1.0 : 0.0;
            for (int j = 0; j < i; j++)
                y[i] -= A->grid[i][j] * y[j];
        }

        // Back substitution (Ux = y)
        for (int i = n - 1; i >= 0; i--)
        {
            x[i] = y[i];
            for (int j = i + 1; j < n; j++)
                x[i] -= A->grid[i][j] * x[j];
            x[i] /= A->grid[i][i];
        }

        // Copy solution vector into inverse matrix
        for (int i = 0; i < n; i++)
            inv->grid[i][col] = x[i];

        free(y);
        free(x);
    }

    free(pivot);
    destroy(A);
    return inv;
}

#endif // MATRIX_H