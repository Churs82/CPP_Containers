#ifndef S21_CONTAINERS_COMMON_H
#define S21_CONTAINERS_COMMON_H

#include <functional>
#include <iostream>
#include <utility>

namespace s21 {

// Common interface for s21 containers
struct s21_container {
  virtual ~s21_container() {};
};

// Interface for unordered containers(vector, list , stack etc)
struct s21_sequence_container : public s21_container {
  virtual ~s21_sequence_container() {};
};

// Interface for containers adapter
struct s21_container_adapter : public s21_container {
  virtual ~s21_container_adapter() {};
};

};  // namespace s21
#endif  // S21_CONTAINERS_COMMON_H