#include <iostream>
#include <stdlib.h>
#include <vector>

#include <matrix.hpp>
#include <quaternion.hpp>
#include <catch.hpp>
#include <settings.hpp>

using namespace numerics;
using namespace settings;

TEST_CASE("Creating Quaternion", "[construction]")
{

    SECTION("Creating Quaternion via constructor") {
        real r = 18;
        real i = 112;
        real j = 11.1;
        real k = 1.123;

        Quaternion a = Quaternion(r, i, j, k);
        REQUIRE(a.r == r);
        REQUIRE(a.i == i);
        REQUIRE(a.j == j);
        REQUIRE(a.k == k);
    }

}

TEST_CASE("Quaternion can be applied with", "[Auxilary Operations]") {

    SECTION("Squared Magnitude") {
        real r = 18;
        real i = 112;
        real j = 11.1;
        real k = 1.123;

        Quaternion a = Quaternion(r, i, j, k);
        real sq_mag = a.SquaredMagnitude();
        REQUIRE(sq_mag == r * r + i * i + j* j + k * k);
    }

    SECTION("Normalise") {
        real r = 18;
        real i = 112;
        real j = 11.1;
        real k = 1.123;

        Quaternion a = Quaternion(r, i, j, k);
        a.Normalise();
        Approx(1 == r * r + i * i + j* j + k * k).margin(1e-12);
    }
}

TEST_CASE("Quaternion can be operated with", "[Operations]")
{
    SECTION("Addition") {
        real a_r = 3;
        real a_i = 1;
        real a_j = 9;
        real a_k = 10;

        real b_r = 5;
        real b_i = 11;
        real b_j = 19;
        real b_k = 100;

        Quaternion a = Quaternion(a_r, a_i, a_j, a_k);
        Quaternion b = Quaternion(b_r, b_i, b_j, b_k);

        auto c = a + b;
        REQUIRE(c.r == a_r + b_r);
        REQUIRE(c.i == a_i + b_i);
        REQUIRE(c.j == a_j + b_j);
        REQUIRE(c.k == a_k + b_k);

    }

    SECTION("Subtraction") {
        real a_r = 3;
        real a_i = 1;
        real a_j = 9;
        real a_k = 10;

        real b_r = 5;
        real b_i = 11;
        real b_j = 19;
        real b_k = 100;

        Quaternion a = Quaternion(a_r, a_i, a_j, a_k);
        Quaternion b = Quaternion(b_r, b_i, b_j, b_k);

        auto c = a - b;

        REQUIRE(c.r == a_r - b_r);
        REQUIRE(c.i == a_i - b_i);
        REQUIRE(c.j == a_j - b_j);
        REQUIRE(c.k == a_k - b_k);
    }


    SECTION("Multiplication") {
        real a_r = 3;
        real a_i = 1;
        real a_j = 9;
        real a_k = 10;

        real b_r = 5;
        real b_i = 11;
        real b_j = 19;
        real b_k = 100;

        Quaternion a = Quaternion(a_r, a_i, a_j, a_k);
        Quaternion b = Quaternion(b_r, b_i, b_j, b_k);

        auto c = a * b;

        REQUIRE(c.r == -1167);
        REQUIRE(c.i == 748);
        REQUIRE(c.j == 112);
        REQUIRE(c.k == 270);
    }

    SECTION("Inverse") {
        real a_r = 3;
        real a_i = 1;
        real a_j = 9;
        real a_k = 10;

        Quaternion a = Quaternion(a_r, a_i, a_j, a_k);

        auto a_inv = Quaternion::Inverse(a);

        Approx(a_inv.r == 0.0157068).margin(1e-12);
        Approx(a_inv.i == -0.0052356).margin(1e-12);
        Approx(a_inv.j == -0.0471204).margin(1e-12);
        Approx(a_inv.k == -0.052356).margin(1e-12);
    }


}