#ifndef _S21_STACK_H
#define _S21_STACK_H 1
#pragma once
#include "s21_containers_common.h"

namespace s21 {

template <typename T>
using StackNode = QueueNode<T>;

template <typename T, bool is_const = false>
using StackIterator_base = QueueIterator_base<T, is_const>;

template <typename T>
class stack : public s21_sequence_container {
 public:
  using value_type = T;   // the template parameter T
  using reference = T &;  // defines the type of the reference to an element
  using const_reference =
      const T &;             // defines the type of the constant reference
  using size_type = size_t;  // defines the type of the container size (standard
                             // type is size_t)
  using iterator = StackIterator_base<T, false>;
  using const_iterator = StackIterator_base<T, true>;
  using node_type = StackNode<T>;

 private:
  size_type size_ = 0;
  node_type *front_ = nullptr;

 public:
  // default constructor, creates empty stack
  stack() = default;

  // initializer list constructor, creates stack initizialized using
  // std::initializer_list
  stack(std::initializer_list<value_type> const &items) {
    for (const auto &item : items) {
      push(item);
    }
  }

  // copy constructor
  stack(const stack &s) : stack() { *this = s; };
  // move constructor
  stack(stack &&s) : stack() { *this = std::forward<stack<T>>(s); };

  // destructor
  ~stack() {
    while (!empty()) pop();
  }

  // assignment operator overload for copying object
  stack &operator=(const stack &s) {
    if (this != &s) {
      while (size() > s.size()) {
        pop();
      }
      iterator sit(s.front_);
      iterator it_end(nullptr);
      while (size() < s.size()) {
        this->push(*sit);
      }
      iterator it(front_);
      while (it != it_end && sit != it_end) {
        *it = *sit;
        ++it;
        ++sit;
      }
    }
    return *this;
  }
  // assignment operator overload for moving object
  stack &operator=(stack &&s) {
    swap(s);
    return *this;
  }

  /*Stack Element access*/
  // access the top element
  const_reference top() {
    if (empty()) throw std::out_of_range("stack: stack is empty");
    return front_->getData();
  }

  /*Stack Capacity*/
  // checks whether the container is empty
  bool empty() const { return (size() == 0); }
  // returns the number of elements
  size_type size() const { return size_; }

  /*Stack Modifiers*/
  // inserts an element at the top
  void push(const_reference value) {
    front_ = new node_type(value, front_);
    size_++;
  }
  // removes the first element
  void pop() {
    if (empty()) throw std::out_of_range("stack: stack is empty");
    auto temp = front_;
    front_ = front_->getNext();
    delete temp;
    --size_;
  }
  // swaps the contents
  void swap(stack &other) {
    std::swap(front_, other.front_);
    std::swap(size_, other.size_);
  }

  template <class... Args>
  void insert_many_back(Args &&...args) {
    auto last = front_;
    while (size() && last->getNext() != nullptr) last = last->getNext();
    for (auto &item : {args...}) {
      node_type *new_node = new node_type(item);
      if (last != nullptr)
        new_node->splice_after(last);
      else
        front_ = new_node;
      last = new_node;
      ++size_;
    }
  }
};
};  // namespace s21

#endif  // S21_STACK_H