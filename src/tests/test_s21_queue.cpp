#include "test_s21_containers.h"

#ifdef GCOV
template class s21::queue<int>;
template class s21::QueueNode<int>;
template class s21::QueueIterator_base<int, false>;
#endif

using s21::queue;

TEST(testQueue, defaultConstructor) {
  queue<int> q;
  EXPECT_EQ(q.size(), 0);
}
TEST(testQueue, initializerListConstructor) {
  queue<int> q = {1, 2, 3};
  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
}

TEST(testQueue, copyConstructor) {
  queue<int> q1 = {1, 2, 3};
  queue<int> q2 = q1;
  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}

TEST(testQueue, moveConstructor) {
  queue<int> q1 = {1, 2, 3};
  queue<int> q2 = std::move(q1);
  EXPECT_EQ(q1.size(), 0);
  EXPECT_EQ(q2.size(), 3);
  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);
}

TEST(testQueue, copyAssignment) {
  queue<int> q1 = {0, 1, 2, 3};
  queue<int> q2 = {4, 5, 6, 7, 8, 9};
  queue<int> q3 = {10, 11};
  q2 = q1;
  EXPECT_EQ(q2.size(), 4);
  EXPECT_EQ(q2.front(), 0);
  EXPECT_EQ(q2.back(), 3);
  q3 = q2;
  EXPECT_EQ(q3.size(), 4);
  EXPECT_EQ(q3.front(), 0);
  EXPECT_EQ(q3.back(), 3);
}

TEST(testQueue, moveAssignment) {
  queue<int> q1 = {0, 1, 2, 3};
  queue<int> q2 = {4, 5, 6, 7, 8, 9};
  queue<int> q3 = {10, 11};
  q2 = std::move(q1);
  EXPECT_EQ(q2.size(), 4);
  EXPECT_EQ(q2.front(), 0);
  EXPECT_EQ(q2.back(), 3);
  q3 = std::move(q2);
  EXPECT_EQ(q3.size(), 4);
  EXPECT_EQ(q3.front(), 0);
  EXPECT_EQ(q3.back(), 3);
}

TEST(testQueue, swap) {
  queue<int> q1 = {0, 1, 2, 3};
  queue<int> q2 = {4, 5, 6, 7, 8, 9};
  queue<int> q3 = {10, 11};
  q1.swap(q2);
  EXPECT_EQ(q1.size(), 6);
  EXPECT_EQ(q1.front(), 4);
  EXPECT_EQ(q1.back(), 9);
  EXPECT_EQ(q2.size(), 4);
  EXPECT_EQ(q2.front(), 0);
  EXPECT_EQ(q2.back(), 3);
  q1.swap(q3);
  EXPECT_EQ(q1.size(), 2);
  EXPECT_EQ(q1.front(), 10);
  EXPECT_EQ(q1.back(), 11);
  EXPECT_EQ(q3.size(), 6);
  EXPECT_EQ(q3.front(), 4);
  EXPECT_EQ(q3.back(), 9);
}

TEST(testQueue, push) {
  queue<int> q;
  q.push(10);
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 10);
  q.push(11);
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 11);
  q.push(12);
  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 12);
}

TEST(testQueue, pop) {
  queue<int> q = {0, 1, 2, 3};
  q.pop();
  EXPECT_EQ(q.size(), 3);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 3);
  q.pop();
  EXPECT_EQ(q.size(), 2);
  EXPECT_EQ(q.front(), 2);
  EXPECT_EQ(q.back(), 3);
  q.pop();
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), 3);
  EXPECT_EQ(q.back(), 3);
}

TEST(testQueue, insert_many_back) {
  queue<int> q;
  q.insert_many_back(10, 11, 12, 134);
  EXPECT_EQ(q.size(), 4);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 134);
  q.insert_many_back(1345, 1346);
  EXPECT_EQ(q.size(), 6);
  EXPECT_EQ(q.front(), 10);
  EXPECT_EQ(q.back(), 1346);
}