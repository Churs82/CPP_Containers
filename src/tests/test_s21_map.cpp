#include "test_s21_containers.h"

#ifdef GCOV
template class s21::map<const int, int>;
template class s21::BinaryTree<std::pair<const int, int>>;
#endif

TEST(testMap, atException) {
  s21::map<int, int> m = {{1, 2}, {2, 3}, {3, 4}};
  ASSERT_THROW(m.at(10), std::out_of_range);
  ASSERT_THROW(m.at(-1), std::out_of_range);
}

TEST(testMap, insert) {
  s21::map<int, int> m = {{1, 2}, {2, 3}, {3, 4}};
  auto [it, is_inserted] = m.insert({4, 5});
  ASSERT_TRUE(is_inserted);
  ASSERT_EQ(it->first, 4);
  ASSERT_EQ(it->second, 5);
  auto [it2, is_inserted2] = m.insert({4, 6});
  ASSERT_FALSE(is_inserted2);
  ASSERT_EQ(it2->first, 4);
  ASSERT_EQ(it2->second, 5);
}

TEST(testMap, moveAssignment) {
  s21::map<int, int> m1 = {{1, 2}, {2, 3}, {3, 4}};
  s21::map<int, int> m2;

  m2 = std::move(m1);

  ASSERT_TRUE(m1.empty());
  ASSERT_EQ(m2.size(), 3);
  ASSERT_EQ(m2.at(1), 2);
  ASSERT_EQ(m2.at(2), 3);
  ASSERT_EQ(m2.at(3), 4);
}

TEST(testMap, copyConstructor) {
  s21::map<int, int> m1 = {{1, 2}, {2, 3}, {3, 4}};
  s21::map<int, int> m2(m1);

  ASSERT_EQ(m1.size(), m2.size());
  ASSERT_EQ(m2.at(1), 2);
  ASSERT_EQ(m2.at(2), 3);
  ASSERT_EQ(m2.at(3), 4);
}

TEST(testMap, emptyTreeSize) {
  s21::map<int, int> m;
  ASSERT_EQ(m.size(), 0);
  ASSERT_TRUE(m.empty());
}

TEST(testMap, insertSingleElement) {
  s21::map<int, int> m;
  m.insert(1, 10);
  ASSERT_EQ(m.size(), 1);
  ASSERT_FALSE(m.empty());
  ASSERT_EQ(m.at(1), 10);
}

TEST(testMap, insertMultipleElements) {
  s21::map<int, int> m;
  m.insert(1, 10);
  m.insert(2, 20);
  m.insert(3, 30);
  ASSERT_EQ(m.size(), 3);
  ASSERT_EQ(m.at(1), 10);
  ASSERT_EQ(m.at(2), 20);
  ASSERT_EQ(m.at(3), 30);
}

TEST(testMap, eraseElement) {
  s21::map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
  ASSERT_EQ(m.size(), 3);
  auto it = m.find(2);
  m.erase(it);
  ASSERT_EQ(m.size(), 2);
  ASSERT_THROW(m.at(2), std::out_of_range);
}

TEST(testMap, clearTree) {
  s21::map<int, int> m = {{1, 10}, {2, 20}, {3, 30}};
  ASSERT_EQ(m.size(), 3);
  m.clear();
  ASSERT_EQ(m.size(), 0);
  ASSERT_TRUE(m.empty());
}

TEST(testMap, insertDuplicateKeys) {
  s21::map<int, int> m;
  m.insert(1, 10);
  auto [it, success] = m.insert(1, 20);
  ASSERT_FALSE(success);
  ASSERT_EQ(m.size(), 1);
  ASSERT_EQ(m.at(1), 10);  // Value should remain the same
}

TEST(testMap, sizeAfterSequentialOperations) {
  s21::map<int, int> m;
  ASSERT_EQ(m.size(), 0);
  m.insert(1, 10);
  ASSERT_EQ(m.size(), 1);
  m.insert(2, 20);
  ASSERT_EQ(m.size(), 2);
  m.erase(m.find(1));
  ASSERT_EQ(m.size(), 1);
  m.clear();
  ASSERT_EQ(m.size(), 0);
}

TEST(testMap, findOnEmptyTree) {
  s21::map<int, int> m;
  ASSERT_EQ(m.find(1), m.end());
}

TEST(testMap, eraseNonExistentElement) {
  s21::map<int, int> m = {{1, 10}, {2, 20}};
  auto it = m.find(3);
  ASSERT_EQ(it, m.end());
  ASSERT_THROW(m.erase(it), std::runtime_error);  // Cannot erase end iterator
}

TEST(testMap, complexInsertionAndErasure) {
  s21::map<int, int> m;
  for (int i = 1; i <= 100; ++i) {
    m.insert(i, i * 10);
  }
  ASSERT_EQ(m.size(), 100);

  for (int i = 1; i <= 50; ++i) {
    m.erase(m.find(i));
  }
  ASSERT_EQ(m.size(), 50);

  for (int i = 51; i <= 100; ++i) {
    ASSERT_NO_THROW(m.at(i));
  }
}

TEST(testMap, complexInsertionBalancing) {
  s21::map<int, int> m;
  m.insert(10, 100);
  m.insert(20, 200);
  m.insert(30, 300);  // Causes a left rotation at 10
  m.insert(40, 400);
  m.insert(50, 500);  // Causes a left rotation at 30
  m.insert(25, 250);  // Causes a right-left rotation at 20

  ASSERT_EQ(m.size(), 6);

  std::vector<int> keys;
  for (auto it = m.begin(); it != m.end(); ++it) {
    keys.push_back(it->first);
  }
  ASSERT_EQ(keys, (std::vector<int>{10, 20, 25, 30, 40, 50}));
}

TEST(testMap, complexRemovalLeafNodes) {
  s21::map<int, int> m = {
      {10, 100}, {20, 200}, {30, 300}, {40, 400}, {50, 500}};
  m.erase(m.find(10));
  m.erase(m.find(50));

  ASSERT_EQ(m.size(), 3);

  std::vector<int> keys;
  for (auto it = m.begin(); it != m.end(); ++it) {
    keys.push_back(it->first);
  }
  ASSERT_EQ(keys, (std::vector<int>{20, 30, 40}));
}

TEST(testMap, complexRemovalSingleChildNodes) {
  s21::map<int, int> m = {{10, 100}, {20, 200}, {30, 300},
                          {25, 250}, {40, 400}, {50, 500}};
  m.erase(m.find(25));
  m.erase(m.find(50));

  ASSERT_EQ(m.size(), 4);

  std::vector<int> keys;
  for (auto it = m.begin(); it != m.end(); ++it) {
    keys.push_back(it->first);
  }
  ASSERT_EQ(keys, (std::vector<int>{10, 20, 30, 40}));
}

TEST(testMap, complexRemovalTwoChildren) {
  s21::map<int, int> m = {{10, 100}, {20, 200}, {30, 300},
                          {25, 250}, {40, 400}, {50, 500}};

  m.erase(m.find(20));

  ASSERT_EQ(m.size(), 5);

  std::vector<int> keys;
  for (auto it = m.begin(); it != m.end(); ++it) {
    keys.push_back(it->first);
  }
  ASSERT_EQ(keys, (std::vector<int>{10, 25, 30, 40, 50}));
}

TEST(testMap, complexInsertionAndRemovalStressTest) {
  s21::map<int, int> m;

  for (int i = 1; i <= 1000; ++i) {
    m.insert(i, i * 10);
  }
  ASSERT_EQ(m.size(), 1000);

  for (int i = 1; i <= 500; ++i) {
    m.erase(m.find(i));
  }
  ASSERT_EQ(m.size(), 500);

  for (int i = 501; i <= 1000; ++i) {
    ASSERT_NO_THROW(m.at(i));
    ASSERT_EQ(m.at(i), i * 10);
  }
}

TEST(testMap, insertAndRemoveRandomized) {
  s21::map<int, int> m;

  std::vector<int> keys = {42, 23, 16, 8, 15, 4, 108};
  for (int key : keys) {
    m.insert(key, key * 10);
  }

  m.erase(m.find(16));
  m.erase(m.find(8));
  ASSERT_EQ(m.size(), 5);

  std::vector<int> expected_keys = {4, 15, 23, 42, 108};
  std::vector<int> actual_keys;
  for (auto it = m.begin(); it != m.end(); ++it) {
    actual_keys.push_back(it->first);
  }
  ASSERT_EQ(actual_keys, expected_keys);
}

TEST(testMap, mergeWithNonOverlappingKeys) {
  s21::map<int, int> m1 = {{1, 10}, {2, 20}};
  s21::map<int, int> m2 = {{3, 30}, {4, 40}};

  m1.merge(m2);

  ASSERT_EQ(m1.size(), 4);
  ASSERT_TRUE(m2.empty());

  ASSERT_EQ(m1.at(1), 10);
  ASSERT_EQ(m1.at(3), 30);
}

TEST(testMap, mergeWithOverlappingKeys) {
  s21::map<int, int> m1 = {{1, 10}, {2, 20}};
  s21::map<int, int> m2 = {{2, 99}, {3, 30}};

  m1.merge(m2);

  ASSERT_EQ(m1.size(), 3);
  ASSERT_EQ(m2.size(), 1);

  ASSERT_EQ(m1.at(2), 20);
  ASSERT_EQ(m1.at(3), 30);
}

TEST(testMap, defaultConstructor) {
  s21::map<int, std::string> m;

  ASSERT_TRUE(m.empty());
  ASSERT_EQ(m.size(), 0);
}

TEST(testMap, initializerListConstructor) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};

  ASSERT_FALSE(m.empty());
  ASSERT_EQ(m.size(), 3);
  ASSERT_EQ(m.at(1), "one");
  ASSERT_EQ(m.at(2), "two");
  ASSERT_EQ(m.at(3), "three");
}

TEST(testMap, copyConstructor2) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2 = m1;

  ASSERT_EQ(m2.size(), 2);
  ASSERT_EQ(m2.at(1), "one");
  ASSERT_EQ(m2.at(2), "two");

  m2.at(1) = "uno";
  ASSERT_EQ(m1.at(1), "one");
  ASSERT_EQ(m2.at(1), "uno");
}

TEST(testMap, moveConstructor) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2 = std::move(m1);

  ASSERT_TRUE(m1.empty());
  ASSERT_EQ(m2.size(), 2);
  ASSERT_EQ(m2.at(1), "one");
  ASSERT_EQ(m2.at(2), "two");
}

TEST(testMap, bracketOperator) {
  s21::map<int, std::string> m;

  m[1] = "one";
  ASSERT_EQ(m[1], "one");

  m[1] = "uno";
  ASSERT_EQ(m[1], "uno");

  m[2] = "two";
  ASSERT_EQ(m[2], "two");
}

TEST(testMap, iterators) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}, {3, "three"}};

  int count = 0;
  for (auto it = m.begin(); it != m.end(); ++it) {
    count++;
    if (it->first == 1) {
      ASSERT_EQ(it->second, "one");
    } else if (it->first == 2) {
      ASSERT_EQ(it->second, "two");
    } else if (it->first == 3) {
      ASSERT_EQ(it->second, "three");
    }
  }

  ASSERT_EQ(count, 3);
}

TEST(testMap, empty) {
  s21::map<int, std::string> m1;
  s21::map<int, std::string> m2 = {{1, "one"}};

  ASSERT_TRUE(m1.empty());
  ASSERT_FALSE(m2.empty());
}

TEST(testMap, maxSize) {
  s21::map<int, std::string> m;

  ASSERT_GT(m.max_size(), 0);
}

TEST(testMap, contains) {
  s21::map<int, std::string> m = {{1, "one"}, {2, "two"}};

  ASSERT_TRUE(m.contains(1));
  ASSERT_TRUE(m.contains(2));
  ASSERT_FALSE(m.contains(3));
}

TEST(testMap, insertOrAssign) {
  s21::map<int, std::string> m;

  m.insert_or_assign(1, "one");
  ASSERT_EQ(m.at(1), "one");

  m.insert_or_assign(1, "uno");
  ASSERT_EQ(m.at(1), "uno");
}

TEST(testMap, swap) {
  s21::map<int, std::string> m1 = {{1, "one"}, {2, "two"}};
  s21::map<int, std::string> m2 = {{3, "three"}, {4, "four"}};

  m1.swap(m2);

  ASSERT_EQ(m1.size(), 2);
  ASSERT_EQ(m1.at(3), "three");
  ASSERT_EQ(m1.at(4), "four");

  ASSERT_EQ(m2.size(), 2);
  ASSERT_EQ(m2.at(1), "one");
  ASSERT_EQ(m2.at(2), "two");
}

TEST(testMap, merge) {
  s21::map<int, int> m1 = {{1, 10}, {2, 20}};
  s21::map<int, int> m2 = {{2, 99}, {3, 30}};

  m1.merge(m2);

  ASSERT_EQ(m1.size(), 3);
  ASSERT_EQ(m2.size(), 1);

  ASSERT_EQ(m1.at(2), 20);
  ASSERT_EQ(m1.at(3), 30);
}

TEST(testMap, floatKeyIntValue) {
  s21::map<float, int> m;

  m[3.14f] = 100;
  m[2.718f] = 200;

  ASSERT_EQ(m.at(3.14f), 100);
  ASSERT_EQ(m.at(2.718f), 200);
  ASSERT_EQ(m.size(), 2);

  s21::map<float, int> empty_map;
  ASSERT_TRUE(empty_map.empty());
  ASSERT_FALSE(m.empty());
}

TEST(testMap, stringKeyFloatValue) {
  s21::map<std::string, float> m;

  m["pi"] = 3.14159f;
  m["e"] = 2.71828f;

  ASSERT_EQ(m.at("pi"), 3.14159f);
  ASSERT_EQ(m.at("e"), 2.71828f);
  ASSERT_EQ(m.size(), 2);

  try {
    m.at("golden_ratio");
    FAIL() << "Expected std::out_of_range exception";
  } catch (const std::out_of_range& e) {
    ASSERT_STREQ(e.what(), "Key not found");
  } catch (...) {
    FAIL() << "Expected std::out_of_range exception";
  }
}

TEST(testMap, stringKeyStringValue) {
  s21::map<std::string, std::string> m;

  m["apple"] = "red";
  m["banana"] = "yellow";

  ASSERT_EQ(m.at("apple"), "red");
  ASSERT_EQ(m.at("banana"), "yellow");
  ASSERT_EQ(m.size(), 2);

  ASSERT_TRUE(m.contains("apple"));
  ASSERT_TRUE(m.contains("banana"));
  ASSERT_FALSE(m.contains("cherry"));
}

TEST(testMap, intKeyStringValue) {
  s21::map<int, std::string> m;

  m[1] = "one";
  m[2] = "two";

  ASSERT_EQ(m.at(1), "one");
  ASSERT_EQ(m.at(2), "two");
  ASSERT_EQ(m.size(), 2);

  m.insert_or_assign(1, "uno");
  ASSERT_EQ(m.at(1), "uno");
}

TEST(testMap, floatKeyFloatValue) {
  s21::map<float, float> m;

  m[1.23f] = 4.56f;
  m[7.89f] = 0.12f;

  ASSERT_EQ(m.at(1.23f), 4.56f);
  ASSERT_EQ(m.at(7.89f), 0.12f);

  ASSERT_EQ(m.size(), 2);
}

TEST(testMap, insertManyBasic) {
  s21::map<int, std::string> test_map;

  auto results =
      test_map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                           std::make_pair(3, "three"));

  EXPECT_EQ(test_map.size(), 3);
  EXPECT_EQ(test_map[1], "one");
  EXPECT_EQ(test_map[2], "two");
  EXPECT_EQ(test_map[3], "three");

  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
  }
}

TEST(testMap, insertManyDuplicates) {
  s21::map<int, std::string> test_map;

  auto results =
      test_map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                           std::make_pair(1, "duplicate"));

  EXPECT_EQ(test_map.size(), 2);  // Duplicate key should not increase size
  EXPECT_EQ(test_map[1], "one");  // Original value remains unchanged
  EXPECT_EQ(test_map[2], "two");

  EXPECT_FALSE(
      results.back().second);  // Last insertion (duplicate) should fail
}

TEST(testMap, insertManyWithConversion) {
  s21::map<int, std::string> test_map;

  auto results =
      test_map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                           std::make_pair(3, "three"));

  EXPECT_EQ(test_map.size(), 3);
  EXPECT_EQ(test_map[1], "one");
  EXPECT_EQ(test_map[2], "two");
  EXPECT_EQ(test_map[3], "three");

  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
  }
}

TEST(testMap, insertManyPairs) {
  s21::map<int, std::string> test_map;

  auto results =
      test_map.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                           std::make_pair(3, "three"));

  EXPECT_EQ(test_map.size(), 3);
  EXPECT_EQ(test_map[1], "one");
  EXPECT_EQ(test_map[2], "two");
  EXPECT_EQ(test_map[3], "three");

  for (const auto& result : results) {
    EXPECT_TRUE(result.second);
  }
}

TEST(testMap, insertManyNoArgs) {
  s21::map<int, std::string> test_map;

  test_map.insert({1, "one"});
  test_map.insert({2, "two"});
  test_map.insert({3, "three"});

  size_t initial_size = test_map.size();
  auto results = test_map.insert_many();
  EXPECT_EQ(test_map.size(), initial_size);
}

TEST(testMap, deduceFromInitializerList) {
  std::map std_map = {std::pair{1, 2}, {3, 4}, {5, 6}};
  s21::map s21_map = {std::pair{1, 2}, {3, 4}, {5, 6}};

  EXPECT_EQ(std_map.size(), 3);
  EXPECT_EQ(s21_map.size(), 3);

  EXPECT_EQ(std_map[1], 2);
  EXPECT_EQ(s21_map[1], 2);

  EXPECT_EQ(std_map[3], 4);
  EXPECT_EQ(s21_map[3], 4);

  EXPECT_EQ(std_map[5], 6);
  EXPECT_EQ(s21_map[5], 6);
}

TEST(testMap, emptyInitializerList) {
  std::map<int, int> std_map = {};
  s21::map<int, int> s21_map = {};

  EXPECT_TRUE(std_map.empty());
  EXPECT_TRUE(s21_map.empty());

  EXPECT_EQ(std_map.size(), 0);
  EXPECT_EQ(s21_map.size(), 0);
}

TEST(testMap, deduceWithStringKeys) {
  std::map std_map = {std::pair{"a", 1}, {"b", 2}, {"c", 3}};
  s21::map s21_map = {std::pair{"a", 1}, {"b", 2}, {"c", 3}};

  EXPECT_EQ(std_map.size(), 3);
  EXPECT_EQ(s21_map.size(), 3);

  EXPECT_EQ(std_map["a"], 1);
  EXPECT_EQ(s21_map["a"], 1);

  EXPECT_EQ(std_map["b"], 2);
  EXPECT_EQ(s21_map["b"], 2);

  EXPECT_EQ(std_map["c"], 3);
  EXPECT_EQ(s21_map["c"], 3);
}

TEST(testMap, iteratorsValidity) {
  s21::map<int, std::string> s21_map = {
      {1, "one"},
      {2, "two"},
      {3, "three"},
  };

  std::vector<std::pair<const int, std::string>> std_vector(s21_map.begin(),
                                                            s21_map.end());

  ASSERT_EQ(std_vector.size(), 3);

  ASSERT_EQ(std_vector[0].first, 1);
  ASSERT_EQ(std_vector[0].second, "one");

  ASSERT_EQ(std_vector[1].first, 2);
  ASSERT_EQ(std_vector[1].second, "two");

  ASSERT_EQ(std_vector[2].first, 3);
  ASSERT_EQ(std_vector[2].second, "three");
}

void AddMapTests() {}
