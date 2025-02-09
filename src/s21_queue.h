#ifndef _S21_QUEUE_H
#define _S21_QUEUE_H 1
#pragma once
#include "s21_containers_common.h"

namespace s21 {

template <typename T>
class QueueNode {
  T data;
  QueueNode *next = nullptr;

 public:
  QueueNode(const T &value) : data(value) {}

  QueueNode(QueueNode *prev, const T &value) : data(value) {
    splice_after(prev);
  }
  QueueNode(const T &value, QueueNode *next_node) : data(value) {
    splice_before(next_node);
  }

  QueueNode *getNext() { return next; }
  T &getData() { return data; }
  T *pointer() { return &data; }

  void splice_after(QueueNode *prev) noexcept {
    if (prev != nullptr) prev->next = this;
  }
  void splice_before(QueueNode *next_node) noexcept { next = next_node; }
};

template <typename T, bool is_const = false>
class QueueIterator_base {
 public:
  using pointer = std::conditional_t<is_const, const T *, T *>;
  using reference = std::conditional_t<is_const, const T &, T &>;
  using value_type = T;
  using iterator_category = std::forward_iterator_tag;
  using node_type = QueueNode<T>;

 private:
  node_type *node_;

 public:
  QueueIterator_base() = delete;
  QueueIterator_base(node_type *node) : node_(node) {}
  reference operator*() noexcept { return node_->getData(); }
  pointer operator->() { return node_->pointer(); }

  QueueIterator_base &operator++() {
    if (node_ != nullptr) node_ = node_->getNext();
    return *this;
  }

  QueueIterator_base operator++(int) {
    QueueIterator_base temp = *this;
    ++(*this);
    return temp;
  }

  bool operator==(const QueueIterator_base &other) const {
    return node_ == other.node_;
  }

  bool operator!=(const QueueIterator_base &other) const {
    return !(*this == other);
  }
};

template <typename T>
class queue : public s21_sequence_container {
 public:
  using value_type = T;   // the template parameter T
  using reference = T &;  // defines the type of the reference to an element
  using const_reference =
      const T &;             // defines the type of the constant reference
  using size_type = size_t;  // defines the type of the container size (standard
                             // type is size_t)
  using iterator = QueueIterator_base<T, false>;
  using const_iterator = QueueIterator_base<T, true>;

 private:
  size_type size_ = 0;
  QueueNode<T> *front_ = nullptr;
  QueueNode<T> *back_ = nullptr;

 public:
  // default constructor, creates empty queue
  queue() = default;

  // initializer list constructor, creates queue initizialized using
  // std::initializer_list
  queue(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) {
      push(item);
    }
  }

  // copy constructor
  queue(const queue &q) : queue() { *this = q; };
  // move constructor
  queue(queue &&q) : queue() { *this = std::forward<queue<T>>(q); };

  // destructor
  ~queue() {
    while (!empty()) pop();
  }

  // assignment operator overload for copying object
  queue &operator=(const queue &q) {
    if (this != &q) {
      while (size() > q.size()) {
        pop();
      }
      iterator it(front_);
      iterator qit(q.front_);
      iterator it_end(nullptr);

      while (it != it_end && qit != it_end) {
        *it = *qit;
        ++it;
        ++qit;
      }
      while (size() < q.size()) {
        this->push(*(qit++));
      }
    }
    return *this;
  }
  // assignment operator overload for moving object
  queue &operator=(queue &&q) {
    swap(q);
    return *this;
  }

  /*Queue Element access*/
  // access the first element
  const_reference front() const {
    if (empty()) throw std::out_of_range("queue: queue is empty");
    return front_->getData();
  }
  // access the last element
  const_reference back() const {
    if (empty()) throw std::out_of_range("queue: queue is empty");
    return back_->getData();
  }

  /*Queue Capacity*/
  // checks whether the container is empty
  bool empty() const { return (size() == 0); }
  // returns the number of elements
  size_type size() const { return size_; }

  /*Queue Modifiers*/
  // inserts an element at the end
  void push(const_reference value) {
    if (empty())
      front_ = back_ = new QueueNode(value);
    else
      back_ = new QueueNode(back_, value);
    size_++;
  }
  // removes the first element
  void pop() {
    if (empty()) throw std::out_of_range("queue: queue is empty");
    auto temp = front_;
    front_ = front_->getNext();
    delete temp;
    --size_;
    if (empty()) back_ = nullptr;
  }
  // swaps the contents
  void swap(queue &other) {
    std::swap(front_, other.front_);
    std::swap(back_, other.back_);
    std::swap(size_, other.size_);
  }

  template <class... Args>
  void insert_many_back(Args &&...args) {
    (push(std::forward<Args>(args)), ...);
  }
};
};  // namespace s21

#endif  // S21_QUEUE_H