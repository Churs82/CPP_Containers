#ifndef S21_BINARY_TREE_H
#define S21_BINARY_TREE_H

#include "s21_vector.h"

namespace s21 {

/* SFINAE (Substitution Failure Is Not An Error) - implemented using template
specialization combined with std::enable_if */
// primary template
template <typename T, typename Enable = void>
struct KeyType {  // type trait - conditionally alters the type based on whether
                  // T is a std::pair
  using type = T;
};

// specialization
template <typename T>
/* std::enable_if is used to guard the evaluation: prevents the compiler from
   accessing typename T::first_type when T is not a std::pair */
struct KeyType<
    T, std::enable_if_t<std::is_same_v<
           T, std::pair<typename T::first_type, typename T::second_type>>>> {
  using type = typename T::first_type;
};

/*
 * AVL tree with parent references.
 * A self-balancing binary search tree where each node
 * stores a reference to its parent in addition to its children.
 * The balance of the tree is maintained by ensuring that the heights
 * of the subtrees of any node differ by no more than one.
 * Provides logarithmic search, insert, and delete operations.
 */
template <typename T>
class BinaryTree {
 public:
  using value_type = T;
  using Key = typename KeyType<T>::type;

  struct Node {
    value_type data;
    Node* left;
    Node* right;
    Node* parent;
    int height;

    Node(const value_type& value)
        : data(value),
          left(nullptr),
          right(nullptr),
          parent(nullptr),
          height(1) {}
  };

  template <bool is_const = false>
  /* A template class that handles both iterator and const_iterator through
   * std::conditional_t (a template utility in C++ that is used to select one of
   * two types based on a boolean condition at compile-time). As a base for
   * associative containers, is bidirectional (supports operator++ and
   * operator--), read-only (operator* returns const Key&), and preserving
   * order.
   */
  class iterator_base {
    friend class BinaryTree;

   public:
    // used by the standard library to determine the capabilities of an iterator
    using iterator_category = std::bidirectional_iterator_tag;
    using TreeType =
        std::conditional_t<is_const, const BinaryTree<T>, BinaryTree<T>>;

    // iterator traits for the iterator to be compatible with std containers
    using value_type = T;
    using pointer = value_type*;
    using reference = value_type&;
    using difference_type = std::ptrdiff_t;

    iterator_base(Node* node = nullptr, TreeType* tree = nullptr) noexcept
        : node_(node), tree_(tree) {}

    reference operator*() const {
      if (!node_) throw std::runtime_error("Dereferencing end iterator");
      return node_->data;
    }

    pointer operator->() const {
      if (!node_) throw std::runtime_error("Dereferencing end iterator");
      return &(node_->data);
    }

    /*
     * Pre-increment:
     * modifies the current iterator in place and returns a reference to it.
     */
    iterator_base& operator++() {
      if (node_ == tree_->end_node_) {
        throw std::runtime_error("Incrementing past end iterator");
      } else if (node_ == tree_->max_node_) {
        // transition from max_node_ to end_node_
        node_ = tree_->end_node_;
      } else if (node_->right) {
        node_ = tree_->leftmost_node(node_->right);
      } else {
        // traverse upward until find a parent that is not fully traversed
        Node* parent = node_->parent;
        while (parent && node_ == parent->right) {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    /*
     * Post-increment:
     * Creates a copy of the iterator, increments the current iterator in place,
     * and returns the copy.
     */
    iterator_base operator++(int) {
      iterator_base tmp = *this;
      ++(*this);
      return tmp;
    }

    iterator_base& operator--() {
      if (node_ == tree_->min_node_) {
        throw std::runtime_error("Decrementing past begin iterator");
      } else if (node_ == tree_->end_node_) {
        // transition from end_node_ to max_node_
        node_ = tree_->max_node_;
      } else if (node_->left) {
        node_ = tree_->rightmost_node(node_->left);
      } else {
        // traverse upward until find a parent that is not fully traversed
        Node* parent = node_->parent;
        while (parent && node_ == parent->left) {
          node_ = parent;
          parent = parent->parent;
        }
        node_ = parent;
      }
      return *this;
    }

    iterator_base operator--(int) {
      iterator_base tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator==(const iterator_base& other) const noexcept {
      return node_ == other.node_;
    }

    bool operator!=(const iterator_base& other) const noexcept {
      return node_ != other.node_;
    }

   private:
    Node* node_;
    TreeType* tree_;
  };

  // type aliases for iterator and const_iterator
  using iterator = iterator_base<false>;
  using const_iterator = iterator_base<true>;

  /* Member functions */
  // constructor
  BinaryTree()
      : root_(nullptr),
        min_node_(nullptr),
        max_node_(nullptr),
        end_node_(new Node(T{})),
        size_(0) {}

  // copy constructor
  BinaryTree(const BinaryTree& other)
      : root_(nullptr), end_node_(new Node(T{})), size_(0) {
    copy_tree(other.root_, nullptr);
    root_ = balance(root_);
    update_min_max_nodes();
    size_ = other.size_;
  }

  // destructor
  ~BinaryTree() noexcept {
    clear_tree(root_);
    delete end_node_;
  }

  // move constructor
  BinaryTree(BinaryTree&& other) noexcept {
    root_ = other.root_;
    min_node_ = other.min_node_;
    max_node_ = other.max_node_;
    end_node_ = other.end_node_;
    size_ = other.size_;

    other.root_ = nullptr;
    other.min_node_ = nullptr;
    other.max_node_ = nullptr;
    other.end_node_ = nullptr;
    other.size_ = 0;
  }

  // move assignment
  BinaryTree& operator=(BinaryTree&& other) {
    if (this != &other) {
      clear_tree(root_);
      delete end_node_;

      root_ = other.root_;
      min_node_ = other.min_node_;
      max_node_ = other.max_node_;
      end_node_ = other.end_node_;
      size_ = other.size_;

      other.root_ = nullptr;
      other.min_node_ = nullptr;
      other.max_node_ = nullptr;
      other.end_node_ = new Node(T{});
      other.size_ = 0;
    }
    return *this;
  }

  /* Iterators */
  iterator begin() { return root_ ? iterator(min_node_, this) : end(); }

  iterator end() { return iterator(end_node_, this); }

  const_iterator begin() const {
    return root_ ? const_iterator(min_node_, this) : end();
  }

  const_iterator end() const { return const_iterator(end_node_, this); }

  /* Capacity */
  bool empty() const noexcept { return begin() == end(); }  // size_ == 0

  std::size_t size() const noexcept { return size_; }

  /* Theoretical maximum number of elements the container can hold.
  The std::containers use a more complex internal structure (a Red-Black Tree),
  which imposes additional memory overhead per element like metadata, so the
  size of its node may differ. Size of s21::BinaryTree<int>::Node: 40 bytes
  Estimated size of std::set Node: 80 bytes (the value is an approximation,
  since Node is a private member), hence, the max_size() would differ for the
  implementations. */
  std::size_t max_size() const noexcept {
    return static_cast<std::size_t>(SIZE_MAX / sizeof(Node));
  };

  /* Modifiers */
  void clear() {
    clear_tree(root_);
    root_ = nullptr;
    min_node_ = nullptr;
    max_node_ = nullptr;
    size_ = 0;
  }

  std::pair<iterator, bool> insert(const value_type& value,
                                   bool allow_duplicates = false) {
    auto [new_root, inserted] = insert_node(root_, value, allow_duplicates);
    root_ = new_root;

    if (inserted) {
      update_min_max_nodes();
      ++size_;
    }

    Node* inserted_node = find_node(root_, extract_key(value));
    return {iterator(inserted_node, this), inserted};
  }

  void erase(iterator pos) {
    if (pos == end()) throw std::runtime_error("Cannot erase end iterator");
    Node* node_to_remove = pos.node_;
    Node* child =
        (node_to_remove->left ? node_to_remove->left : node_to_remove->right);
    if (!node_to_remove->left || !node_to_remove->right) {
      // Case 1 or 2: Node has no children or one child
      root_ = erase_node(root_, node_to_remove, child);
    } else {
      // Case 3: Node has two children
      Node* successor = leftmost_node(node_to_remove->right);
      // re-linking instead of swapping node->data due to const Key in map
      Node* successor_child = successor->right;
      root_ = erase_node(root_, successor, successor_child);
      successor->left = node_to_remove->left;
      successor->right = node_to_remove->right;
      if (successor->left) successor->left->parent = successor;
      if (successor->right) successor->right->parent = successor;
      if (node_to_remove->parent) {
        if (node_to_remove->parent->left == node_to_remove)
          node_to_remove->parent->left = successor;
        else
          node_to_remove->parent->right = successor;
      } else
        root_ = successor;
      successor->parent = node_to_remove->parent;
    }
    delete node_to_remove;
    if (root_) {
      update_height(root_);
      root_ = balance(root_);
    }
    update_min_max_nodes();
    --size_;
  }

  void merge(BinaryTree& other, bool allow_duplicates = false) {
    // create a const copy of 'other' to iterate over it while modifying the
    // original and traverse it using const_iterator
    const BinaryTree const_other(other);
    for (auto const_it = const_other.begin(); const_it != const_other.end();
         ++const_it) {
      if (insert(*const_it, allow_duplicates).second) {
        // erase the element from 'other' using an iterator
        other.erase(other.find(extract_key(*const_it)));
      }
    }
  }

  /* Lookup */
  iterator find(const Key& key) noexcept {
    Node* node = find_node(root_, key);
    return node ? iterator(node, this) : end();
  }

  const_iterator find(const Key& key) const noexcept {
    Node* node = find_node(root_, key);
    return node ? const_iterator(node, this) : end();
  }

  bool contains(const Key& key) const noexcept { return find(key) != end(); }

 protected:
  Node* root_;
  Node* min_node_;
  Node* max_node_;
  Node* end_node_;  // one element past the last, end()
  std::size_t size_;

  Key extract_key(const value_type& value) const {
    if constexpr (std::is_same_v<value_type, Key>) {
      // set, multiset: value_type is Key
      return value;
    } else {
      // map: value_type is std::pair<Key, T>, return the key part of the pair
      return value.first;
    }
  }

  std::pair<Node*, bool> insert_node(Node* node, const value_type& value,
                                     bool allow_duplicates) {
    std::pair<Node*, bool> result = {node, false};
    if (!node) {
      // Base case: create a new node
      result.first = new Node(value);
      result.second = true;
    } else {
      if (std::less<Key>()(extract_key(value), extract_key(node->data))) {
        // insert into the left subtree
        auto [new_left, was_inserted] =
            insert_node(node->left, value, allow_duplicates);
        node->left = new_left;
        if (new_left) new_left->parent = node;
        result.second = was_inserted;
      } else if (std::less<Key>()(extract_key(node->data),
                                  extract_key(value)) ||
                 allow_duplicates) {
        // insert into the right subtree
        auto [new_right, was_inserted] =
            insert_node(node->right, value, allow_duplicates);
        node->right = new_right;
        if (new_right) new_right->parent = node;
        result.second = was_inserted;
      } else {
        // duplicate key found and not allowed, no insertion
        result.first = node;
        result.second = false;
      }
      if (result.second) {
        update_height(node);
        result.first = balance(node);
        if (result.first->left) result.first->left->parent = result.first;
        if (result.first->right) result.first->right->parent = result.first;
      }
    }
    return result;
  }

  Node* erase_node(Node* root, Node* node, Node* child) {
    if (node == root) {
      root = child;
    } else {
      if (node == node->parent->left)
        node->parent->left = child;
      else
        node->parent->right = child;

      if (child) child->parent = node->parent;
    }
    return root;
  }

  Node* copy_tree(Node* other_node, Node* parent) {
    if (!other_node) return nullptr;
    Node* new_node = new Node(other_node->data);
    new_node->parent = parent;
    if (!parent) root_ = new_node;
    new_node->left = copy_tree(other_node->left, new_node);
    new_node->right = copy_tree(other_node->right, new_node);

    return new_node;
  }

  void clear_tree(Node* node) {
    if (node) {
      clear_tree(node->left);
      clear_tree(node->right);
      delete node;
    }
  }

  Node* rotate_right(Node* old_root) {
    Node* new_root = old_root->left;  // Left child becomes the new root
    Node* detached_node =
        new_root->right;  // Temporary node (right child of new root)

    // Perform rotation
    new_root->right = old_root;
    old_root->left = detached_node;

    // Update parent pointers
    if (detached_node) detached_node->parent = old_root;
    new_root->parent = old_root->parent;
    old_root->parent = new_root;

    update_height(old_root);
    update_height(new_root);

    return new_root;
  }

  Node* rotate_left(Node* old_root) {
    Node* new_root = old_root->right;  // Right child becomes the new root
    Node* detached_node =
        new_root->left;  // Temporary node (left child of new root)

    // Perform rotation
    new_root->left = old_root;
    old_root->right = detached_node;

    // Update parent pointers
    if (detached_node) detached_node->parent = old_root;
    new_root->parent = old_root->parent;
    old_root->parent = new_root;

    update_height(old_root);
    update_height(new_root);

    return new_root;
  }

  Node* balance(Node* node) {
    int balance_factor = get_balance_factor(node);

    if (balance_factor > 1) {
      if (get_balance_factor(node->left) < 0)
        node->left = rotate_left(node->left);
      node = rotate_right(node);
    } else if (balance_factor < -1) {
      if (get_balance_factor(node->right) > 0)
        node->right = rotate_right(node->right);
      node = rotate_left(node);
    }

    return node;
  }

  Node* find_node(Node* node, const Key& key) const noexcept {
    Node* found_node = nullptr;
    while (node && !found_node) {
      if (std::less<Key>()(key, extract_key(node->data))) {
        node = node->left;
      } else if (std::less<Key>()(extract_key(node->data), key)) {
        node = node->right;
      } else {
        found_node = node;
      }
    }
    return found_node;
  }

  void update_min_max_nodes() {
    min_node_ = BinaryTree::leftmost_node(root_);
    max_node_ = BinaryTree::rightmost_node(root_);
  }

  void update_height(Node* node) {
    node->height =
        1 + std::max(get_height(node->left), get_height(node->right));
  }

  static int get_height(Node* node) { return node ? node->height : 0; }

  static int get_balance_factor(Node* node) {
    return node ? get_height(node->left) - get_height(node->right) : 0;
  }

  static Node* leftmost_node(Node* node) {
    while (node && node->left) node = node->left;
    return node;
  }

  static Node* rightmost_node(Node* node) {
    while (node && node->right) node = node->right;
    return node;
  }
};

}  // namespace s21

#endif
