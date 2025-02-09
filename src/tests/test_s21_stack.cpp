#include "test_s21_containers.h"

#ifdef GCOV
template class s21::stack<int>;
#endif

using s21::stack;

TEST(testStack, defaultConstructor) {
  stack<int> s;
  EXPECT_EQ(s.size(), 0);
}
TEST(testStack, initializerListConstructor) {
  stack<int> s = {1, 2, 3};
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.top(), 3);
  s.pop();
  EXPECT_EQ(s.top(), 2);
  s.pop();
  EXPECT_EQ(s.top(), 1);
}

TEST(testStack, copyConstructor) {
  stack<int> s1 = {1, 2, 3};
  stack<int> s2 = s1;
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.top(), 3);
  s2.pop();
  EXPECT_EQ(s2.top(), 2);
  s2.pop();
  EXPECT_EQ(s2.top(), 1);
}

TEST(testStack, moveConstructor) {
  stack<int> s1 = {1, 2, 3};
  stack<int> s2 = std::move(s1);
  EXPECT_EQ(s1.size(), 0);
  EXPECT_EQ(s2.top(), 3);
  s2.pop();
  EXPECT_EQ(s2.top(), 2);
  s2.pop();
  EXPECT_EQ(s2.top(), 1);
}

TEST(testStack, copyAssignment) {
  stack<int> s1 = {0, 1, 2, 3};
  stack<int> s2 = {4, 5, 6, 7, 8, 9};
  stack<int> s3 = {10, 11};
  s2 = s1;
  EXPECT_EQ(s2.size(), 4);
  EXPECT_EQ(s2.top(), 3);
  s3 = s2;
  EXPECT_EQ(s3.size(), 4);
  EXPECT_EQ(s2.top(), 3);
}

TEST(testStack, moveAssignment) {
  stack<int> s1 = {0, 1, 2, 3};
  stack<int> s2 = {4, 5, 6, 7, 8, 9};
  stack<int> s3 = {10, 11};
  s2 = std::move(s1);
  EXPECT_EQ(s2.size(), 4);
  EXPECT_EQ(s2.top(), 3);
  s3 = std::move(s2);
  EXPECT_EQ(s3.size(), 4);
  EXPECT_EQ(s3.top(), 3);
  EXPECT_EQ(s2.size(), 2);
  EXPECT_EQ(s1.size(), 6);
}

TEST(testStack, swap) {
  stack<int> s1 = {0, 1, 2, 3};
  stack<int> s2 = {4, 5, 6, 7, 8, 9};
  stack<int> s3 = {10, 11};
  s1.swap(s2);
  EXPECT_EQ(s1.size(), 6);
  EXPECT_EQ(s1.top(), 9);
  EXPECT_EQ(s2.size(), 4);
  EXPECT_EQ(s2.top(), 3);
  s1.swap(s3);
  EXPECT_EQ(s1.size(), 2);
  EXPECT_EQ(s1.top(), 11);
  EXPECT_EQ(s3.size(), 6);
  EXPECT_EQ(s3.top(), 9);
}

TEST(testStack, push) {
  stack<int> s;
  s.push(10);
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top(), 10);
  s.push(11);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 11);
  s.push(12);
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.top(), 12);
}

TEST(testStack, pop) {
  stack<int> s = {0, 1, 2, 3};
  s.pop();
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.top(), 2);
  s.pop();
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 1);
  s.pop();
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top(), 0);
  s.pop();
  EXPECT_EQ(s.size(), 0);
  EXPECT_THROW(s.pop(), std::out_of_range);
}

TEST(testStack, insert_many_back) {
  stack<int> s;
  s.insert_many_back(10, 11, 12, 134);
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.top(), 10);
  s.insert_many_back(1345, 1346);
  EXPECT_EQ(s.size(), 6);
  s.pop();
  s.pop();
  s.pop();
  s.pop();
  EXPECT_EQ(s.top(), 1345);
}