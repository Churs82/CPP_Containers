#include "test_s21_containers.h"

#ifdef GCOV
template class s21::multiset<int>;
template class s21::BinaryTree<int>;
#endif

TEST(multisetTest, defaultConstructor) {
  s21::multiset<int> ms;
  ASSERT_TRUE(ms.empty());
  ASSERT_EQ(ms.size(), 0);
}

TEST(multisetTest, initializerListConstructor) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};
  ASSERT_EQ(ms.size(), 5);
  ASSERT_TRUE(ms.contains(1));
  ASSERT_TRUE(ms.contains(2));
  ASSERT_TRUE(ms.contains(3));
  ASSERT_TRUE(ms.contains(4));
}

TEST(multisetTest, copyConstructor) {
  s21::multiset<int> ms1 = {1, 2, 3};
  s21::multiset<int> ms2 = ms1;

  ASSERT_EQ(ms1.size(), 3);
  ASSERT_EQ(ms2.size(), 3);
  ASSERT_TRUE(ms1.contains(1));
  ASSERT_TRUE(ms2.contains(1));
}

TEST(multisetTest, moveConstructor) {
  s21::multiset<int> ms1 = {1, 2, 3};
  s21::multiset<int> ms2 = std::move(ms1);

  ASSERT_EQ(ms2.size(), 3);
  ASSERT_TRUE(ms2.contains(1));
  ASSERT_TRUE(ms2.contains(2));
  ASSERT_TRUE(ms2.contains(3));
  ASSERT_TRUE(ms1.empty());  // ms1 should be empty after move
}

TEST(multisetTest, insert) {
  s21::multiset<int> ms;
  ms.insert(1);
  ms.insert(2);
  ms.insert(2);

  ASSERT_EQ(ms.size(), 3);  // One insertion of 2 happens twice
  ASSERT_TRUE(ms.contains(1));
  ASSERT_TRUE(ms.contains(2));
}

TEST(multisetTest, erase) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};
  auto it = ms.find(3);
  ms.erase(it);

  ASSERT_EQ(ms.size(), 4);  // One occurrence of 3 is removed
  ASSERT_TRUE(ms.contains(3));
}

TEST(multisetTest, clear) {
  s21::multiset<int> ms = {1, 2, 3};
  ms.clear();

  ASSERT_TRUE(ms.empty());
  ASSERT_EQ(ms.size(), 0);
}

TEST(multisetTest, sizeAndMaxSize) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  ASSERT_EQ(ms.size(), 5);
  ASSERT_GT(ms.max_size(),
            ms.size());  // max_size() should be greater than current size
}

TEST(multisetTest, contains) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  ASSERT_TRUE(ms.contains(3));   // contains key 3
  ASSERT_FALSE(ms.contains(5));  // does not contain key 5
}

TEST(multisetTest, count) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  ASSERT_EQ(ms.count(3), 2);  // There are two occurrences of key 3
  ASSERT_EQ(ms.count(5), 0);  // Key 5 does not exist
}

TEST(multisetTest, find) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  auto it = ms.find(3);
  ASSERT_NE(it, ms.end());  // Should find the first occurrence of 3

  it = ms.find(5);
  ASSERT_EQ(it, ms.end());  // Should not find 5
}

TEST(multisetTest, equalRange) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  auto range = ms.equal_range(3);
  ASSERT_EQ(range.first != ms.end(), true);
  ASSERT_EQ(range.second != ms.end(), true);
}

TEST(multisetTest, lowerBound) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  auto it = ms.lower_bound(3);
  ASSERT_EQ(*it, 3);  // The first element not less than 3 is 3
}

TEST(multisetTest, upperBound) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  auto it = ms.upper_bound(3);
  ASSERT_EQ(*it, 4);  // The first element greater than 3 is 4
}

TEST(multisetTest, swap) {
  s21::multiset<int> ms1 = {1, 2, 3};
  s21::multiset<int> ms2 = {4, 5, 6};

  ms1.swap(ms2);

  ASSERT_EQ(ms1.size(), 3);
  ASSERT_EQ(ms2.size(), 3);
  ASSERT_TRUE(ms1.contains(4));
  ASSERT_TRUE(ms2.contains(1));
}

TEST(multisetTest, merge) {
  s21::multiset<int> ms1 = {1, 2, 3};
  s21::multiset<int> ms2 = {3, 4, 5};

  ms1.merge(ms2);

  ASSERT_EQ(ms1.size(), 6);  // 1, 2, 3, 3, 4, 5
  ASSERT_TRUE(ms1.contains(1));
  ASSERT_TRUE(ms1.contains(2));
  ASSERT_TRUE(ms1.contains(3));
  ASSERT_TRUE(ms1.contains(4));
  ASSERT_TRUE(ms1.contains(5));

  ASSERT_TRUE(ms2.empty());
}

TEST(multisetTest, iterator) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  auto it = ms.begin();
  ASSERT_EQ(*it, 1);

  ++it;
  ASSERT_EQ(*it, 2);

  ++it;
  ASSERT_EQ(*it, 3);
}

TEST(multisetTest, constIterator) {
  s21::multiset<int> ms = {1, 2, 3, 3, 4};

  auto it = ms.begin();
  ASSERT_EQ(*it, 1);

  ++it;
  ASSERT_EQ(*it, 2);

  ++it;
  ASSERT_EQ(*it, 3);
}
TEST(multisetTest, iteratorPreIncrement) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4, 5};
  auto it = s21_multiset.begin();

  ++it;
  ASSERT_EQ(*it, 2);

  ++it;
  ASSERT_EQ(*it, 3);

  ++it;
  ASSERT_EQ(*it, 4);

  ++it;
  ASSERT_EQ(*it, 5);

  ++it;
  ASSERT_EQ(it, s21_multiset.end());
}

TEST(multisetTest, iteratorPostIncrement) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4, 5};
  auto it = s21_multiset.begin();

  auto tmp = it++;
  ASSERT_EQ(*tmp, 1);
  ASSERT_EQ(*it, 2);

  tmp = it++;
  ASSERT_EQ(*tmp, 2);
  ASSERT_EQ(*it, 3);
}

TEST(multisetTest, iteratorPreDecrement) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4, 5};
  auto it = s21_multiset.end();
  --it;
  ASSERT_EQ(*it, 5);

  --it;
  ASSERT_EQ(*it, 4);

  --it;
  ASSERT_EQ(*it, 3);

  --it;
  ASSERT_EQ(*it, 2);

  --it;
  ASSERT_EQ(*it, 1);
}

TEST(multisetTest, iteratorPostDecrement) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4, 5};
  auto s21_it = s21_multiset.end();

  std::multiset<int> std_multiset = {1, 2, 3, 4, 5};
  auto std_it = std_multiset.end();

  --s21_it;
  --std_it;

  auto s21_tmp = s21_it--;
  auto std_tmp = std_it--;
  ASSERT_EQ(*s21_tmp, *std_tmp);
  ASSERT_EQ(*s21_it, *std_it);

  s21_tmp = s21_it--;
  std_tmp = std_it--;
  ASSERT_EQ(*s21_tmp, *std_tmp);
  ASSERT_EQ(*s21_it, *std_it);
}

TEST(multisetTest, iteratorIncrementPastEnd) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4, 5};
  auto it = s21_multiset.end();

  EXPECT_THROW({ ++it; }, std::runtime_error);
}

TEST(multisetTest, iteratorDecrementPastBegin) {
  s21::multiset<int> s21_multiset = {1, 2, 3, 4, 5};
  auto it = s21_multiset.begin();

  EXPECT_THROW({ --it; }, std::runtime_error);
}

TEST(multisetTest, insertManyBasic) {
  s21::multiset<int> test_multiset;

  auto results = test_multiset.insert_many(1, 2, 3, 3, 4);

  EXPECT_EQ(test_multiset.size(),
            5);  // All values, including duplicates, are stored

  EXPECT_EQ(test_multiset.count(3), 2);  // Duplicate is counted
  for (int value : {1, 2, 3, 4}) {
    EXPECT_TRUE(test_multiset.contains(value));
  }
}

TEST(multisetTest, insertManyDuplicatesAllowed) {
  s21::multiset<int> test_multiset;

  auto results = test_multiset.insert_many(1, 1, 1, 2, 2, 3);

  EXPECT_EQ(test_multiset.size(), 6);    // Duplicates allowed
  EXPECT_EQ(test_multiset.count(1), 3);  // Three 1's
  EXPECT_EQ(test_multiset.count(2), 2);  // Two 2's
  EXPECT_EQ(test_multiset.count(3), 1);  // One 3
}

TEST(multisetTest, insertManyKeys) {
  s21::multiset<int> test_multiset;

  auto results = test_multiset.insert_many(1, 1, 2, 3);

  EXPECT_EQ(test_multiset.size(), 4);
  EXPECT_EQ(test_multiset.count(1), 2);

  for (int value : {1, 2, 3}) {
    EXPECT_TRUE(test_multiset.contains(value));
  }
}

TEST(multisetTest, insertManyNoArgs) {
  s21::multiset<int> test_multiset;

  test_multiset.insert(1);
  test_multiset.insert(2);
  test_multiset.insert(3);

  size_t initial_size = test_multiset.size();

  auto results = test_multiset.insert_many();

  EXPECT_EQ(test_multiset.size(), initial_size);
  EXPECT_EQ(test_multiset.count(1), 1);
  EXPECT_EQ(test_multiset.count(2), 1);
  EXPECT_EQ(test_multiset.count(3), 1);
}

TEST(multisetTest, deduceFromInitializerList) {
  std::multiset std_set = {1, 2, 3, 4, 5};
  s21::multiset s21_set = {1, 2, 3, 4, 5};

  EXPECT_EQ(std_set.size(), 5);
  EXPECT_EQ(s21_set.size(), 5);

  auto std_it = std_set.begin();
  auto s21_it = s21_set.begin();

  for (; std_it != std_set.end(); ++std_it, ++s21_it) {
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(multisetTest, emptyInitializerList) {
  std::multiset<int> std_set = {};
  s21::multiset<int> s21_set = {};

  EXPECT_TRUE(std_set.empty());
  EXPECT_TRUE(s21_set.empty());
  EXPECT_EQ(std_set.size(), 0);
  EXPECT_EQ(s21_set.size(), 0);
}

TEST(multisetTest, duplicatesInMultiset) {
  std::multiset std_set = {1, 2, 2, 3, 3, 3};
  s21::multiset s21_set = {1, 2, 2, 3, 3, 3};

  EXPECT_EQ(std_set.size(), 6);  // 1, 2, 2, 3, 3, 3
  EXPECT_EQ(s21_set.size(), 6);  // 1, 2, 2, 3, 3, 3

  auto std_it = std_set.begin();
  auto s21_it = s21_set.begin();

  for (; std_it != std_set.end(); ++std_it, ++s21_it) {
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(multisetTest, deduceStringFromInitializerList) {
  std::multiset std_set = {"apple", "banana", "cherry"};
  s21::multiset s21_set = {"apple", "banana", "cherry"};

  EXPECT_EQ(std_set.size(), 3);
  EXPECT_EQ(s21_set.size(), 3);

  auto std_it = std_set.begin();
  auto s21_it = s21_set.begin();

  for (; std_it != std_set.end(); ++std_it, ++s21_it) {
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(multisetTest, iteratorsValidity) {
  std::multiset<int> std_multiset = {1, 2, 2, 3};
  std::vector<int> std_vector(std_multiset.begin(), std_multiset.end());

  ASSERT_EQ(std_vector.size(), 4);
  ASSERT_EQ(std_vector[0], 1);
  ASSERT_EQ(std_vector[1], 2);
  ASSERT_EQ(std_vector[2], 2);
  ASSERT_EQ(std_vector[3], 3);
}

void AddMultisetTests() {}
