#ifndef S21_SET_H
#define S21_SET_H

#include "s21_binary_tree.h"

namespace s21 {

template <typename Key>
class set : public BinaryTree<Key> {
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
  set() {}

  // initializer list constructor
  set(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) {
      insert(item);
    }
  }

  // copy constructor
  set(const set &s) : BinaryTree<value_type>(s) {}

  // move constructor
  set(set &&s) noexcept : BinaryTree<value_type>(std::move(s)) {}

  // destructor
  ~set() noexcept {}

  // move assignment operator
  set &operator=(set &&s) {
    BinaryTree<value_type>::operator=(std::move(s));
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
  using BinaryTree<value_type>::insert;

  template <typename... Args>
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> results;
    if constexpr (sizeof...(args) == 0) return results;

    (results.push_back(insert(std::forward<Args>(args))), ...);
    return results;
  }

  using BinaryTree<value_type>::erase;

  void swap(set &other) noexcept { std::swap(*this, other); }

  void merge(set &other) { BinaryTree<value_type>::merge(other); }

  /* Lookup */
  using BinaryTree<value_type>::find;
  using BinaryTree<value_type>::contains;
};

// deduction guide
// s{1, 2, 3};
template <typename T>
set(std::initializer_list<T>) -> set<T>;

}  // namespace s21

#endif
