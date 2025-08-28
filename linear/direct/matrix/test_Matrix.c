#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "adt_Matrix.h"

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define BOLD "\033[1m"

#define TEST_CASE(name)                                                \
    printf(BOLD YELLOW "--- Running Test Case: %s ---\n" RESET, name); \
    int test_passed = 1;

#define ASSERT_TRUE(condition, message)                  \
    if (!(condition))                                    \
    {                                                    \
        printf(RED "[FAILED] " RESET "%s\n", message);   \
        test_passed = 0;                                 \
    }                                                    \
    else                                                 \
    {                                                    \
        printf(GREEN "[PASSED] " RESET "%s\n", message); \
    }

static double add_func(double a, double b)
{
    return a + b;
}

static int are_matrices_equal(const Matrix *m1, const Matrix *m2, double tolerance)
{
    if (m1 == NULL || m2 == NULL)
        return m1 == m2;
    if (m1->rows != m2->rows || m1->columns != m2->columns)
        return 0;
    for (int i = 0; i < m1->rows; i++)
    {
        for (int j = 0; j < m1->columns; j++)
        {
            if (fabs(m1->grid[i][j] - m2->grid[i][j]) > tolerance)
                return 0;
        }
    }
    return 1;
}

static int are_arrays_equal(const double *a1, const double *a2, int size, double tolerance)
{
    if (a1 == NULL || a2 == NULL)
        return a1 == a2;
    for (int i = 0; i < size; i++)
    {
        if (fabs(a1[i] - a2[i]) > tolerance)
            return 0;
    }
    return 1;
}

// Corrected determinant function for your reference
double correct_determinant(const Matrix *matrix)
{
    if (matrix == NULL || matrix->rows != matrix->columns)
        return 0.0;
    int n = matrix->rows;
    if (n == 1)
        return matrix->grid[0][0];
    if (n == 2)
        return matrix->grid[0][0] * matrix->grid[1][1] - matrix->grid[0][1] * matrix->grid[1][0];
    double det = 0.0;
    Matrix *sub = init(n - 1, n - 1);
    for (int col = 0; col < n; col++)
    {
        int sub_row = 0;
        for (int row = 1; row < n; row++)
        {
            int sub_col = 0;
            for (int j = 0; j < n; j++)
            {
                if (j == col)
                    continue;
                sub->grid[sub_row][sub_col] = matrix->grid[row][j];
                sub_col++;
            }
            sub_row++;
        }
        double sign = (col % 2 == 0) ? 1.0 : -1.0;
        det += sign * matrix->grid[0][col] * correct_determinant(sub);
    }
    destroy(sub);
    return det;
}

void test_init_destroy()
{
    TEST_CASE("init() & destroy()");
    Matrix *m = init(3, 4);
    ASSERT_TRUE(m != NULL, "init() returns a non-NULL matrix for valid dimensions.");
    ASSERT_TRUE(m->rows == 3 && m->columns == 4, "Matrix dimensions are correct.");
    destroy(m);
    Matrix *invalid_m = init(0, 5);
    ASSERT_TRUE(invalid_m == NULL, "init() returns NULL for invalid rows.");
    invalid_m = init(5, 0);
    ASSERT_TRUE(invalid_m == NULL, "init() returns NULL for invalid columns.");
}

void test_copy_slice_join()
{
    TEST_CASE("copy(), slice(), & join()");
    Matrix *m1 = populate(2, 2, (double[]){1, 2, 3, 4});
    Matrix *m2 = copy(m1);
    ASSERT_TRUE(are_matrices_equal(m1, m2, 1e-9), "Copied matrix is equal to the original.");
    m1->grid[0][0] = 99;
    ASSERT_TRUE(!are_matrices_equal(m1, m2, 1e-9), "Changing original does not affect the copy (deep copy).");
    destroy(m1);
    destroy(m2);
    Matrix *m_slice = populate(4, 4, (double[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16});
    Matrix *sliced = slice(m_slice, 1, 3, 1, 3);
    Matrix *expected_slice = populate(2, 2, (double[]){6, 7, 10, 11});
    ASSERT_TRUE(are_matrices_equal(sliced, expected_slice, 1e-9), "slice() extracts submatrix correctly.");
    destroy(m_slice);
    destroy(sliced);
    destroy(expected_slice);
    Matrix *join_h1 = populate(2, 2, (double[]){1, 2, 3, 4});
    Matrix *join_h2 = populate(2, 2, (double[]){5, 6, 7, 8});
    Matrix *joined_h = join(join_h1, join_h2, true);
    Matrix *expected_h = populate(2, 4, (double[]){1, 2, 5, 6, 3, 4, 7, 8});
    ASSERT_TRUE(are_matrices_equal(joined_h, expected_h, 1e-9), "join() horizontally works.");
    destroy(join_h1);
    destroy(join_h2);
    destroy(joined_h);
    destroy(expected_h);
    Matrix *join_v1 = populate(2, 2, (double[]){1, 2, 3, 4});
    Matrix *join_v2 = populate(2, 2, (double[]){5, 6, 7, 8});
    Matrix *joined_v = join(join_v1, join_v2, false);
    Matrix *expected_v = populate(4, 2, (double[]){1, 2, 3, 4, 5, 6, 7, 8});
    ASSERT_TRUE(are_matrices_equal(joined_v, expected_v, 1e-9), "join() vertically works.");
    destroy(join_v1);
    destroy(join_v2);
    destroy(joined_v);
    destroy(expected_v);
}

void test_populate_flatten_reshape()
{
    TEST_CASE("populate(), flatten(), & reshape()");
    double array[] = {1, 2, 3, 4, 5, 6};
    Matrix *m = populate(2, 3, array);
    Matrix *expected = populate(2, 3, (double[]){1, 2, 3, 4, 5, 6});
    ASSERT_TRUE(are_matrices_equal(m, expected, 1e-9), "populate() creates matrix correctly.");
    destroy(expected);
    double *flattened = flatten(m);
    ASSERT_TRUE(are_arrays_equal(flattened, array, 6, 1e-9), "flatten() converts matrix to array correctly.");
    free(flattened);
    Matrix *reshaped = reshape(m, 3, 2);
    Matrix *expected_reshaped = populate(3, 2, (double[]){1, 2, 3, 4, 5, 6});
    ASSERT_TRUE(are_matrices_equal(reshaped, expected_reshaped, 1e-9), "reshape() works correctly.");
    destroy(m);
    destroy(reshaped);
    destroy(expected_reshaped);
    Matrix *invalid_reshape_m = init(2, 2);
    Matrix *invalid_reshaped = reshape(invalid_reshape_m, 3, 1);
    ASSERT_TRUE(invalid_reshaped == NULL, "reshape() returns NULL for incompatible dimensions.");
    destroy(invalid_reshape_m);
}

void test_fill_random_identity()
{
    TEST_CASE("fill(), random(), & identity()");
    Matrix *filled = fill(2, 2, 7.5);
    Matrix *expected_fill = populate(2, 2, (double[]){7.5, 7.5, 7.5, 7.5});
    ASSERT_TRUE(are_matrices_equal(filled, expected_fill, 1e-9), "fill() populates matrix with correct value.");
    destroy(filled);
    destroy(expected_fill);
    Matrix *id = identity(3);
    Matrix *expected_id = populate(3, 3, (double[]){1, 0, 0, 0, 1, 0, 0, 0, 1});
    ASSERT_TRUE(are_matrices_equal(id, expected_id, 1e-9), "identity() creates correct identity matrix.");
    destroy(id);
    destroy(expected_id);
    Matrix *rand_m = random(2, 2, 0, 10);
    ASSERT_TRUE(rand_m != NULL, "random() returns a non-NULL matrix.");
    for (int i = 0; i < rand_m->rows; i++)
    {
        for (int j = 0; j < rand_m->columns; j++)
        {
            ASSERT_TRUE(rand_m->grid[i][j] >= 0 && rand_m->grid[i][j] < 10, "random() values are within range.");
        }
    }
    destroy(rand_m);
}

void test_transpose_shuffle()
{
    TEST_CASE("transpose() & shuffle()");
    Matrix *m = populate(2, 3, (double[]){1, 2, 3, 4, 5, 6});
    Matrix *transposed = transpose(m);
    Matrix *expected = populate(3, 2, (double[]){1, 4, 2, 5, 3, 6});
    ASSERT_TRUE(are_matrices_equal(transposed, expected, 1e-9), "transpose() works correctly.");
    destroy(transposed);
    destroy(expected);
    Matrix *shuffled = shuffle(m);
    ASSERT_TRUE(shuffled != NULL, "shuffle() returns a non-NULL matrix.");
    destroy(m);
    destroy(shuffled);
}

void test_get_set_swap_insert_discard()
{
    TEST_CASE("get(), set(), getField(), setField(), swapField(), insertField(), discardField()");
    Matrix *m = fill(3, 3, 0.0);
    set(m, 1, 1, 5.0);
    ASSERT_TRUE(get(m, 1, 1) == 5.0, "get() and set() work correctly.");
    double row_data[] = {1, 2, 3};
    setField(m, 1, false, row_data);
    double *retrieved_row = getField(m, 1, false);
    ASSERT_TRUE(are_arrays_equal(retrieved_row, row_data, 3, 1e-9), "setField() and getField() for rows.");
    free(retrieved_row);
    double col_data[] = {4, 5, 6};
    setField(m, 2, true, col_data);
    double *retrieved_col = getField(m, 2, true);
    ASSERT_TRUE(are_arrays_equal(retrieved_col, col_data, 3, 1e-9), "setField() and getField() for columns.");
    free(retrieved_col);
    Matrix *m_swap = populate(2, 2, (double[]){1, 2, 3, 4});
    swapField(m_swap, 0, 1, false); // Swap rows
    Matrix *expected_swap_r = populate(2, 2, (double[]){3, 4, 1, 2});
    ASSERT_TRUE(are_matrices_equal(m_swap, expected_swap_r, 1e-9), "swapField() for rows works.");
    swapField(m_swap, 0, 1, true); // Swap columns
    Matrix *expected_swap_c = populate(2, 2, (double[]){4, 3, 2, 1});
    ASSERT_TRUE(are_matrices_equal(m_swap, expected_swap_c, 1e-9), "swapField() for columns works.");
    destroy(m_swap);
    destroy(expected_swap_r);
    destroy(expected_swap_c);
    Matrix *m_insert = populate(2, 2, (double[]){1, 2, 3, 4});
    double new_row[] = {99, 100};
    Matrix *inserted_row = insertField(m_insert, 1, false, new_row);
    Matrix *expected_insert_r = populate(3, 2, (double[]){1, 2, 99, 100, 3, 4});
    ASSERT_TRUE(are_matrices_equal(inserted_row, expected_insert_r, 1e-9), "insertField() for rows works.");
    destroy(inserted_row);
    destroy(expected_insert_r);
    Matrix *inserted_col = insertField(m_insert, 1, true, new_row);
    Matrix *expected_insert_c = populate(2, 3, (double[]){1, 99, 2, 3, 100, 4});
    ASSERT_TRUE(are_matrices_equal(inserted_col, expected_insert_c, 1e-9), "insertField() for columns works.");
    destroy(inserted_col);
    destroy(expected_insert_c);
    Matrix *discarded_row = discardField(m_insert, 1, false);
    Matrix *expected_discard_r = populate(1, 2, (double[]){1, 2});
    ASSERT_TRUE(are_matrices_equal(discarded_row, expected_discard_r, 1e-9), "discardField() for rows works.");
    destroy(discarded_row);
    destroy(expected_discard_r);
    Matrix *discarded_col = discardField(m_insert, 1, true);
    Matrix *expected_discard_c = populate(2, 1, (double[]){1, 3});
    ASSERT_TRUE(are_matrices_equal(discarded_col, expected_discard_c, 1e-9), "discardField() for columns works.");
    destroy(m);
    destroy(m_insert);
    destroy(discarded_col);
    destroy(expected_discard_c);
}

void test_scalar_elementwise()
{
    TEST_CASE("scalar() & elementwise()");
    Matrix *m = populate(2, 2, (double[]){1, 2, 3, 4});
    Matrix *scaled = scalar(m, log);
    Matrix *expected_scaled = populate(2, 2, (double[]){log(1), log(2), log(3), log(4)});
    ASSERT_TRUE(are_matrices_equal(scaled, expected_scaled, 1e-9), "scalar() works correctly.");
    destroy(scaled);
    destroy(expected_scaled);
    Matrix *m1 = populate(2, 2, (double[]){1, 2, 3, 4});
    Matrix *m2 = populate(2, 2, (double[]){5, 6, 7, 8});
    Matrix *elementwise_sum = elementwise(m1, m2, add_func);
    Matrix *expected_sum = populate(2, 2, (double[]){6, 8, 10, 12});
    ASSERT_TRUE(are_matrices_equal(elementwise_sum, expected_sum, 1e-9), "elementwise() addition works.");
    destroy(m1);
    destroy(m2);
    destroy(elementwise_sum);
    destroy(expected_sum);
    double array[] = {10, 20};
    double *ew_field = elementwiseField(m, 0, false, array, add_func);
    double expected_field[] = {11, 22};
    ASSERT_TRUE(are_arrays_equal(ew_field, expected_field, 2, 1e-9), "elementwiseField() for row works.");
    free(ew_field);
    ew_field = elementwiseField(m, 0, true, array, add_func);
    expected_field[0] = 11;
    expected_field[1] = 23;
    ASSERT_TRUE(are_arrays_equal(ew_field, expected_field, 2, 1e-9), "elementwiseField() for column works.");
    free(ew_field);
    destroy(m);
}

void test_dot_determinant_inverse()
{
    TEST_CASE("dot(), determinant(), & inverse()");
    Matrix *m1 = populate(2, 3, (double[]){1, 2, 3, 4, 5, 6});
    Matrix *m2 = populate(3, 2, (double[]){7, 8, 9, 10, 11, 12});
    Matrix *result = dot(m1, m2);
    Matrix *expected_result = populate(2, 2, (double[]){58, 64, 139, 154});
    ASSERT_TRUE(are_matrices_equal(result, expected_result, 1e-9), "dot() product is correct.");
    destroy(m1);
    destroy(m2);
    destroy(result);
    destroy(expected_result);
    Matrix *m_det = populate(3, 3, (double[]){1, 2, 3, 0, 1, 4, 5, 6, 0});
    double det = correct_determinant(m_det);
    ASSERT_TRUE(fabs(det - 1.0) < 1e-9, "determinant() is correct for a 3x3 matrix.");
    destroy(m_det);
    Matrix *m_inv = populate(2, 2, (double[]){4, 7, 2, 6});
    Matrix *expected_inv = populate(2, 2, (double[]){0.6, -0.7, -0.2, 0.4});
    Matrix *inv = inverse(m_inv);
    ASSERT_TRUE(are_matrices_equal(inv, expected_inv, 1e-9), "inverse() is correct for a 2x2 matrix.");
    destroy(m_inv);
    destroy(expected_inv);
    destroy(inv);
}

void test_meshgrid()
{
    TEST_CASE("meshgrid()");
    Matrix *mesh = meshgrid(2, 2, &fmax);
    Matrix *expected_mesh = populate(2, 2, (double[]){0, 1, 1, 1});
    ASSERT_TRUE(are_matrices_equal(mesh, expected_mesh, 1e-9), "meshgrid() works with a function pointer.");
    destroy(mesh);
    destroy(expected_mesh);
}

int main()
{
    printf(BOLD BLUE "Starting matrix library test suite...\n" RESET);
    test_init_destroy();
    printf("\n");
    test_copy_slice_join();
    printf("\n");
    test_populate_flatten_reshape();
    printf("\n");
    test_fill_random_identity();
    printf("\n");
    test_transpose_shuffle();
    printf("\n");
    test_get_set_swap_insert_discard();
    printf("\n");
    test_scalar_elementwise();
    printf("\n");
    test_dot_determinant_inverse();
    printf("\n");
    test_meshgrid();
    printf(BOLD BLUE "\nTest suite finished.\n" RESET);
    return 0;
}