#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include "../src/matrix.h"
#include "../../catch/catch.hpp"
#include "../../utils/settings.h"

//TODO
//Do tests with actual numbers given instead of just using for loops and numbers

TEST_CASE("Creating Matrices of different size", "[construction]")
{

    int a_row_test = 4;
    int a_col_test = 4;
    int a_total_elements = a_row_test * a_col_test;

    int b_row_test = 3;
    int b_col_test = 4;
    int b_total_elements = b_row_test * b_col_test;

    int c_row_test = 15;
    int c_col_test = 11;
    int c_total_elements = c_row_test * c_col_test;

    int d_row_test = 13;
    int d_col_test = 19;
    int d_total_elements = d_row_test * d_col_test;
    std::vector<real> d_test_values;
    for (int i = 0; i < d_total_elements; i++)
    {
        d_test_values.push_back(rand() % 100); //make sure to change this so we can also test for floats, or for any real
    }

    Matrix *a = new Matrix(a_row_test, a_col_test);
    int a_col = a->get_num_cols();
    int a_row = a->get_num_rows();

    Matrix *b = new Matrix(b_row_test, b_col_test);
    int b_col = b->get_num_cols();
    int b_row = b->get_num_rows();

    Matrix c = Matrix::createMatrixArange(c_row_test, c_col_test);
    int c_col = c.get_num_cols();
    int c_row = c.get_num_rows();

    Matrix *d = new Matrix(d_row_test, d_col_test, &d_test_values[0]);
    int d_col = d->get_num_cols();
    int d_row = d->get_num_rows();

    SECTION("Construxtion of Matrix given rows and columns")
    {

        REQUIRE(a_col == a_col_test);
        REQUIRE(a_row == a_row_test);

        REQUIRE(b_col == b_col_test);
        REQUIRE(b_row == b_row_test);

        const real *a_values = a->getValues();
        for (int i = 0; i < a_total_elements; i++)
        {
            REQUIRE(a_values[i] == 0);
        }

        const real *b_values = b->getValues();
        for (int i = 0; i < b_total_elements; i++)
        {
            REQUIRE(b_values[i] == 0);
        }
    }
    SECTION("Construction of Matrix via arange")
    {
        REQUIRE(c_col == c_col_test);
        REQUIRE(c_row == c_row_test);

        const real *c_values = c.getValues();
        for (int i = 0; i < c_total_elements; i++)
        {
            REQUIRE(c_values[i] == i);
        }
    }

    SECTION("Construction of Matrix via a given array")
    {
        REQUIRE(d_col == d_col_test);
        REQUIRE(d_row == d_row_test);

        const real *d_values = d->getValues();
        for (int i = 0; i < d_total_elements; i++)
        {
            REQUIRE(d_values[i] == d_test_values[i]);
        }
    }
}

TEST_CASE("Matrices can be operated with", "[Operations]")
{

    int a_row = 4;
    int a_col = 4;
    int a_total_elements = a_row * a_col;

    int b_row = 4;
    int b_col = 4;
    int b_total_elements = b_row * b_col;

    int c_row = 100;
    int c_col = 10;
    int c_total_elements = c_row * c_col;

    int d_row = 3;
    int d_col = 4;
    int d_total_elements = d_row * d_col;

    std::vector<real> a_test_values;
    std::vector<real> b_test_values;
    for (int i = 0; i < a_total_elements; i++)
    {
        a_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        b_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
    }

    Matrix *a = new Matrix(a_row, a_col, &a_test_values[0]);

    SECTION("Matrix Addition")
    {
        //test with both no error thrown and error thrown
    }

    SECTION("Matrix Subtraction")
    {
        //test with both no error thrown and error thrown
    }

    SECTION("Matrix Elementwise Multiplication")
    {
        //test with both no error thrown and error thrown
    }

    SECTION("Matrix Division")
    {
        //test with both no error thrown and error thrown
    }

    SECTION("Matrix Multiplication")
    {
        //test with both no error thrown and error thrown
    }

    SECTION("Matrix Inverse")
    {
        //test with both no error thrown and error thrown
    }
}