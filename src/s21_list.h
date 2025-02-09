#ifndef _S21_LIST_H
#define _S21_LIST_H 1
#pragma once
#include "s21_containers_common.h"

namespace s21 {

struct listNode_base {
 protected:
  listNode_base *next, *prev;

 public:
  listNode_base() : next(myBase()), prev(myBase()) {}

  void swapNeighbors() noexcept { std::swap(next, prev); }

  void swap(listNode_base &other) noexcept {
    std::swap(*this, other);
    next->prev = this;
    prev->next = this;
    other.next->prev = &other;
    other.prev->next = &other;
  }
  // перенос ноды перед pos можно в другой лист
  void rebase(listNode_base *pos) noexcept {
    unhook();
    hook(pos);
  }

  listNode_base *getNext() const noexcept { return this->next; }

  listNode_base *getPrev() const noexcept { return this->prev; }

 protected:
  listNode_base *myBase() noexcept { return this; }
  // Hooks the node into the list before the node pointed to by parent.
  // \param[in] pos The node before which the node is inserted.
  // \pre parent != nullptr
  // \post The node is inserted before the node pointed to by parent.
  // The node is not inserted if parent is nullptr.
  // \note The node is not inserted if parent is nullptr.
  void hook(listNode_base *pos) noexcept {
    if (pos == nullptr || this == pos) return;
    next = pos->myBase();
    prev = pos->prev;
    pos->prev->next = myBase();
    pos->prev = myBase();
  }
  // \brief Unhooks the node from the list.
  // \details The node is not inserted after calling this method.
  // \pre The node is inserted into a list.
  // \post The node is not inserted into a list.
  void unhook() noexcept {
    if (next != myBase() && next->prev == myBase()) next->prev = prev;
    if (prev != myBase() && prev->next == myBase()) prev->next = next;
  }
};

template <typename T>
class listNode : public listNode_base {
  T data_;

 public:
  listNode() {
    this->next = myBase();
    this->prev = myBase();
  }
  /*Специальный конструктор сразу со вставкой*/
  listNode(const T &d, listNode_base *pos) : data_(d) { hook(pos); }

  ~listNode() { unhook(); }

  T *pointer() { return &data_; }
  const T *pointer() const { return &data_; }
  T &data() { return data_; }
  const T &data() const { return data_; }
};
// ListIterator Class with const base trick to get const iterator and not const
// classes without copypast
template <typename T, bool is_const = false>
class ListIterator_base {
 public:
  using pointer = std::conditional_t<is_const, const T *, T *>;
  using reference = std::conditional_t<is_const, const T &, T &>;
  using iterator = ListIterator_base<T, false>;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;
  using node_type = listNode<T>;
  using node_base = listNode_base;

 private:
  node_base *node;

 public:
  ListIterator_base() = delete;
  ListIterator_base(node_base *node) : node(node) {}
  ListIterator_base(const node_base *node)
      : node(const_cast<node_base *>(node)) {}
  reference operator*() noexcept { return get_node()->data(); }
  pointer operator->() noexcept { return get_node()->pointer(); }
  node_type *get_node() const { return static_cast<node_type *>(node); }

  ListIterator_base &operator++() noexcept {
    node = node->getNext();
    return *this;
  }
  ListIterator_base operator++(int) noexcept {
    node = node->getNext();
    return ListIterator_base{node->getPrev()};
  }
  ListIterator_base &operator--() noexcept {
    node = node->getPrev();
    return *this;
  }
  ListIterator_base operator--(int) noexcept {
    node = node->getPrev();
    return ListIterator_base{node->getNext()};
  }

  bool operator==(const ListIterator_base &other) const noexcept {
    return node == other.node;
  }

  bool operator!=(const ListIterator_base &other) const noexcept {
    return !(node == other.node);
  }

  iterator constcast() const noexcept {
    return iterator(const_cast<listNode_base *>(node));
  }
  operator value_type *() const noexcept { return get_node()->pointer(); }
};

template <typename T>
using ListIterator = ListIterator_base<T, false>;

template <typename T>
using ListConstIterator = ListIterator_base<T, true>;

template <typename T, typename Allocator = std::allocator<listNode<T>>>
class list {
  using a_traits = std::allocator_traits<Allocator>;
  using node_type = listNode<T>;

 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = ListIterator<T>;
  using const_iterator = ListConstIterator<T>;
  using size_type = size_t;

 private:
  listNode_base fake_node;
  size_type size_;
  Allocator alloc;

 public:
  // *List Functions*
  // default constructor, creates empty list 
  list()
      : fake_node(),
        size_(0),
        alloc(a_traits::select_on_container_copy_construction(Allocator())) {};

  // parameterized constructor, creates the list of size n
  list(size_type n) : list() {
    if (n > max_size()) throw std::length_error("list too long");
    for (size_type i = 0; i < n; i++) {
      node_type *new_node = a_traits::allocate(alloc, 1);
      a_traits::construct(alloc, new_node);
      new_node->rebase(&fake_node);
      ++size_;
    }
  };

  // initializer list constructor, creates list
  // initizialized using std::initializer_list<T>
  list(std::initializer_list<value_type> const &items) : list() {
    for (auto it = items.begin(); it != items.end(); it++) push_back(*it);
  };

  // copy constructor
  list(const list &l) : list() { *this = l; };
  // move constructor
  list(list &&l) : list() { *this = l; };
  // destructor
  ~list() { clear(); };
  // assignment operator overload for copying object
  list &operator=(const list &l) {
    if (this != &l) {
      const_iterator l_it = l.begin();
      iterator this_it = begin();
      while (l_it != l.end() && this_it != end()) {
        *this_it = *l_it;
        ++l_it;
        ++this_it;
      }
      while (l_it == l.end() && size() > l.size()) {
        pop_back();
      }
      while (l_it != l.end() && size() < l.size()) {
        push_back(*l_it);
        ++l_it;
        ++this_it;
      }
    }
    return *this;
  };
  // assignment operator overload for moving object
  list &operator=(list &&l) {
    swap(l);
    return *this;
  };
  // *List Element access*
  // access the first element
  const_reference front() {
    if (empty()) throw std::out_of_range("list: list is empty");
    return *(begin());
  };
  // access the last element
  const_reference back() {
    if (empty()) throw std::out_of_range("list: list is empty");
    return *(--end());
  };

  // *List Iterators*
  // returns an iterator to the beginning
  iterator begin() noexcept { return ++iterator(&fake_node); };
  // returns an iterator to the end
  iterator end() noexcept { return iterator(&fake_node); };
  // *List Const Iterators*
  const_iterator cbegin() const noexcept {
    return ++const_iterator(&fake_node);
  }
  // returns an iterator to the beginning
  const_iterator begin() const noexcept { return cbegin(); };
  const_iterator cend() const noexcept { return const_iterator(&fake_node); };
  // returns an iterator to the end
  const_iterator end() const noexcept { return cend(); };

  // *List Capacity*
  // checks whether the container is empty
  bool empty() const noexcept { return size_ == 0; };
  // returns the number of elements
  size_type size() const noexcept { return size_; };
  // returns the maximum possible number of elements
  size_type max_size() const noexcept {
    return static_cast<size_type>((-1) / sizeof(node_type));
  };

  // *List Modifiers*
  // clears the contents
  void clear() {
    while (begin() != end()) erase(begin());
  };
  // inserts element into concrete pos and returns
  // the iterator that points to the new element
  iterator insert(iterator pos, const_reference value) {
    node_type *new_node = a_traits::allocate(alloc, 1);
    a_traits::construct(alloc, new_node, value, pos.get_node());
    size_++;
    return iterator(new_node);
  };
  // erases element at pos
  void erase(iterator pos) noexcept {
    if (pos == end() || empty()) return;
    a_traits::destroy(alloc, pos.get_node());
    a_traits::deallocate(alloc, pos.get_node(), 1);
    size_--;
  }
  // adds an element to the end
  void push_back(const_reference value) { insert(end(), value); }
  // removes the last element
  void pop_back() noexcept {
    if (empty()) return;
    erase(--end());
  }
  // adds an element to the head
  void push_front(const_reference value) { insert(begin(), value); }
  // removes the first element
  void pop_front() noexcept { erase(begin()); }
  // swaps the contents
  void swap(list &other) {
    // меняем местами фейковую ноду, вчсе остальные ноды потянутся за ней.
    fake_node.swap(other.fake_node);
    std::swap(size_, other.size_);
  };
  // merges two sorted lists
  void merge(list &other) {
    if (&fake_node == &other.fake_node) return;
    if (other.empty()) return;
    if (empty()) swap(other);
    for (iterator it = begin(); !other.empty(); ++it) {
      while (!(other.empty()) && (it == end() || *(other.begin()) < *it)) {
        other.begin().get_node()->rebase(it.get_node());
        ++size_;
        --other.size_;
      }
    }
  }
  // transfers elements from list
  // other starting from pos
  void splice(const_iterator pos, list &other) {
    if (&fake_node == &other.fake_node) return;
    if (other.empty()) return;
    while (!other.empty()) {
      other.begin().get_node()->rebase(pos.constcast().get_node());
      ++size_;
      --other.size_;
    }
  }
  // reverses the order of the elements
  void reverse() noexcept {
    // меняем ссылки на предыдущий и следующий элемент местами. Переходим к
    // предыдущему(который был следующим)
    // и т.к. лист закольцован через фейковую ноду - совершаем полный круг пока
    // не упремяс в begin()(изначальный)
    for (iterator it = begin(), eit = end(); it != eit; --it) {
      it.get_node()->swapNeighbors();
    }
    fake_node.swapNeighbors();
  };
  // | removes consecutive duplicate elements
  void unique() noexcept {
    for (auto it = begin(); it != --end() && !empty(); ++it) {
      auto nextit = it;
      ++nextit;
      while (*it == *nextit) erase(nextit++);
    }
  }  //
  void sort() {
    if (size_ < 2) return;
    quick_sort_recursive(begin(), --end());
  }

  // sorts the elements
  // extra functions
  // Inserts new elements into the container directly before `pos`
  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args) {
    for (auto arg : {std::forward<Args>(args)...}) {
      a_traits::construct(alloc, a_traits::allocate(alloc, 1),
                          std::forward<decltype(arg)>(arg),
                          pos.constcast().get_node());
      ++size_;
    }
    return iterator(--pos.constcast());
  }
  // Appends new elements to the end of the container.
  template <typename... Args>
  void insert_many_back(Args &&...args) {
    insert_many(cend(), std::forward<Args>(args)...);
  }
  // Inserts new elements at the beginning of the list.
  // Parameters:
  // Args&&... args - A variadic list of elements to insert.
  template <typename... Args>
  void insert_many_front(Args &&...args) {
    // Use insert_many to insert elements at the beginning.
    insert_many(cbegin(), std::forward<Args>(args)...);
  }

 private:
  /**
   * Делит сегмент списка на две части вокруг опорного элемента:
   * Элементы меньше или равные опорному элементу перемещаются влево
   * а элементы больше опорного перемещаются вправо.
   *
   * @param low Итератор указывающий на первый элемент сегмента списка
   * @param high Итератор указывающий на конец сегмента списка. Он же
   * используется как опорный.
   * @return Итератор, указывающий на опорный элемент.
   */
  iterator partition(iterator low, iterator high) {
    auto pivot = *high;
    auto i = low;
    for (auto j = low; j != high; ++j) {
      if (*j <= pivot) {
        std::swap(*i, *j);
        ++i;
      }
    }
    std::swap(*i, *high);
    return i;
  }

  /**
   * @brief Рекурсивно сортирует кусок листа по qsort-алгоритму.
   *
   * @param low Итератор на первый элемент.
   * @param high Итератор на последний элемент.
   *
   * @details
   * Функция делит список на части и сортирует каждую часть отдельно, потом
   * между собой.
   */
  void quick_sort_recursive(iterator low, iterator high) {
    if (low != high && low != ++high) {
      auto pivot = partition(low, --high);
      quick_sort_recursive(low, --pivot);
      quick_sort_recursive(++pivot, high);
    }
  }
};
}  // namespace s21

#endif