#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

#include <utility>

#include <matrix.hpp>
// should add docker and add environment path to catch/test.h
#include <catch.hpp>
#include <settings.hpp>

using namespace numerics;
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
    std::vector<settings::real> d_test_values;
    for (int i = 0; i < d_total_elements; i++)
    {
        d_test_values.push_back(rand() % 100); //make sure to change this so we can also test for floats, or for any settings::real
    }

    Matrix *a = new Matrix(a_row_test, a_col_test);
    std::pair<int, int> a_shape = a->shape();
    int a_row = a_shape.first;
    int a_col = a_shape.second;

    Matrix *b = new Matrix(b_row_test, b_col_test);
    std::pair<int, int> b_shape = b->shape();
    int b_row = b_shape.first;
    int b_col = b_shape.second;

    Matrix *c = Matrix::CreateMatrixArange(c_row_test, c_col_test);
    std::pair<int, int> c_shape = c->shape();
    int c_row = c_shape.first;
    int c_col = c_shape.second;

    Matrix *d = new Matrix(d_row_test, d_col_test, &d_test_values[0]);
    std::pair<int, int> d_shape = d->shape();
    int d_row = d_shape.first;
    int d_col = d_shape.second;

    SECTION("Construction of Matrix given rows and columns")
    {

        REQUIRE(a_col == a_col_test);
        REQUIRE(a_row == a_row_test);

        REQUIRE(b_col == b_col_test);
        REQUIRE(b_row == b_row_test);

        const settings::real *a_values = a->GetValues();
        for (int i = 0; i < a_total_elements; i++)
        {
            REQUIRE(a_values[i] == 0);
        }

        const settings::real *b_values = b->GetValues();
        for (int i = 0; i < b_total_elements; i++)
        {
            REQUIRE(b_values[i] == 0);
        }
    }
    SECTION("Construction of Matrix via arange")
    {
        REQUIRE(c_col == c_col_test);
        REQUIRE(c_row == c_row_test);

        const settings::real *c_values = c->GetValues();
        for (int i = 0; i < c_total_elements; i++)
        {
            REQUIRE(c_values[i] == i);
        }
    }

    SECTION("Construction of Matrix via a given array")
    {
        REQUIRE(d_col == d_col_test);
        REQUIRE(d_row == d_row_test);

        const settings::real *d_values = d->GetValues();
        for (int i = 0; i < d_total_elements; i++)
        {
            REQUIRE(d_values[i] == d_test_values[i]);
        }
    }
    delete a;
    delete b;
    delete c;
    delete d;
}

TEST_CASE("Matrices can be applied with", "[Auxilary Operations]") {

    SECTION("Matrix Operation ()") {
        int a_row = 10;
        int a_col = 20;
        int a_total_elements = a_row * a_col;

        std::vector<settings::real> a_test_values;
        for(int i=0; i<a_total_elements; i++) {
            a_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        }

        Matrix a = Matrix(a_row, a_col, &a_test_values[0]);

        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                REQUIRE(a(i, j) == a_test_values.at(i * a_col + j));
            }
        }
    }

    SECTION("Matrix Norm") {
        int a_row = 10;
        int a_col = 20;
        int a_total_elements = a_row * a_col;

        std::vector<settings::real> a_test_values;
        for(int i=0; i<a_total_elements; i++) {
            a_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        }

        Matrix a = Matrix(a_row, a_col, &a_test_values[0]);
        settings::real a_norm = Matrix::Norm(a);


        settings::real a_norm_val = 0;
        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                a_norm_val += a(i, j) * a(i, j);
            }
        }
        REQUIRE(std::sqrt(a_norm_val) == a_norm);
    }


    SECTION("Matrix Normalise") {
        int a_row = 10;
        int a_col = 20;
        int a_total_elements = a_row * a_col;

        std::vector<settings::real> a_test_values;
        for(int i=0; i<a_total_elements; i++) {
            a_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        }

        Matrix a = Matrix(a_row, a_col, &a_test_values[0]);

        settings::real a_norm_val = 0;
        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                a_norm_val += a(i, j) * a(i, j);
            }
        }
        a_norm_val = std::sqrt(a_norm_val);
        a.Normalise();
        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                REQUIRE(a(i, j) == a_test_values.at(i * a_col + j)/a_norm_val);
            }
        }

        settings::real new_norm = 0;
        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                new_norm += a(i, j) * a(i, j);
            }
        }
        REQUIRE(new_norm == Approx(1).margin(1e-12));
    }


    SECTION("Matrix Dot") {

        int a_row = 10;
        int a_col = 20;
        int a_total_elements = a_row * a_col;

        int b_row = a_row;
        int b_col = a_col;
        int b_total_elements = b_row * b_col;

        int c_row = a_row + 1;
        int c_col = 20;
        int c_total_elements = c_row * c_col;

        std::vector<settings::real> a_test_values;
        std::vector<settings::real> b_test_values;
        std::vector<settings::real> c_test_values;

        for(int i=0; i<a_total_elements; i++) {
            a_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
            b_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        }

        for(int i=0; i<c_total_elements; i++) {
            c_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        }

        Matrix a = Matrix(a_row, a_col, &a_test_values[0]);
        Matrix b = Matrix(b_row, b_col, &b_test_values[0]);
        Matrix c = Matrix(c_row, c_col, &c_test_values[0]);

        settings::real ab_dot = 0;
        for(int i=0; i<a_total_elements; i++) {
            ab_dot += a_test_values.at(i) * b_test_values.at(i);
        }
        settings::real ab_dot_test = Matrix::Dot(a, b);

        REQUIRE(ab_dot == ab_dot_test);

        REQUIRE_THROWS_AS(Matrix::Dot(a, c), std::invalid_argument);
    }

    SECTION("Matrix Cross Product") {
        auto a = Matrix::CreateColumnVec(3, 7, 1);
        auto b = Matrix::CreateColumnVec(9, 1, 4);
        auto c = Matrix(4, 1);
        auto d = Matrix(3, 2);

        auto ab_vect = Matrix::VectorProduct(a, b);

        REQUIRE(ab_vect.shape().first == 3);
        REQUIRE(ab_vect.shape().second == 1);

        REQUIRE(ab_vect(0, 0) == 27);
        REQUIRE(ab_vect(1, 0) == -3);
        REQUIRE(ab_vect(2, 0) == -60);

        REQUIRE_THROWS_AS(Matrix::VectorProduct(a, d), std::invalid_argument);
        REQUIRE_THROWS_AS(Matrix::VectorProduct(a, c), std::invalid_argument);
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

    std::vector<settings::real> a_test_values;
    std::vector<settings::real> b_test_values;
    std::vector<settings::real> c_test_values;
    std::vector<settings::real> d_test_values;

    for (int i = 0; i < a_total_elements; i++)
    {
        a_test_values.push_back(rand() % 1000); //make sure to change this so we can also test for floats
        b_test_values.push_back((rand() % 1000) + 0.01); //make sure to change this so we can also test for floats //to avoid zero division
    }

    for (int i = 0; i < c_total_elements; i++)
    {
        c_test_values.push_back(rand() % 1000);
    }

    for (int i = 0; i < d_total_elements; i++)
    {
        d_test_values.push_back(rand() % 1000);
    }


    Matrix a = Matrix(a_row, a_col, &a_test_values[0]);
    Matrix b = Matrix(b_row, b_col, &b_test_values[0]);
    Matrix c = Matrix(c_row, c_col, &c_test_values[0]);
    Matrix d = Matrix(d_row, d_col, &d_test_values[0]);

    SECTION("Matrix Addition")
    {
        auto sum_ab = a + b;
        REQUIRE(sum_ab.shape().first == a_row);
        REQUIRE(sum_ab.shape().second == a_col);

        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                REQUIRE(sum_ab(i, j) == a(i, j) + b(i, j));
            }
        }

       REQUIRE_THROWS_AS(a+c, std::invalid_argument);
    }

    SECTION("Matrix Subtraction")
    {
        auto sub_ab = a - b;
        REQUIRE(sub_ab.shape().first == a_row);
        REQUIRE(sub_ab.shape().second == a_col);

        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                REQUIRE(sub_ab(i, j) == a(i, j) - b(i, j));
            }
        }

        REQUIRE_THROWS_AS(a-c, std::invalid_argument);
    }

    SECTION("Matrix Elementwise Multiplication")
    {
        auto mul_ab = a * b;
        REQUIRE(mul_ab.shape().first == a_row);
        REQUIRE(mul_ab.shape().second == a_col);

        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                REQUIRE(mul_ab(i, j) == a(i, j) * b(i, j));
            }
        }

        REQUIRE_THROWS_AS(a*c, std::invalid_argument);
    }

    SECTION("Matrix Division")
    {
        auto div_ab = a / b;
        REQUIRE(div_ab.shape().first == a_row);
        REQUIRE(div_ab.shape().second == a_col);

        for(int i=0; i<a_row; i++) {
            for(int j=0; j<a_col; j++) {
                REQUIRE(div_ab(i, j) == a(i, j) / b(i, j));
            }
        }

        REQUIRE_THROWS_AS(a / c, std::invalid_argument);
        b(0, 0, 0);
        REQUIRE_THROWS_AS(a / b, std::overflow_error);
        b(0, 0, 1);
    }

    SECTION("Matrix Multiplication")
    {

        auto mat_da = Matrix::MatMul(d, a);
        auto da_row = mat_da.shape().first;
        auto da_col = mat_da.shape().second;
        REQUIRE(da_row == d_row);
        REQUIRE(da_col == a_col);

        // do quaternion operators
        // check if (,) operation works too
        for(int i=0; i<da_row; i++) {
            for(int j=0; j<da_row; j++) {
                int sum_d = 0;
                for(int k=0; k<d_col; k++) {
                    sum_d += d(i, k) * a(k, j);
                }
                REQUIRE(sum_d == mat_da(i, j));
            }
        }

        auto m_i = Matrix(5, a_col + 1);
        REQUIRE_THROWS_AS(Matrix::MatMul(m_i, a), std::invalid_argument);
    }

    SECTION("Matrix Transpose")
    {
        Matrix c = Matrix::Transpose(a);
        for (int i = 0; i < a_col; i++)
        {
            for (int j = 0; j < a_row; j++)
            {
                REQUIRE(c(i, j) == a(j, i));
            }
        }
    }
}