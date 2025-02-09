#ifndef S21_MAP_H
#define S21_MAP_H

#include "s21_binary_tree.h"

namespace s21 {

template <typename Key, typename T>
class map : public BinaryTree<std::pair<const Key, T>> {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type &;
  using const_reference = const value_type &;
  using iterator = typename BinaryTree<value_type>::iterator;
  using const_iterator = typename BinaryTree<value_type>::const_iterator;
  using size_type = size_t;

  /* Member functions */
  // default constructor
  map() {}

  // initializer list constructor
  map(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) {
      insert(item.first, item.second);
    }
  }

  // copy constructor
  map(const map &m) : BinaryTree<value_type>(m) {}

  // move constructor
  map(map &&m) noexcept : BinaryTree<value_type>(std::move(m)) {}

  // destructor
  ~map() noexcept {}

  // move assignment operator
  map &operator=(map &&m) {
    BinaryTree<value_type>::operator=(std::move(m));
    return *this;
  }

  /* Element access */
  T &at(const Key &key) {
    auto it = BinaryTree<value_type>::find(key);
    if (it == end()) throw std::out_of_range("Key not found");
    return it->second;
  }

  T &operator[](const Key &key) {
    auto [it, inserted] = insert({key, T()});
    return it->second;
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

  std::pair<iterator, bool> insert(const Key &key, const T &obj) {
    return BinaryTree<value_type>::insert(std::make_pair(key, obj));
  }

  std::pair<iterator, bool> insert_or_assign(const Key &key, const T &obj) {
    auto result = BinaryTree<value_type>::insert(std::make_pair(key, obj));
    if (!result.second)
      result.first->second = obj;  // update the value of the existing key
    return result;
  }

  /* O(m log n) time complexity, where m is the number of elements being
inserted and n is the current number of elements in the container. Each
insertion takes O(log n) time. Each insertion is atomic.*/
  template <typename... Args>  // Parameter pack
  s21::vector<std::pair<iterator, bool>> insert_many(Args &&...args) {
    s21::vector<std::pair<iterator, bool>> results;
    if constexpr (sizeof...(args) == 0) return results;

    /* Fold expression - provides a concise way to apply a binary operator
    (e.g. +, insert() here) to a parameter pack. Syntactic sugar for
    loop/recursion: reduces a list of args into a single result (e.g.
    sum/product, vector here).*/
    (results.push_back(insert(std::forward<Args>(args))), ...);
    return results;
  }

  using BinaryTree<value_type>::erase;

  void swap(map &other) noexcept { std::swap(*this, other); }

  void merge(map &other) { BinaryTree<value_type>::merge(other); }

  /* Lookup */
  using BinaryTree<value_type>::contains;
};

// deduction guide
// m{std::pair{1, "one"s}, {2,"two"s}, {3,"three"s}};
template <typename Key, typename T>
map(std::initializer_list<std::pair<Key, T>>) -> map<const Key, T>;

}  // namespace s21

#endif
