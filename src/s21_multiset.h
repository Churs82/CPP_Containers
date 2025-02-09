#ifndef S21_MULTISET_H
#define S21_MULTISET_H

#include "s21_binary_tree.h"

namespace s21 {

template <typename Key>
class multiset : public BinaryTree<Key> {
 public:
  using key_type = Key;
  using value_type = Key;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename BinaryTree<value_type>::iterator;
  using const_iterator = typename BinaryTree<value_type>::const_iterator;
  using size_type = size_t;

  /* Member functions */
  // default constructor
  multiset() {}

  // initializer list constructor
  multiset(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) {
      insert(item);
    }
  }

  // copy constructor
  multiset(const multiset &ms) : BinaryTree<value_type>(ms) {}

  // move constructor
  multiset(multiset &&ms) noexcept : BinaryTree<value_type>(std::move(ms)) {}

  // destructor
  ~multiset() noexcept {}

  // move assignment operator
  multiset &operator=(multiset &&ms) {
    BinaryTree<value_type>::operator=(std::move(ms));
    return *this;
  }

  /* Iterators */
  using BinaryTree<value_type>::begin;
  using BinaryTree<value_type>::end;

  /* Capacity */
  using BinaryTree<value_type>::empty;
  using BinaryTree<value_type>::size;
  using BinaryTree<value_type>::max_size;

  /* Modifiers */
  using BinaryTree<value_type>::clear;

  iterator insert(const value_type &value) {
    return BinaryTree<value_type>::insert(value, true).first;
  }

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> results;
    if constexpr (sizeof...(args) == 0) return results;

    (results.push_back(
         BinaryTree<value_type>::insert(std::forward<Args>(args), true)),
     ...);
    return results;
  }

  using BinaryTree<value_type>::erase;

  void swap(multiset &other) noexcept { std::swap(*this, other); }

  void merge(multiset &other) { BinaryTree<value_type>::merge(other, true); }

  /* Lookup */
  size_type count(const Key &key) {
    auto range = equal_range(key);
    size_type result = 0;
    for (auto it = range.first; it != range.second; ++it) {
      ++result;
    }
    return result;
  }

  using BinaryTree<value_type>::find;
  using BinaryTree<value_type>::contains;

  std::pair<iterator, iterator> equal_range(const Key &key) {
    return {lower_bound(key), upper_bound(key)};
  }

  iterator lower_bound(const Key &key) {
    iterator result = end();
    for (iterator it = begin(); it != end(); ++it) {
      if (!(*it < key)) {
        result = it;
        break;
      }
    }
    return result;
  }

  iterator upper_bound(const Key &key) {
    iterator result = end();
    for (iterator it = begin(); it != end(); ++it) {
      if (key < *it) {
        result = it;
        break;
      }
    }
    return result;
  }
};

// deduction guide
// m{1, 2, 3};
template <typename T>
multiset(std::initializer_list<T>) -> multiset<T>;

}  // namespace s21

#endif
