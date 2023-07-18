/*
 * @Author: ls02 liangson02@163.com
 * @Date: 2023-07-18 11:09:57
 * @LastEditors: ls02 liangson02@163.com
 * @LastEditTime: 2023-07-18 11:26:20
 * @FilePath: /Vector/unit_test/unit_vector.cc
 * @Description: 
 * 
 * Copyright (c) 2023 by ls02, All Rights Reserved. 
 */

#include <gtest/gtest.h>
#include <iostream>
#include "../src/MyVector.hpp"

class VectorTest : public testing::Test {};

TEST_F(VectorTest, insert)
{
    ls::vector<int> v1;
    
    // 头插
    v1.insert(0, 1);
    EXPECT_EQ(1, v1[0]);
    v1.insert(0, 2);
    EXPECT_EQ(2, v1[0]);
    v1.insert(0, 3);
    EXPECT_EQ(3, v1[0]);
    v1.insert(0, 4);
    EXPECT_EQ(4, v1[0]);

    // 尾插
    v1.insert(v1.size(), 9);
    EXPECT_EQ(9, v1[v1.size() - 1]);

    v1.insert(v1.size(), 8);
    EXPECT_EQ(8, v1[v1.size() - 1]);

    v1.insert(v1.size(), 7);
    EXPECT_EQ(7, v1[v1.size() - 1]);

    v1.insert(v1.size(), 6);
    EXPECT_EQ(6, v1[v1.size() - 1]);

    // 中间插入
    v1.insert(3, 10);
    EXPECT_EQ(10, v1[3]);

    v1.insert(3, 14);
    EXPECT_EQ(14, v1[3]);

    v1.insert(3, 13);
    EXPECT_EQ(13, v1[3]);

    v1.insert(3, 12);
    EXPECT_EQ(12, v1[3]);
}



int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
