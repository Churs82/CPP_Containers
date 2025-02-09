#include "test_s21_containers.h"

TEST(arrayTest, DefaultConstructor) {
  s21::array<int, 3> arr;
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 0);
  EXPECT_EQ(arr[1], 0);
  EXPECT_EQ(arr[2], 0);
}

TEST(arrayTest, Constructor) {
  s21::array<int, 3> arr = {1, 2, 3};
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);
}

TEST(arrayTest, CopyConstructor) {
  s21::array<int, 3> arr1 = {1, 2, 3};
  s21::array<int, 3> arr2 = arr1;
  EXPECT_EQ(arr2.size(), 3);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
  EXPECT_EQ(arr2[2], 3);
}

TEST(arrayTest, MoveConstructor) {
  s21::array<int, 3> arr1 = {1, 2, 3};
  s21::array<int, 3> arr2 = std::move(arr1);

  EXPECT_EQ(arr2.size(), 3);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
  EXPECT_EQ(arr2[2], 3);
  EXPECT_EQ(arr1.size(), 3);
}

TEST(arrayTest, destructor) {
  {
    s21::array<float, 3> s21_arr = {1.1f, 2.2f, 3.3f};
    // After the scope ends, it should be destroyed automatically
    // Test with valgrind to ensure no memory leaks
  }
}

TEST(arrayTest, MoveAssignmentOperator) {
  s21::array<int, 3> arr1 = {1, 2, 3};
  s21::array<int, 3> arr2;
  arr2 = std::move(arr1);

  EXPECT_EQ(arr2.size(), 3);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
  EXPECT_EQ(arr2[2], 3);
  EXPECT_EQ(arr1.size(), 3);
}

TEST(arrayTest, At) {
  s21::array<int, 3> arr = {10, 20, 30};
  EXPECT_EQ(arr.at(0), 10);
  EXPECT_EQ(arr.at(1), 20);
  EXPECT_EQ(arr.at(2), 30);

  EXPECT_THROW(arr.at(3), std::out_of_range);
}

TEST(arrayTest, FrontBack) {
  s21::array<int, 3> arr = {10, 20, 30};
  EXPECT_EQ(arr.front(), 10);
  EXPECT_EQ(arr.back(), 30);
}

TEST(arrayTest, data) {
  std::array<float, 3> std_arr = {1.1f, 2.2f, 3.3f};
  s21::array<float, 3> s21_arr = {1.1f, 2.2f, 3.3f};

  for (size_t i = 0; i < std_arr.size(); ++i) ASSERT_EQ(std_arr[i], s21_arr[i]);
}

TEST(arrayTest, Empty) {
  s21::array<int, 3> arr = {10, 20, 30};
  EXPECT_FALSE(arr.empty());

  s21::array<int, 0> emptyArr;
  EXPECT_TRUE(emptyArr.empty());
}

TEST(arrayTest, Swap) {
  s21::array<int, 3> arr1 = {1, 2, 3};
  s21::array<int, 3> arr2 = {4, 5, 6};

  arr1.swap(arr2);

  EXPECT_EQ(arr1[0], 4);
  EXPECT_EQ(arr1[1], 5);
  EXPECT_EQ(arr1[2], 6);

  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
  EXPECT_EQ(arr2[2], 3);
}

TEST(arrayTest, Fill) {
  s21::array<int, 3> arr = {0, 0, 0};
  arr.fill(5);
  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr[1], 5);
  EXPECT_EQ(arr[2], 5);
}

TEST(arrayTest, MaxSize) {
  s21::array<int, 3> arr;
  EXPECT_TRUE(arr.max_size() > arr.size());
}

TEST(arrayTest, deductionGuide) {
  s21::array arr = {1, 2, 3};  // Should deduce array<int, 3>
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 1);
  EXPECT_EQ(arr[1], 2);
  EXPECT_EQ(arr[2], 3);

  // Test with a larger initializer list
  s21::array arr2 = {10, 20, 30, 40};  // Should deduce array<int, 4>
  EXPECT_EQ(arr2.size(), 4);
  EXPECT_EQ(arr2[0], 10);
  EXPECT_EQ(arr2[1], 20);
  EXPECT_EQ(arr2[2], 30);
  EXPECT_EQ(arr2[3], 40);
}

TEST(arrayTest, deductionGuideDifferentTypes) {
  s21::array arr = {'a', 'b', 'c'};  // Should deduce array<char, 3>
  EXPECT_EQ(arr.size(), 3);
  EXPECT_EQ(arr[0], 'a');
  EXPECT_EQ(arr[1], 'b');
  EXPECT_EQ(arr[2], 'c');

  s21::array arr2 = {1.1, 2.2, 3.3};  // Should deduce array<double, 3>
  EXPECT_EQ(arr2.size(), 3);
  EXPECT_EQ(arr2[0], 1.1);
  EXPECT_EQ(arr2[1], 2.2);
  EXPECT_EQ(arr2[2], 3.3);
}

TEST(arrayTest, iteratorsValidity) {
  s21::array<int, 3> s21_array = {1, 2, 3};

  std::vector<int> std_vector(s21_array.begin(), s21_array.end());

  ASSERT_EQ(std_vector.size(), 3);

  ASSERT_EQ(std_vector[0], 1);
  ASSERT_EQ(std_vector[1], 2);
  ASSERT_EQ(std_vector[2], 3);
}

TEST(arrayTest, beginEndMethods) {
  s21::array<int, 3> s21_array = {10, 20, 30};
  std::array<int, 3> std_array = {10, 20, 30};

  auto s21_begin = s21_array.begin();
  auto s21_end = s21_array.end();
  auto std_begin = std_array.begin();
  auto std_end = std_array.end();

  for (size_t i = 0; i < 3; ++i) {
    ASSERT_EQ(*(s21_begin + i), *(std_begin + i));
    ASSERT_EQ(*(s21_end - (3 - i)), *(std_end - (3 - i)));
  }
}

TEST(arrayTest, iteratorsComparisonWithStdArray) {
  s21::array<int, 4> s21_array = {4, 5, 6, 7};
  std::array<int, 4> std_array = {4, 5, 6, 7};

  std::vector<int> std_vector_s21(s21_array.begin(), s21_array.end());
  std::vector<int> std_vector_std(std_array.begin(), std_array.end());

  ASSERT_EQ(std_vector_s21.size(), 4);
  ASSERT_EQ(std_vector_std.size(), 4);

  for (size_t i = 0; i < 4; ++i) {
    ASSERT_EQ(std_vector_s21[i], std_vector_std[i]);
  }
}

TEST(arrayTest, constIteratorsValidity) {
  const s21::array<int, 3> s21_array = {100, 200, 300};

  std::vector<int> std_vector(s21_array.begin(), s21_array.end());

  ASSERT_EQ(std_vector.size(), 3);

  ASSERT_EQ(std_vector[0], 100);
  ASSERT_EQ(std_vector[1], 200);
  ASSERT_EQ(std_vector[2], 300);
}

TEST(arrayTest, iteratorIncrementDecrement) {
  s21::array<int, 5> s21_array = {1, 2, 3, 4, 5};

  auto it = s21_array.begin();

  ++it;
  ASSERT_EQ(*it, 2);

  ++it;
  ASSERT_EQ(*it, 3);

  --it;
  ASSERT_EQ(*it, 2);

  --it;
  ASSERT_EQ(*it, 1);
}

TEST(arrayTest, reverseIterators) {
  s21::array<int, 5> s21_array = {1, 2, 3, 4, 5};

  auto rev_it = s21_array.end();
  --rev_it;
  ASSERT_EQ(*rev_it, 5);

  --rev_it;
  ASSERT_EQ(*rev_it, 4);

  rev_it = s21_array.end();
  rev_it--;
  ASSERT_EQ(*rev_it, 5);

  rev_it--;
  ASSERT_EQ(*rev_it, 4);
}

TEST(arrayTest, iteratorPostDecrement) {
  s21::array<int, 5> s21_array = {1, 2, 3, 4, 5};
  auto s21_it = s21_array.end();

  std::array<int, 5> std_array = {1, 2, 3, 4, 5};
  auto std_it = std_array.end();

  --s21_it;
  --std_it;

  auto s21_tmp = s21_it--;
  auto std_tmp = std_it--;
  ASSERT_EQ(*s21_tmp, *std_tmp);
  ASSERT_EQ(*s21_it, *std_it);
}

void AddArrayTests() {}
