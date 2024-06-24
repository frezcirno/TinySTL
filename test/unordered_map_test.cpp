#include <gtest/gtest.h>
#include "tiny/unordered_map.h"

TEST(unordered_map, constructor)
{
    tiny::unordered_map<int, int> map;
    EXPECT_EQ(map.size(), 0);
}

TEST(unordered_map, insert)
{
    tiny::unordered_map<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    EXPECT_EQ(map.size(), 3);
}

TEST(unordered_map, operator)
{
    tiny::unordered_map<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    EXPECT_EQ(map[1], 2);
    map[1] = 3;
    EXPECT_EQ(map[1], 3);
}

TEST(unordered_map, count)
{
    tiny::unordered_map<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    EXPECT_EQ(map.count(1), 1);
    EXPECT_EQ(map.count(4), 0);
}

TEST(unordered_map, erase)
{
    tiny::unordered_map<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    map.erase(++map.begin());
    EXPECT_EQ(map.size(), 2);
}
