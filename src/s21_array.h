#ifndef S21_ARRAY_H
#define S21_ARRAY_H

namespace s21 {
template <typename T, size_t N>
class array {
 public:
  using value_type = T;
  using reference = value_type &;
  using const_reference = const value_type &;
  /* Raw pointers (T*) and const T* are valid iterators in C++. The stdlib
   already provides std::iterator_traits for pointers with
   std::random_access_iterator_tag assigned. */
  using iterator = value_type *;
  using const_iterator = const value_type *;
  using size_type = size_t;

  /* Member functions */
  array() noexcept {
    for (size_t i = 0; i < N; ++i) data_[i] = T{};
  }

  array(std::initializer_list<value_type> const &items) {
    size_t i = 0;
    for (auto &item : items) {
      if (i < N) data_[i++] = item;
    }
  }

  array(const array &a) { std::copy(a.begin(), a.end(), begin()); }

  array(array &&a) noexcept { std::move(a.begin(), a.end(), begin()); }

  ~array() noexcept = default;

  array &operator=(array &&a) {
    if (this != &a)  // avoid self-assignment
      std::move(a.begin(), a.end(), begin());

    return *this;
  }

  /* Element access */
  reference at(size_type pos) {
    if (pos >= size()) throw std::out_of_range("Index out of bounds");
    return data_[pos];
  }

  reference operator[](size_type pos) noexcept { return data_[pos]; }

  const_reference front() const noexcept { return data_[0]; }

  const_reference back() const noexcept { return data_[size() - 1]; }

  iterator data() noexcept { return data_; }

  /* Iterators */
  iterator begin() noexcept { return data_; }
  iterator end() noexcept { return data_ + size(); }

  const_iterator begin() const noexcept { return data_; }
  const_iterator end() const noexcept { return data_ + size(); }

  /* Capacity */
  bool empty() const noexcept { return size() == 0; }
  size_type size() const noexcept { return N; }

  size_type max_size() const noexcept {
    return static_cast<size_type>(SIZE_MAX / sizeof(value_type));
  }

  /* Modifiers */
  void swap(array &other) noexcept {
    // only applicable to same-sized arrays
    for (size_type i = 0; i < size(); ++i) std::swap(data_[i], other.data_[i]);
  }

  void fill(const_reference value) { std::fill(begin(), end(), value); }

 private:
  value_type data_[N];
};

/* deduction guide for class template argument deduction (CTAD)
to allow for arr<int, 3> b{1, 2, 3}; and arr b{1, 2, 3}; initializations */

/* Works for initialization from a std::initializer_list (which provides a
single argument, but one that contains all the elements). Deduces the type T
from the type of the elements inside the initializer_list, and N automatically
as the number of elements in the initializer_list, since the size of an
initializer list is a compile-time constant */
template <typename T, size_t N>
array(std::initializer_list<T>) -> array<T, N>;

/* Works for initialization with a list of arguments, and the size is deduced
 * based on the number of those arguments */
template <class T, class... N>
array(T, N...) -> array<T, 1 + sizeof...(N)>;

}  // namespace s21

#endif
