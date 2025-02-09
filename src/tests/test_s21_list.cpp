#include "test_s21_containers.h"

#ifdef GCOV
template class s21::list<int>;
template class s21::listNode<int>;
template class s21::ListIterator_base<int, false>;
#endif

using s21::list;

TEST(testList, insert) {
  list<int> test_list;
  test_list.push_back(3);
  test_list.push_front(2);
  test_list.push_back(4);
  test_list.push_front(1);
  ASSERT_EQ(test_list.size(), 4);
  ASSERT_EQ(test_list.front(), 1);
  ASSERT_EQ(test_list.back(), 4);
  test_list.insert(test_list.begin(), 0);
  ASSERT_EQ(test_list.size(), 5);
  ASSERT_EQ(test_list.front(), 0);
  ASSERT_EQ(test_list.back(), 4);
  test_list.insert(test_list.end(), 5);
  ASSERT_EQ(test_list.size(), 6);
  ASSERT_EQ(test_list.front(), 0);
  ASSERT_EQ(test_list.back(), 5);
}

TEST(testList, sort) {
  list<int> test_list;
  test_list.push_back(2);
  test_list.push_back(1);
  test_list.push_back(4);
  test_list.push_back(3);
  test_list.push_back(5);
  test_list.sort();
  ASSERT_EQ(test_list.size(), 5);
  ASSERT_EQ(test_list.front(), 1);
  ASSERT_EQ(*(++test_list.begin()), 2);
  ASSERT_EQ(test_list.back(), 5);
}

TEST(testList, merge) {
  list<int> test_list1{1, 2, 3, 8, 9};
  list<int> test_list2{4, 5, 6, 7, 10};
  test_list1.merge(test_list2);
  ASSERT_EQ(test_list1.size(), 10);
  ASSERT_EQ(test_list1.front(), 1);
  ASSERT_EQ(test_list1.back(), 10);
}

TEST(testList, unique) {
  list<int> test_list{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 10,
                      10, 10, 11, 12, 13, 14, 15, 16, 17, 18};
  ASSERT_EQ(test_list.size(), 18 + 3);
  test_list.unique();
  ASSERT_EQ(test_list.size(), 18);
}

TEST(testList, reverse) {
  list<int> test_list{1,  2,  3,  4,  5,  6,  7,  8,  9,
                      10, 11, 12, 13, 14, 15, 16, 17, 18};
  ASSERT_EQ(test_list.size(), 18);
  ASSERT_EQ(test_list.front(), 1);
  ASSERT_EQ(test_list.back(), 18);
  test_list.reverse();
  ASSERT_EQ(test_list.size(), 18);
  ASSERT_EQ(test_list.front(), 18);
  ASSERT_EQ(*(++(test_list.begin())), 17);
  ASSERT_EQ(test_list.back(), 1);
}

TEST(testList, splice) {
  list<int> test_list1{1,  2,  3,  4,  5,  6,  7,  8,  9,
                       10, 11, 12, 13, 14, 15, 16, 17, 18};
  list<int> test_list2{19, 20, 21, 22};
  test_list1.splice(test_list1.cbegin(), test_list2);
  ASSERT_TRUE(test_list2.empty());
  ASSERT_EQ(test_list1.front(), 19);
  ASSERT_EQ(test_list1.back(), 18);
}

TEST(testList, copyAssigment) {
  list<int> test_list1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  list<int> test_list2{10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  test_list1 = test_list2;
  ASSERT_EQ(test_list1.size(), 11);
  ASSERT_EQ(test_list1.front(), 10);
  ASSERT_EQ(test_list1.back(), 20);
}

TEST(testList, moveAssigment) {
  list<int> test_list1{1, 2, 3, 4};
  list<int> test_list2{10, 11, 12, 13, 14, 15, 16, 17, 18};
  test_list1 = std::move(test_list2);
  ASSERT_EQ(test_list1.size(), 9);
  ASSERT_EQ(test_list1.front(), 10);
  ASSERT_EQ(test_list1.back(), 18);
}

TEST(testList, swap) {
  list<int> test_list1{1, 2, 3, 4, 5, 6, 7, 8, 9};
  list<int> test_list2{10, 11, 12, 13, 14, 15, 16, 17, 18};
  test_list1.swap(test_list2);
  ASSERT_EQ(test_list1.size(), 9);
  ASSERT_EQ(test_list1.front(), 10);
  ASSERT_EQ(test_list1.back(), 18);
  ASSERT_EQ(test_list2.size(), 9);
  ASSERT_EQ(test_list2.front(), 1);
  ASSERT_EQ(test_list2.back(), 9);
}

TEST(testList, pop) {
  list<int> test_list{1, 2, 3, 4, 5, 6, 7, 8, 9};
  test_list.pop_front();
  ASSERT_EQ(test_list.size(), 8);
  ASSERT_EQ(test_list.front(), 2);
  ASSERT_EQ(test_list.back(), 9);
  test_list.pop_back();
  ASSERT_EQ(test_list.size(), 7);
  ASSERT_EQ(test_list.front(), 2);
  ASSERT_EQ(test_list.back(), 8);
}

TEST(testList, erase) {
  list<int> test_list{1, 2, 3, 4, 5, 6, 7, 8, 9};
  test_list.erase(++(test_list.begin()));
  ASSERT_EQ(test_list.size(), 8);
  ASSERT_EQ(test_list.front(), 1);
}

TEST(testList, clear) {
  list<int> test_list{1, 2, 3, 4, 5, 6, 7, 8, 9};
  test_list.clear();
  ASSERT_TRUE(test_list.empty());
}

TEST(testList, insert_many) {
  list<int> test_list{1, 2, 3, 4, 5, 6, 7, 8, 9};
  ASSERT_EQ(test_list.size(), 9);
  test_list.insert_many(++(test_list.cbegin()), 10, 11, 12, 13, 15);
  ASSERT_EQ(test_list.size(), 14);
  ASSERT_EQ(test_list.front(), 1);
  ASSERT_EQ(*(++(test_list.begin())), 10);
  ASSERT_EQ(test_list.back(), 9);
  test_list.insert_many_back(16, 17, 18);
  ASSERT_EQ(test_list.size(), 17);
  ASSERT_EQ(test_list.front(), 1);
  ASSERT_EQ(test_list.back(), 18);
  test_list.insert_many_front(-1, 0);
  ASSERT_EQ(test_list.size(), 19);
  ASSERT_EQ(test_list.front(), -1);
  ASSERT_EQ(test_list.back(), 18);
}

// Test to ensure that creating a list with a size that exceeds the maximum
// allowable size throws a std::length_error exception. This tests the list's
// ability to handle overflow situations and enforce size constraints.
TEST(testList, overflow) {
  EXPECT_THROW(list<int> test_list(static_cast<size_t>(-1)), std::length_error);
}

TEST(testList, constructors) {
  list<int> test_list(100500);
  ASSERT_EQ(test_list.size(), 100500);
  list<int> test_list2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  test_list = test_list2;
  ASSERT_EQ(test_list.size(), 10);
  ASSERT_EQ(test_list.front(), 1);
  ASSERT_EQ(test_list.back(), 10);
  test_list = list<int>{10, 11, 12};
  ASSERT_EQ(test_list.size(), 3);
  ASSERT_EQ(test_list.front(), 10);
  ASSERT_EQ(test_list.back(), 12);
  list<int> test_list3(test_list);
  ASSERT_EQ(test_list3.size(), 3);
  ASSERT_EQ(test_list3.front(), 10);
  ASSERT_EQ(test_list3.back(), 12);
  list<int> test_list4(std::move(test_list3));
  ASSERT_EQ(test_list4.size(), 3);
  ASSERT_EQ(test_list4.front(), 10);
  ASSERT_EQ(test_list4.back(), 12);
}
