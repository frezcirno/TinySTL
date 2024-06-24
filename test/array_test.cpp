#include <gtest/gtest.h>
#include <array>
#include "tiny/array.h"

TEST(array, constructor)
{
    tiny::array<int, 100> a;
    EXPECT_EQ(a.size(), 100);
}

TEST(array, fill)
{
    tiny::array<int, 100> a;
    a.fill(1);
    for (auto it = a.begin(); it != a.end(); it++) { EXPECT_EQ(*it, 1); }
}