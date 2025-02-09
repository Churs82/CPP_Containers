#include "test_s21_containers.h"

#ifdef GCOV
template class s21::vector<int>;
#endif

using s21::vector;

TEST(testVector, maxsize) {
  size_t s = 0;
  auto v = vector<int>();
  ASSERT_THROW(v.reserve(s - 1), std::length_error);
}

TEST(testVector, defaultConstructor) {
  vector<int> v;
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.capacity(), 0);
  ASSERT_EQ(v.empty(), true);
}

TEST(testVector, reserve) {
  vector<int> v = {1, 2};
  v.reserve(50);
  ASSERT_EQ(v.capacity(), 50);
  v.reserve(10);
  ASSERT_EQ(v.capacity(), 50);
  v.shrink_to_fit();
  ASSERT_EQ(v.capacity(), 2);
  v.reserve(100);
  ASSERT_EQ(v.capacity(), 100);
}

TEST(testVector, insert) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.insert(v.begin() + 3, 10);
  ASSERT_EQ(v.size(), 6);
  ASSERT_EQ(v[3], 10);
}

TEST(testVector, insert_shrink) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.shrink_to_fit();
  v.insert(v.begin() + 3, 10);
  ASSERT_EQ(v.size(), 6);
  ASSERT_EQ(v[3], 10);
}

TEST(testVector, erase) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.erase(v.begin() + 3);
  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[3], 5);
}

TEST(testVector, clear) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.clear();
  ASSERT_EQ(v.size(), 0);
  ASSERT_EQ(v.empty(), true);
}

TEST(testVector, push_back) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.shrink_to_fit();
  ASSERT_EQ(v.capacity(), 5);
  v.push_back(6);
  ASSERT_EQ(v.size(), 6);
  ASSERT_EQ(v[5], 6);
}

TEST(testVector, pop_back) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.pop_back();
  ASSERT_EQ(v.size(), 4);
  ASSERT_EQ(v[4], 5);
}

TEST(testVector, at) {
  vector<int> v = {1, 2, 3, 4, 5};
  ASSERT_EQ(v.at(3), 4);
}

TEST(testVector, square_brackets) {
  vector<float> v = {1.5, 2.123, 3.32322, 4.3244, 5.2322};
  ASSERT_FLOAT_EQ(v[2], 3.32322);
}

/* Test copy constructor*/
TEST(testVector, copyConstructor) {
  vector<int> v = {1, 2, 3, 4, 5};
  vector<int> v1(v);
  ASSERT_EQ(v1.size(), 5);
  ASSERT_EQ(v1[0], 1);
  ASSERT_EQ(v1[1], 2);
  ASSERT_EQ(v1[2], 3);
  ASSERT_EQ(v1[3], 4);
  ASSERT_EQ(v1[4], 5);
}

/* Test move constructor*/
TEST(testVector, moveConstructor) {
  vector<int> v = {1, 2, 3, 4, 5};
  vector<int> v1(std::move(v));
  ASSERT_EQ(v1.size(), 5);
  ASSERT_EQ(v1.front(), 1);
  ASSERT_EQ(v1[1], 2);
  ASSERT_EQ(v1[2], 3);
  ASSERT_EQ(v1[3], 4);
  ASSERT_EQ(v1.back(), 5);
  v = v1 = vector{6, 7, 8, 9, 10};
  ASSERT_EQ(v1[0], 6);
  ASSERT_EQ(v1[1], 7);
  ASSERT_EQ(v1[2], 8);
  ASSERT_EQ(v1[3], 9);
  ASSERT_EQ(v1[4], 10);
}

/* Test swap vector*/
TEST(testVector, swap) {
  vector<int> v = {1, 2, 3, 4, 5};
  vector<int> v1 = {6, 7, 8, 9, 10};
  v.swap(v1);
  ASSERT_EQ(v.size(), 5);
  ASSERT_EQ(v[0], 6);
  ASSERT_EQ(v[1], 7);
  ASSERT_EQ(v[2], 8);
  ASSERT_EQ(v[3], 9);
  ASSERT_EQ(v[4], 10);
  ASSERT_EQ(v1.size(), 5);
  ASSERT_EQ(v1[0], 1);
  ASSERT_EQ(v1[1], 2);
  ASSERT_EQ(v1[2], 3);
  ASSERT_EQ(v1[3], 4);
  ASSERT_EQ(v1[4], 5);
}

TEST(testVector, iterator) {
  vector<int> v = {1, 2, 3, 4, 5};
  auto it = v.begin();
  ASSERT_EQ(*it, 1);
  ASSERT_EQ(*(it++), 1);
  ASSERT_EQ(*(it--), 2);
  ASSERT_EQ(*(++it), 2);
  ASSERT_EQ(*(--it), 1);
  it += 2;
  ASSERT_EQ(*it, 3);
  it -= 2;
  ASSERT_EQ(*it, 1);
  it = v.end() - 1;
  ASSERT_EQ(*it, 5);
  it = v.begin() + 2;
  ASSERT_EQ(*it, 3);
}

TEST(testVector, exception) {
  vector<int> v = {1, 2, 3, 4, 5};
  ASSERT_THROW(v.at(10), std::out_of_range);
}

TEST(testVector, exception2) {
  vector<int> v = {1, 2, 3, 4, 5};
  ASSERT_THROW(v.at(-1), std::out_of_range);
}

TEST(testVector, exception3) {
  vector<int> v = {};
  ASSERT_THROW(v.front(), std::out_of_range);
  ASSERT_THROW(v.back(), std::out_of_range);
}

TEST(testVector, exception4) {
  vector<int> v = {};
  ASSERT_THROW(v.insert(v.end() + 1, 10), std::out_of_range);
  ASSERT_THROW(v.erase(v.end()), std::out_of_range);
}

// TEST(testVector, exception5) {
//   vector<int> v;
//   ASSERT_THROW(v.insert_many_back(1, 2, 3, 4), std::runtime_error);
//   ASSERT_THROW(v.push_back(1), std::runtime_error);
// }

TEST(testVector, insert_many_back) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.insert_many_back(6, 7, 8, 9, 10);
  ASSERT_EQ(v.size(), 10);
  ASSERT_EQ(v[0], 1);
  ASSERT_EQ(v[1], 2);
  ASSERT_EQ(v[2], 3);
  ASSERT_EQ(v[3], 4);
  ASSERT_EQ(v[4], 5);
  ASSERT_EQ(v[5], 6);
  ASSERT_EQ(v[6], 7);
  ASSERT_EQ(v[7], 8);
  ASSERT_EQ(v[8], 9);
  ASSERT_EQ(v[9], 10);
}

TEST(testVector, insert_many_back_shrink) {
  vector<int> v = {1, 2, 3, 4, 5};
  v.shrink_to_fit();
  v.insert_many_back(6, 7, 8, 9, 10);
  ASSERT_EQ(v.size(), 10);
  ASSERT_EQ(v[0], 1);
  ASSERT_EQ(v[1], 2);
  ASSERT_EQ(v[2], 3);
  ASSERT_EQ(v[3], 4);
  ASSERT_EQ(v[4], 5);
  ASSERT_EQ(v[5], 6);
  ASSERT_EQ(v[6], 7);
  ASSERT_EQ(v[7], 8);
  ASSERT_EQ(v[8], 9);
  ASSERT_EQ(v[9], 10);
}

TEST(testVector, insert_many) {
  auto v = vector<int>{1, 2, 3, 4, 5};
  const vector<int> v1{5, 6, 7, 8, 9};
  v.insert_many(v.cbegin() + 1, v1[2], v1[3], v1[0]);
}

TEST(testVector, insert_many_shrink) {
  auto v = vector<int>{1, 2, 3, 4, 5};
  ASSERT_EQ(v.capacity(), 10);
  v.shrink_to_fit();
  ASSERT_EQ(v.capacity(), v.size());
  ASSERT_EQ(v.capacity(), 5);
  const vector<int> v1{5, 6, 7, 8, 9};
  v.insert_many(v.cbegin() + 1, v1[2], v1[3], v1[0]);
  ASSERT_EQ(v.size(), 8);
  ASSERT_EQ(v[0], 1);
  ASSERT_EQ(v[1], 7);
  ASSERT_EQ(v[2], 8);
  ASSERT_EQ(v[3], 5);
  ASSERT_EQ(v[4], 2);
  ASSERT_EQ(v[5], 3);
  ASSERT_EQ(v[6], 4);
  ASSERT_EQ(v[7], 5);
}

TEST(testVector, data) {
  auto v = vector<int>{1, 2, 3, 4, 5};
  ASSERT_EQ(v.data(), &v[0]);
  ASSERT_EQ(v.data() + v.size(), &v[v.size()]);
}

TEST(testVector, vector_of_vectors) {
  auto v1 = vector<int>{1, 2, 3, 4, 5};
  auto v2 = vector<int>{6, 7, 8, 9, 10};
  auto v3 = vector<int>{11, 12, 13, 14, 15};
  auto v4 = vector<int>{16, 17, 18, 19, 20};
  auto v5 = vector<int>{21, 22, 23, 24, 25};
  auto v6 = vector<int>{26, 27, 28, 29, 30};
  auto v = vector<vector<int>>{v1, v2, v3, v5, v6};
  ASSERT_EQ(v.size(), 5);
  v.insert(v.begin() + 2, v4);
  ASSERT_EQ(v.size(), 6);
  ASSERT_EQ(v[0].size(), 5);
  ASSERT_EQ(v[1].size(), 5);
  ASSERT_EQ(v[2].size(), 5);
  ASSERT_EQ(v[3].size(), 5);
  ASSERT_EQ(v[4].size(), 5);
  ASSERT_EQ(v[5].size(), 5);
  v.erase(v.begin() + 2);
  ASSERT_EQ(v.size(), 5);
  v.shrink_to_fit();
  v.insert(v.begin() + 2, v4);
  ASSERT_EQ(v.size(), 6);
}

// Empty function to "group" vector tests
void AddVectorTests() {}
