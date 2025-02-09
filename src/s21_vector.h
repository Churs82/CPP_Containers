#ifndef S21_VECTOR_H
#define S21_VECTOR_H
#pragma once
#include "s21_containers_common.h"

namespace s21 {
template <typename T, bool is_const = false>
class VectorIterator_base {
 public:
  using pointer = std::conditional_t<is_const, const T *, T *>;
  using reference = std::conditional_t<is_const, const T &, T &>;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;

 private:
  pointer ptr;

 public:
  VectorIterator_base(pointer ptr) : ptr(ptr) {}
  VectorIterator_base(const VectorIterator_base &) = default;
  VectorIterator_base(VectorIterator_base &&) = default;

  VectorIterator_base &operator=(const VectorIterator_base &) = default;
  reference operator*() { return *ptr; }

  pointer operator->() { return ptr; }
  reference operator[](size_t n) { return *(ptr + n); }

  VectorIterator_base &operator=(VectorIterator_base &&) = default;

  const pointer get_pointer() const { return ptr; }

  VectorIterator_base &operator++() {
    ptr++;
    return *this;
  }

  VectorIterator_base operator++(int) { return VectorIterator_base{ptr++}; }

  VectorIterator_base &operator--() {
    ptr--;
    return *this;
  }

  VectorIterator_base operator--(int) { return VectorIterator_base{ptr--}; }

  VectorIterator_base operator+(size_t n) {
    return VectorIterator_base(ptr + n);
  }

  VectorIterator_base operator-(size_t n) {
    return VectorIterator_base(ptr - n);
  }

  VectorIterator_base &operator+=(size_t n) { return *this = *this + n; }

  VectorIterator_base &operator-=(size_t n) { return *this = *this - n; }

  difference_type operator-(const VectorIterator_base &other) const {
    return ptr - other.ptr;
  }

  bool operator==(const VectorIterator_base &other) const {
    return ptr == other.ptr;
  }

  bool operator>(const VectorIterator_base &other) const noexcept {
    return ptr > other.ptr;
  }

  bool operator<(const VectorIterator_base &other) const noexcept {
    return ptr < other.ptr;
  }

  bool operator!=(const VectorIterator_base &other) const {
    return !(*this == other);
  }
};
template <typename T>
using VectorIterator = VectorIterator_base<T, false>;
template <typename T>
using VectorConstIterator = VectorIterator_base<T, true>;

template <typename T, typename Allocator = std::allocator<T>>
class vector : public s21_sequence_container {
  /* @brief: defines the type of the container size */
  using size_type = std::size_t;
  using a_traits = std::allocator_traits<Allocator>;
  static const size_type grow_factor = 20;

 public:
  /* @brief: defines the type for iterating through the container */
  // using iterator = VectorIterator<T>; // or internal class
  // `VectorIterator<T>`
  using iterator = VectorIterator<T>;
  /* @brief: defines the constant type for iterating through the container */
  // using const_iterator = VectorConstIterator<T>; // or internal class
  // `VectorConstIterator<T>`
  using const_iterator = VectorConstIterator<T>;

  /* @brief: defines the type of an element (T is template parameter) */
  using value_type = T;
  /* @brief: defines the type of the reference to an element */
  using reference = T &;
  /* @brief: defines the type of the constant reference */
  using const_reference = const T &;

 private:
  /*private attributes*/

  Allocator alloc;
  T *arr = nullptr;
  size_type m_size{};
  size_type m_capacity{};

  void allocate(size_type n) {
    if (n == 0) return;
    try {
      alloc = a_traits::select_on_container_copy_construction(alloc);
      T *new_arr = a_traits::allocate(alloc, n);
      arr = new_arr;
      m_capacity = n;
    } catch (...) {
    }
  }

  // parameterized constructor, creates the vector of size n
  vector(size_type n) : alloc(Allocator()) { allocate(advanceCapacity(n)); }

  static size_type advanceCapacity(const size_t n) {
    return static_cast<size_type>(n * grow_factor / 10.0);
  }

 public:
  /*Vector Member functions*/
  /* @brief: основные публичные методы для взаимодействия с классом: */
  // default constructor, creates empty vector
  vector() noexcept : vector(0) {}

  // initializer list constructor, creates vector initizialized using
  // std::initializer_list<T>
  vector(std::initializer_list<value_type> const &items)
      : vector(items.size()) {
    for (auto itt = items.end(), it = items.begin(); it != itt; ++it)
      a_traits::construct(alloc, arr + (m_size++), *it);
  }

  // copy constructor
  vector(const vector &v) {
    allocate(advanceCapacity(v.size()));
    for (auto itt = v.end(), it = v.begin(); it != itt; ++it)
      a_traits::construct(alloc, arr + (m_size++), *it);
  }
  // move constructor
  vector(vector &&v) noexcept
      : alloc(v.alloc), arr(v.arr), m_size(v.m_size), m_capacity(v.m_capacity) {
    v.arr = nullptr;
    v.m_size = v.m_capacity = 0;
  }
  // destructor
  ~vector() {
    if (arr != nullptr) {
      for (size_type sz = size(), i = 0; i < sz; ++i)
        a_traits::destroy(alloc, arr + i);
      a_traits::deallocate(alloc, arr, m_capacity);
    }
  }

  // assignment operator overload for copy (copy and swap)
  vector &operator=(const vector &v) noexcept {
    if (this != &v) {
      auto copy = v;
      swap(copy);
    }
    return *this;
  }

  // assignment operator overload for moving object
  vector &operator=(vector &&v) noexcept {
    if (this != &v) swap(v);
    return *this;
  }

  /*Vector Element access*/
  // access specified element with bounds checking
  reference at(size_type pos) {
    if (pos >= size()) throw std::out_of_range("vector: out of range");
    return *(arr + pos);
  }
  // access specified element
  reference operator[](size_type pos) { return *(arr + pos); }

  // access specified element
  const_reference operator[](size_type pos) const { return *(arr + pos); }

  // access the first element
  const_reference front() const {
    if (empty()) throw std::out_of_range("vector: vektor is empty");
    return *begin();
  }
  // access the last element
  const_reference back() const {
    if (empty()) throw std::out_of_range("vector: vektor is empty");
    return *(end() - 1);
  }
  // direct access to the underlying array
  T *data() noexcept { return arr; }
  /*Vector Iterators*/
  // returns an iterator to the beginning
  iterator begin() noexcept { return iterator{arr}; }
  // returns an iterator to the end
  iterator end() noexcept { return begin() + m_size; }
  // returns an iterator to the beginning
  const_iterator begin() const noexcept { return const_iterator{arr}; }

  // returns an iterator to the beginning
  const_iterator cbegin() const noexcept { return const_iterator{arr}; }
  // returns an iterator to the end
  const_iterator end() const noexcept { return begin() + m_size; }

  // returns an iterator to the end
  const_iterator cend() const noexcept { return begin() + m_size; }

  /*Vector Capacity*/
  // checks whether the container is empty
  bool empty() const noexcept { return begin() == end(); }
  // returns the number of elements
  size_type size() const noexcept { return end() - begin(); }
  // returns the maximum possible number of elements
  size_type max_size() const noexcept {
    return static_cast<size_type>((-1) / sizeof(T));
  };
  // allocate storage of size elements and copies current array elements to a
  // newely allocated array
  void reserve(size_type size) {
    if (size > max_size()) throw std::length_error("vector: vector is too big");

    if (size > capacity()) {
      T *oldarr = arr;
      arr = a_traits::allocate(alloc, size);
      for (size_type sz = this->size(), i = 0; i < sz; ++i)
        arr[i] = std::move(oldarr[i]);
      a_traits::deallocate(alloc, oldarr, m_capacity);
      m_capacity = size;
    }
  }
  // returns the number of elements that can be held in currently allocated
  // storage
  size_type capacity() const noexcept { return m_capacity; }
  // reduces memory usage by freeing unused memory
  void shrink_to_fit() {
    auto new_arr = a_traits::allocate(alloc, m_size);
    for (size_type i = 0; i < m_size; ++i)
      a_traits::construct(alloc, new_arr + i, std::move(arr[i]));
    std::swap(arr, new_arr);
    a_traits::deallocate(alloc, new_arr, m_capacity);
    m_capacity = m_size;
  }

  /*Vector Modifiers*/
  // clears the contents
  void clear() {
    for (size_type sz = size(), i = 0; i < sz; ++i)
      a_traits::destroy(alloc, arr + i);
    m_size = 0;
  }
  // inserts elements into concrete pos and returns the iterator that points
  // to the new element
  iterator insert(iterator pos, const_reference value) {
    return insert_many(const_iterator(pos.get_pointer()), value);
  }
  // erases element at pos
  void erase(iterator pos) {
    const size_type ipos = pos - begin();
    if (ipos >= size() || begin() > pos)
      throw(std::out_of_range("vector: erase pos is out of range"));
    a_traits::destroy(alloc, arr + ipos);
    for (auto sz = size() - 1, i = ipos; i < sz; ++i) {
      a_traits::construct(alloc, arr + i, std::move(arr[i + 1]));
    }
    --m_size;
  }
  // adds an element to the end
  void push_back(const_reference value) { insert_many_back(value); }
  // removes the last element
  void pop_back() { erase(end() - 1); }
  // swaps the contents
  void swap(vector &other) {
    std::swap(arr, other.arr);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
  }
  template <class... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    const size_type ipos = pos - cbegin();
    const size_type count = sizeof...(Args);
    if (ipos > size() || pos < cbegin())
      throw(std::out_of_range("vector: insert pos is out of range"));
    try {
      if (size() + count > capacity()) {
        auto new_arr =
            a_traits::allocate(alloc, advanceCapacity(size() + count));
        // переносим элементы из старого массива с дыркой под новый элемент
        for (size_type i = 0; i < ipos; ++i) {
          a_traits::construct(alloc, new_arr + i, std::move(arr[i]));
        }
        for (size_type i = ipos; i < m_size; ++i) {
          a_traits::construct(alloc, new_arr + i + count, std::move(arr[i]));
        }
        std::swap(new_arr, arr);
        a_traits::deallocate(alloc, new_arr, m_capacity);
        m_capacity = advanceCapacity(size() + count);
      } else {
        for (auto i = m_size + count - 1; i >= ipos + count; --i) {
          a_traits::construct(alloc, arr + i, std::move(arr[i - count]));
        }
      }
      auto insptr = arr + ipos;
      for (auto arg : {std::forward<Args>(args)...}) {
        a_traits::construct(alloc, insptr++, std::forward<decltype(arg)>(arg));
        ++m_size;
      }
    } catch (const std::exception &e) {
      throw std::runtime_error("vector: exception while inserting element: " +
                               std::string(e.what()));
    }
    return begin() + ipos;
  }

  template <class... Args>
  void insert_many_back(Args &&...args) {
    insert_many(this->cend(), std::forward<Args>(args)...);
  }
};

/* Дедукция типа. Создание объекта типа vector<T> без указания типа с помощью
 * нотации {1,2,3,4,5} */
template <typename T>
vector(std::initializer_list<T>) -> vector<T>;

};  // namespace s21
#endif  // S21_VECTOR_H
