#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include "tiny/swisstable.h"

TEST(SwissTableTest, Insert)
{
    swisstable::SwissTable<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    ASSERT_EQ(map.size(), 3);
}

TEST(SwissTableTest, Erase)
{
    swisstable::SwissTable<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    map.erase(1);
    ASSERT_EQ(map.size(), 2);
}

TEST(SwissTableTest, Count)
{
    swisstable::SwissTable<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    ASSERT_EQ(map.count(1), 1);
    ASSERT_EQ(map.count(4), 0);
}

TEST(SwissTableTest, Operator)
{
    swisstable::SwissTable<int, int> map;
    map.insert({1, 2});
    map.insert({2, 3});
    map.insert({3, 4});
    ASSERT_EQ(map[1], 2);
    map[1] = 3;
    ASSERT_EQ(map[1], 3);
}

TEST(SwissTableTest, MultiThread)
{
    swisstable::SwissTable<int, int> map;
    std::vector<std::thread> threads;
    std::mutex m;
    for (int i = 0; i < 10; i++) {
        threads.push_back(std::thread([&map, &m, i]() {
            std::lock_guard<std::mutex> lock(m);
            map.insert({i, i});
        }));
    }
    for (auto &t : threads) { t.join(); }
    ASSERT_EQ(map.size(), 10);
}