#include <gtest/gtest.h>
#include "tiny/deque.h"
#include <iostream>
#include "mem_test.h"

TEST(deque, constructor)
{
    tiny::deque<int> l1;
    EXPECT_EQ(l1.size(), 0);
}

TEST(deque, push_back)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    EXPECT_EQ(l1.size(), 3);
}

TEST(deque, push_front)
{
    tiny::deque<int> l1;
    l1.push_front(1);
    l1.push_front(2);
    l1.push_front(3);
    EXPECT_EQ(l1.size(), 3);
}

TEST(deque, pop_back)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.pop_back();
    EXPECT_EQ(l1.size(), 2);
}

TEST(deque, pop_front)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.pop_front();
    EXPECT_EQ(l1.size(), 2);
}

TEST(deque, clear)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.clear();
    EXPECT_EQ(l1.size(), 0);
}

TEST(deque, iterator)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    int i = 1;
    for (auto it = l1.begin(); it != l1.end(); it++) {
        EXPECT_EQ(*it, i);
        i++;
    }
}

// TEST(deque, reverse_iterator)
// {
//     tiny::deque<int> l1;
//     l1.push_back(1);
//     l1.push_back(2);
//     l1.push_back(3);
//     int i = 3;
//     for (auto it = l1.rbegin(); it != l1.rend(); it++) {
//         EXPECT_EQ(*it, i);
//         i--;
//     }
// }

TEST(deque, copy_constructor)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    tiny::deque<int> l2(l1);
    EXPECT_EQ(l2.size(), 3);
}

TEST(deque, move_constructor)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    tiny::deque<int> l2(std::move(l1));
    EXPECT_EQ(l2.size(), 3);
}

TEST(deque, copy_assignment)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    tiny::deque<int> l2;
    l2 = l1;
    EXPECT_EQ(l2.size(), 3);
}

TEST(deque, move_assignment)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    tiny::deque<int> l2;
    l2 = std::move(l1);
    EXPECT_EQ(l2.size(), 3);
}

TEST(deque, at)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    EXPECT_EQ(l1.at(0), 1);
    EXPECT_EQ(l1.at(1), 2);
    EXPECT_EQ(l1.at(2), 3);
}

TEST(deque, front)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    EXPECT_EQ(l1.front(), 1);
}

TEST(deque, back)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    EXPECT_EQ(l1.back(), 3);
}

TEST(deque, operator_square_brackets)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    EXPECT_EQ(l1[0], 1);
    EXPECT_EQ(l1[1], 2);
    EXPECT_EQ(l1[2], 3);
}

TEST(deque, insert)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.insert(l1.begin() + 1, 4);
    EXPECT_EQ(l1.size(), 4);
    EXPECT_EQ(l1[1], 4);
}

TEST(deque, erase)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    l1.erase(l1.begin() + 1);
    EXPECT_EQ(l1.size(), 2);
    EXPECT_EQ(l1[1], 3);
}

TEST(deque, swap)
{
    tiny::deque<int> l1;
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    tiny::deque<int> l2;
    l2.push_back(4);
    l2.push_back(5);
    l2.push_back(6);
    l1.swap(l2);
    EXPECT_EQ(l1.size(), 3);
    EXPECT_EQ(l2.size(), 3);
    EXPECT_EQ(l1[0], 4);
    EXPECT_EQ(l2[0], 1);
}
