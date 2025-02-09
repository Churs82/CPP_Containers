#include "test_s21_containers.h"

#ifdef GCOV
template class s21::set<int>;
template class s21::BinaryTree<int>;
#endif

TEST(testSet, defaultConstructor) {
  s21::set<int> s;

  ASSERT_TRUE(s.empty());
  ASSERT_EQ(s.size(), 0);
}

TEST(testSet, initializerListConstructor) {
  s21::set<int> s = {1, 2, 3, 4};

  ASSERT_FALSE(s.empty());
  ASSERT_EQ(s.size(), 4);
  ASSERT_TRUE(s.contains(1));
  ASSERT_TRUE(s.contains(2));
  ASSERT_TRUE(s.contains(3));
  ASSERT_TRUE(s.contains(4));
}

TEST(testSet, copyConstructor) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = s1;

  ASSERT_EQ(s2.size(), 3);
  ASSERT_TRUE(s2.contains(1));
  ASSERT_TRUE(s2.contains(2));
  ASSERT_TRUE(s2.contains(3));

  // Modify s2 to ensure it's a deep copy
  s2.insert(4);
  ASSERT_EQ(s2.size(), 4);
  ASSERT_FALSE(s1.contains(4));  // s1 should remain unchanged
}

TEST(testSet, moveConstructor) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = std::move(s1);

  ASSERT_TRUE(s1.empty());  // s1 should be empty after moving
  ASSERT_EQ(s2.size(), 3);
  ASSERT_TRUE(s2.contains(1));
  ASSERT_TRUE(s2.contains(2));
  ASSERT_TRUE(s2.contains(3));
}

TEST(testSet, moveAssignment) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2;

  s2 = std::move(s1);

  ASSERT_TRUE(s1.empty());
  ASSERT_EQ(s2.size(), 3);
  ASSERT_EQ(*s2.find(1), 1);
  ASSERT_EQ(*s2.find(2), 2);
  ASSERT_EQ(*s2.find(3), 3);
}

TEST(testSet, insert1) {
  s21::set<int> s = {1, 2, 3};
  s.insert(4);
  ASSERT_EQ(s.size(), 4);
  ASSERT_EQ(*s.find(4), 4);
  s.insert(4);
  ASSERT_EQ(s.size(), 4);
  ASSERT_EQ(*s.find(4), 4);
}

TEST(testSet, insert2) {
  s21::set<int> s;

  auto result1 = s.insert(1);
  ASSERT_TRUE(result1.second);
  ASSERT_EQ(s.size(), 1);

  auto result2 = s.insert(1);
  ASSERT_FALSE(result2.second);  // Duplicate element 1 should not be inserted
  ASSERT_EQ(s.size(), 1);

  auto result3 = s.insert(2);
  ASSERT_TRUE(result3.second);
  ASSERT_EQ(s.size(), 2);
}

TEST(testSet, erase) {
  s21::set<int> s = {1, 2, 3, 4};

  auto it = s.find(3);
  ASSERT_NE(it, s.end());
  s.erase(it);
  ASSERT_EQ(s.size(), 3);
  ASSERT_FALSE(s.contains(3));

  s.erase(s.find(1));
  ASSERT_EQ(s.size(), 2);
  ASSERT_FALSE(s.contains(1));
}

TEST(testSet, swap) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {4, 5};

  s1.swap(s2);

  ASSERT_EQ(s1.size(), 2);
  ASSERT_EQ(s2.size(), 3);
  ASSERT_TRUE(s1.contains(4));
  ASSERT_TRUE(s1.contains(5));
  ASSERT_TRUE(s2.contains(1));
  ASSERT_TRUE(s2.contains(2));
  ASSERT_TRUE(s2.contains(3));
}

TEST(testSet, sizeAndMaxSize) {
  s21::set<int> s = {1, 2, 3, 3, 4};

  ASSERT_EQ(s.size(), 4);
  ASSERT_GT(s.max_size(),
            s.size());  // max_size() should be greater than current size
}

TEST(testSet, mergeWithNonOverlappingKeys) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {4, 5, 6};

  s1.merge(s2);

  ASSERT_EQ(s1.size(), 6);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));
  ASSERT_TRUE(s1.contains(4));
  ASSERT_TRUE(s1.contains(5));
  ASSERT_TRUE(s1.contains(6));

  ASSERT_EQ(s2.size(), 0);
}

TEST(testSet, mergeWithOverlappingKeys) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {3, 4, 5};

  s1.merge(s2);

  ASSERT_EQ(s1.size(), 5);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));
  ASSERT_TRUE(s1.contains(4));
  ASSERT_TRUE(s1.contains(5));

  ASSERT_EQ(s2.size(), 1);
  ASSERT_TRUE(s2.contains(3));
}

TEST(testSet, mergeWithEmptySet) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {};

  s1.merge(s2);

  ASSERT_EQ(s1.size(), 3);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));

  ASSERT_EQ(s2.size(), 0);
}

TEST(testSet, mergeSelf) {
  s21::set<int> s1 = {1, 2, 3};

  s1.merge(s1);

  ASSERT_EQ(s1.size(), 3);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));
}

TEST(testSet, mergeSubsetIntoSuperset) {
  s21::set<int> s1 = {1, 2, 3, 4, 5};
  s21::set<int> s2 = {3, 4};

  s1.merge(s2);

  ASSERT_EQ(s1.size(), 5);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));
  ASSERT_TRUE(s1.contains(4));
  ASSERT_TRUE(s1.contains(5));

  ASSERT_EQ(s2.size(), 2);
  ASSERT_TRUE(s2.contains(3));
  ASSERT_TRUE(s2.contains(4));
}

TEST(testSet, mergeSupersetIntoSubset) {
  s21::set<int> s1 = {1, 2};
  s21::set<int> s2 = {2, 3, 4, 5};

  s1.merge(s2);

  ASSERT_EQ(s1.size(), 5);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));
  ASSERT_TRUE(s1.contains(4));
  ASSERT_TRUE(s1.contains(5));

  ASSERT_EQ(s2.size(), 1);
  ASSERT_TRUE(s2.contains(2));
}

TEST(testSet, mergeIdenticalSets) {
  s21::set<int> s1 = {1, 2, 3};
  s21::set<int> s2 = {1, 2, 3};

  s1.merge(s2);

  ASSERT_EQ(s1.size(), 3);
  ASSERT_TRUE(s1.contains(1));
  ASSERT_TRUE(s1.contains(2));
  ASSERT_TRUE(s1.contains(3));

  ASSERT_EQ(s2.size(), 3);
  ASSERT_TRUE(s2.contains(1));
  ASSERT_TRUE(s2.contains(2));
  ASSERT_TRUE(s2.contains(3));
}

TEST(testSet, find) {
  s21::set<int> s = {1, 2, 3, 4};

  auto it = s.find(3);
  ASSERT_NE(it, s.end());
  ASSERT_EQ(*it, 3);

  it = s.find(5);
  ASSERT_EQ(it, s.end());
}

TEST(testSet, contains) {
  s21::set<int> s = {1, 2, 3, 4};

  ASSERT_TRUE(s.contains(1));
  ASSERT_TRUE(s.contains(2));
  ASSERT_FALSE(s.contains(5));
}

TEST(testSet, insertManyBasic) {
  s21::set<int> test_set;

  auto results = test_set.insert_many(1, 2, 3, 4);

  EXPECT_EQ(test_set.size(), 4);

  for (int value : {1, 2, 3, 4}) {
    EXPECT_TRUE(test_set.contains(value));
  }

  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
  }
}

TEST(testSet, insertManyDuplicates) {
  s21::set<int> test_set;

  auto results = test_set.insert_many(1, 2, 1, 3, 3);

  EXPECT_EQ(test_set.size(), 3);

  for (int value : {1, 2, 3}) {
    EXPECT_TRUE(test_set.contains(value));
  }

  EXPECT_FALSE(results[2].second);  // Duplicate 1
  EXPECT_FALSE(results[4].second);  // Duplicate 3
}

TEST(testSet, insertManyKeys) {
  s21::set<int> test_set;

  auto results = test_set.insert_many(1, 2, 3, 4);

  EXPECT_EQ(test_set.size(), 4);

  for (int value : {1, 2, 3, 4}) {
    EXPECT_TRUE(test_set.contains(value));
  }

  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
  }
}

TEST(testSet, insertManyNoArgs) {
  s21::set<int> test_set;

  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(3);

  size_t initial_size = test_set.size();

  auto results = test_set.insert_many();

  EXPECT_EQ(test_set.size(), initial_size);
  EXPECT_TRUE(test_set.contains(1));
  EXPECT_TRUE(test_set.contains(2));
  EXPECT_TRUE(test_set.contains(3));
  s21::set s{1, 2, 3};
}

TEST(testSet, deduceFromInitializerList) {
  std::set std_set = {1, 2, 3, 4, 5};
  s21::set s21_set = {1, 2, 3, 4, 5};

  EXPECT_EQ(std_set.size(), 5);
  EXPECT_EQ(s21_set.size(), 5);

  auto std_it = std_set.begin();
  auto s21_it = s21_set.begin();

  for (; std_it != std_set.end(); ++std_it, ++s21_it) {
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(testSet, emptyInitializerList) {
  std::set<int> std_set = {};
  s21::set<int> s21_set = {};

  EXPECT_TRUE(std_set.empty());
  EXPECT_TRUE(s21_set.empty());
  EXPECT_EQ(std_set.size(), 0);
  EXPECT_EQ(s21_set.size(), 0);
}

TEST(testSet, noDuplicatesInSet) {
  std::set std_set = {1, 2, 2, 3, 3, 3};
  s21::set s21_set = {1, 2, 2, 3, 3, 3};

  EXPECT_EQ(std_set.size(), 3);  // 1, 2, 3
  EXPECT_EQ(s21_set.size(), 3);  // 1, 2, 3

  auto std_it = std_set.begin();
  auto s21_it = s21_set.begin();

  for (; std_it != std_set.end(); ++std_it, ++s21_it) {
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(testSet, deduceStringFromInitializerList) {
  std::set std_set = {"apple", "banana", "cherry"};
  s21::set s21_set = {"apple", "banana", "cherry"};

  EXPECT_EQ(std_set.size(), 3);
  EXPECT_EQ(s21_set.size(), 3);

  auto std_it = std_set.begin();
  auto s21_it = s21_set.begin();

  for (; std_it != std_set.end(); ++std_it, ++s21_it) {
    EXPECT_EQ(*std_it, *s21_it);
  }
}

TEST(testSet, iteratorsValidity) {
  std::set<int> std_set = {1, 2, 3};
  std::vector<int> std_vector(std_set.begin(), std_set.end());

  ASSERT_EQ(std_vector.size(), 3);
  ASSERT_EQ(std_vector[0], 1);
  ASSERT_EQ(std_vector[1], 2);
  ASSERT_EQ(std_vector[2], 3);
}

void AddSetTests() {}
