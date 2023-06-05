#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include <iostream>
#include "../src/MyString.hpp"

class StringTest : public testing::Test {};


TEST_F(StringTest, insert1)
{
    ls::string str("new str");
    EXPECT_STREQ(str.c_str(), "new str");

    str.insert(0, "abc");
    EXPECT_STREQ(str.c_str(), "abcnew str");

    str.insert(1, 'b');
    EXPECT_STREQ(str.c_str(), "abbcnew str");
    
    str.insert(2, "hhhh");
    EXPECT_STREQ(str.c_str(), "abhhhhbcnew str");

    str.insert(str.size(), "niubi");
    EXPECT_STREQ(str.c_str(), "abhhhhbcnew strniubi");
}

TEST_F(StringTest, insert2)
{
    ls::string str("new str");
    EXPECT_STREQ(str.c_str(), "new str");

    str.insert(0, "hello");
    EXPECT_STREQ(str.c_str(), "hellonew str");

    str.insert(0, "abab");
    EXPECT_STREQ(str.c_str(), "ababhellonew str");
}

TEST_F(StringTest, insert3)
{
    ls::string str1("new str1");
    ls::string str2("new str2");
    EXPECT_STREQ(str1.c_str(), "new str1");
    EXPECT_STREQ(str2.c_str(), "new str2");

    str1.insert(0, str2);
    EXPECT_STREQ(str1.c_str(), "new str2new str1");

    str1.insert(1, str2);
    EXPECT_STREQ(str1.c_str(), "nnew str2ew str2new str1");
    str1.insert(str1.size(), str2);
    EXPECT_STREQ(str1.c_str(), "nnew str2ew str2new str1new str2");
}

TEST_F(StringTest, pushback1)
{
    ls::string str("new str");
    EXPECT_STREQ(str.c_str(), "new str");

    str.PushBack("hello");
    EXPECT_STREQ(str.c_str(), "new strhello");

    str.PushBack('a');
    EXPECT_STREQ(str.c_str(), "new strhelloa");
}

TEST_F(StringTest, pushback2)
{
    ls::string str1("new str1");
    EXPECT_STREQ(str1.c_str(), "new str1");

    ls::string str2("new str2");
    EXPECT_STREQ(str2.c_str(), "new str2");

    str1 += "hello";
    EXPECT_STREQ(str1.c_str(), "new str1hello");
    str1 += "abc";
    EXPECT_STREQ(str1.c_str(), "new str1helloabc");
    str1 += "aacc";
    EXPECT_STREQ(str1.c_str(), "new str1helloabcaacc");

    str1 += str2;
    EXPECT_STREQ(str1.c_str(), "new str1helloabcaaccnew str2");
}

TEST_F(StringTest, at1)
{
    ls::string str("new str");

    EXPECT_STREQ(str.c_str(), "new str");
    
    EXPECT_EQ(str.at(0), 'n');
    EXPECT_EQ(str.at(1), 'e');
    EXPECT_EQ(str.at(2), 'w');
    EXPECT_EQ(str.at(3), ' ');
    EXPECT_EQ(str.at(4), 's');
    EXPECT_EQ(str.at(5), 't');
    EXPECT_EQ(str.at(6), 'r');

    EXPECT_EQ(str[0], 'n');
    EXPECT_EQ(str[1], 'e');
    EXPECT_EQ(str[2], 'w');
    EXPECT_EQ(str[3], ' ');
    EXPECT_EQ(str[4], 's');
    EXPECT_EQ(str[5], 't');
    EXPECT_EQ(str[6], 'r');
}

TEST_F(StringTest, reserver1)
{
    ls::string str;
    
    EXPECT_EQ(0, str.capacity());

    str.reserver(10);
    EXPECT_EQ(10, str.capacity()); 

    str.reserver(100);
    EXPECT_EQ(100, str.capacity());

    str.reserver(50);
    EXPECT_EQ(100, str.capacity());

    str.reserver(0);
    EXPECT_EQ(100, str.capacity());
}

TEST_F(StringTest, erase1)
{
    ls::string str("new hello");
    EXPECT_STREQ(str.c_str(), "new hello");

    str.erase(7, 100);
    EXPECT_STREQ(str.c_str(), "new hel");

    str.erase(1, 2);
    EXPECT_STREQ(str.c_str(), "n hel");

    str.erase(0, 3);
    EXPECT_STREQ(str.c_str(), "el");

    str.erase(0);
    EXPECT_STREQ(str.c_str(), "");
}

TEST_F(StringTest, find1)
{
    ls::string str("hello");

    EXPECT_EQ(str.find("ello"), 1);
    EXPECT_EQ(str.find("he"), 0);
    EXPECT_EQ(str.find("llo"), 2);
    EXPECT_EQ(str.find("llo", 3), -1);

    ls::string new_str("llo");
    EXPECT_EQ(str.find(new_str, 2), 2);
    EXPECT_EQ(str.find(new_str, 4), -1);

    ls::string tmp = str.substr(1);
    EXPECT_STREQ(tmp.c_str(), "ello");

    tmp = str.substr(2, 1);
    EXPECT_STREQ(tmp.c_str(), "l");

    tmp = str.substr();
    EXPECT_STREQ(tmp.c_str(), "hello");
}

TEST_F(StringTest, io1)
{
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}



//using ::testing::Return;
//using ::testing::_;
//using ::testing::Invoke;
//// 需要被测试的类
//class Calculator {
//public:
//    virtual ~Calculator() {}
//    virtual int Add(int a, int b) { return a + b; }
//    virtual int Subtract(int a, int b) { return a - b; }
//    virtual int Multiply(int a, int b) { return a * b; }
//    virtual int Divide(int a, int b) { return a / b; }
//};
//
//// 模拟类
//class MockCalculator : public Calculator {
//public:
//    MOCK_METHOD(int, Add, (int a, int b), (override));
//    MOCK_METHOD(int, Subtract, (int a, int b), (override));
//    MOCK_METHOD(int, Multiply, (int a, int b), (override));
//    MOCK_METHOD(int, Divide, (int a, int b), (override));
//};
//
//// 测试用例
//TEST(CalculatorTest, AddTest) {
//    MockCalculator calculator;  // 创建模拟对象
//
//    // 设置模拟对象的行为
//    int expectedSum = 5;
//    EXPECT_CALL(calculator, Add(2, 3))
//        .WillOnce(Return(expectedSum));
//
//    // 调用模拟对象的方法
//    int result = calculator.Add(2, 3);
//
//    // 验证调用和结果
//    EXPECT_EQ(result, expectedSum);
//}
//
//TEST(CalculatorTest, MultiplyTest) {
//    MockCalculator calculator;
//
//    // 设置模拟对象的行为
//    EXPECT_CALL(calculator, Multiply(_, _))
//        .WillOnce(Invoke([](int a, int b) { return a * b; }));
//
//    // 调用模拟对象的方法
//    int result = calculator.Multiply(2, 3);
//
//    // 验证调用和结果
//    EXPECT_EQ(result, 6);
//}

