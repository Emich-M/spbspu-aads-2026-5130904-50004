#ifndef EM_BSTREE_HPP
#define EM_BSTREE_HPP

#include <functional>
#include <stdexcept>
#include <utility>

namespace em
{

template <class Key, class Value>
class BSTNode
{
public:
  using PairType = std::pair<const Key, Value>;

  PairType data_;
  BSTNode *left_ = nullptr;
  BSTNode *right_ = nullptr;
  BSTNode *parent_ = nullptr;

  explicit BSTNode(const Key &k, const Value &v)
      : data_(k, v)
  {
  }

  explicit BSTNode(Key &&k, Value &&v)
      : data_(std::move(k), std::move(v))
  {
  }
};

template <class Key, class Value>
class BSTIterator;

template <class Key, class Value>
class BSTConstIterator
{
  friend class BSTree<Key, Value, std::less<Key>>;
  friend class BSTIterator<Key, Value>;

private:
  using Node = BSTNode<Key, Value>;
  Node *node_ = nullptr;

public:
  using value_type = typename Node::PairType;
  using pointer = const value_type *;
  using reference = const value_type &;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  explicit BSTConstIterator(Node *n = nullptr)
      : node_(n)
  {
  }

  reference operator*() const
  {
    return node_->data_;
  }

  pointer operator->() const
  {
    return &node_->data_;
  }

  BSTConstIterator &operator++();
  BSTConstIterator &operator--();

  bool operator==(const BSTConstIterator &other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const BSTConstIterator &other) const
  {
    return !(*this == other);
  }
};

template <class Key, class Value>
class BSTIterator
{
  friend class BSTree<Key, Value, std::less<Key>>;

private:
  using Node = BSTNode<Key, Value>;
  Node *node_ = nullptr;

public:
  using value_type = typename Node::PairType;
  using pointer = value_type *;
  using reference = value_type &;
  using difference_type = std::ptrdiff_t;
  using iterator_category = std::bidirectional_iterator_tag;

  explicit BSTIterator(Node *n = nullptr)
      : node_(n)
  {
  }

  reference operator*() const
  {
    return node_->data_;
  }

  pointer operator->() const
  {
    return &node_->data_;
  }

  BSTIterator &operator++();
  BSTIterator &operator--();

  bool operator==(const BSTIterator &other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const BSTIterator &other) const
  {
    return !(*this == other);
  }

  operator BSTConstIterator<Key, Value>() const
  {
    return BSTConstIterator<Key, Value>(node_);
  }
};

template <class Key, class Value>
void increment(BSTNode<Key, Value> *&node)
{
  if (node->right_) {
    node = node->right_;
    while (node->left_) {
      node = node->left_;
    }
  } else {
    auto *parent = node->parent_;
    while (parent && node == parent->right_) {
      node = parent;
      parent = parent->parent_;
    }
    node = parent;
  }
}

template <class Key, class Value>
void decrement(BSTNode<Key, Value> *&node)
{
  if (node->left_) {
    node = node->left_;
    while (node->right_) {
      node = node->right_;
    }
  } else {
    auto *parent = node->parent_;
    while (parent && node == parent->left_) {
      node = parent;
      parent = parent->parent_;
    }
    node = parent;
  }
}

template <class Key, class Value>
BSTConstIterator<Key, Value> &BSTConstIterator<Key, Value>::operator++()
{
  increment(node_);
  return *this;
}

template <class Key, class Value>
BSTConstIterator<Key, Value> &BSTConstIterator<Key, Value>::operator--()
{
  decrement(node_);
  return *this;
}

template <class Key, class Value>
BSTIterator<Key, Value> &BSTIterator<Key, Value>::operator++()
{
  increment(node_);
  return *this;
}

template <class Key, class Value>
BSTIterator<Key, Value> &BSTIterator<Key, Value>::operator--()
{
  decrement(node_);
  return *this;
}

template <class Key, class Value, class Compare = std::less<Key>>
class BSTree
{
public:
  using iterator = BSTIterator<Key, Value>;
  using const_iterator = BSTConstIterator<Key, Value>;

  BSTree() = default;

  BSTree(const BSTree &other)
      : comp_(other.comp_)
  {
    for (const auto &kv : other) {
      push(kv.first, kv.second);
    }
  }

  BSTree &operator=(const BSTree &other)
  {
    if (this != &other) {
      clear();
      comp_ = other.comp_;
      for (const auto &kv : other) {
        push(kv.first, kv.second);
      }
    }
    return *this;
  }

  BSTree(BSTree &&other) noexcept
      : root_(other.root_),
        size_(other.size_),
        comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  BSTree &operator=(BSTree &&other) noexcept
  {
    if (this != &other) {
      clear();
      root_ = other.root_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  ~BSTree()
  {
    clear();
  }

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  void clear()
  {
    destroySubtree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  void push(const Key &key, const Value &value)
  {
    Node *parent = nullptr;
    Node *cur = root_;
    while (cur) {
      parent = cur;
      if (comp_(key, cur->data_.first)) {
        cur = cur->left_;
      } else if (comp_(cur->data_.first, key)) {
        cur = cur->right_;
      } else {
        cur->data_.second = value;
        return;
      }
    }

    Node *new_node = new Node(key, value);
    new_node->parent_ = parent;

    if (!parent) {
      root_ = new_node;
    } else if (comp_(key, parent->data_.first)) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
    ++size_;
  }

  void push(Key &&key, Value &&value)
  {
    Node *parent = nullptr;
    Node *cur = root_;
    while (cur) {
      parent = cur;
      if (comp_(key, cur->data_.first)) {
        cur = cur->left_;
      } else if (comp_(cur->data_.first, key)) {
        cur = cur->right_;
      } else {
        cur->data_.second = std::move(value);
        return;
      }
    }

    Node *new_node = new Node(std::move(key), std::move(value));
    new_node->parent_ = parent;

    if (!parent) {
      root_ = new_node;
    } else if (comp_(new_node->data_.first, parent->data_.first)) {
      parent->left_ = new_node;
    } else {
      parent->right_ = new_node;
    }
    ++size_;
  }

  Value get(const Key &key) const
  {
    Node *node = findNode(key);
    if (!node) {
      throw std::out_of_range("Key not found");
    }
    return node->data_.second;
  }

  Value drop(const Key &key)
  {
    Node *node = findNode(key);
    if (!node) {
      throw std::out_of_range("Key not found");
    }

    Value result = std::move(node->data_.second);

    Node *to_delete = node;
    Node *child = nullptr;

    if (!node->left_) {
      child = node->right_;
    } else if (!node->right_) {
      child = node->left_;
    } else {
      Node *succ = successor(node);
      node->data_ = std::move(succ->data_);
      to_delete = succ;
      child = succ->right_;
    }

    Node *parent = to_delete->parent_;

    if (child) {
      child->parent_ = parent;
    }

    if (!parent) {
      root_ = child;
    } else if (to_delete == parent->left_) {
      parent->left_ = child;
    } else {
      parent->right_ = child;
    }

    delete to_delete;
    --size_;
    return result;
  }

  const_iterator begin() const
  {
    return const_iterator(minNode(root_));
  }

  const_iterator end() const
  {
    return const_iterator(nullptr);
  }

  iterator begin()
  {
    return iterator(minNode(root_));
  }

  iterator end()
  {
    return iterator(nullptr);
  }

  const_iterator rotateLeft(const_iterator it)
  {
    if (!it.node_ || !it.node_->right_) {
      return it;
    }
    Node *x = it.node_;
    Node *y = x->right_;

    x->right_ = y->left_;
    if (y->left_) {
      y->left_->parent_ = x;
    }

    y->parent_ = x->parent_;
    if (!x->parent_) {
      root_ = y;
    } else if (x == x->parent_->left_) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }

    y->left_ = x;
    x->parent_ = y;

    return const_iterator(y);
  }

  const_iterator rotateRight(const_iterator it)
  {
    if (!it.node_ || !it.node_->left_) {
      return it;
    }
    Node *x = it.node_;
    Node *y = x->left_;

    x->left_ = y->right_;
    if (y->right_) {
      y->right_->parent_ = x;
    }

    y->parent_ = x->parent_;
    if (!x->parent_) {
      root_ = y;
    } else if (x == x->parent_->left_) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }

    y->right_ = x;
    x->parent_ = y;

    return const_iterator(y);
  }

  const_iterator rotateLargeLeft(const_iterator it)
  {
    return it;
  }

  const_iterator rotateLargeRight(const_iterator it)
  {
    return it;
  }

  size_t height(const_iterator it) const
  {
    if (!it.node_) {
      return 0;
    }
    auto calcHeight = [](Node *n, auto &&self) -> size_t {
      if (!n) {
        return 0;
      }
      size_t left_h = self(n->left_, self);
      size_t right_h = self(n->right_, self);
      return 1 + (left_h > right_h ? left_h : right_h);
    };
    return calcHeight(it.node_, calcHeight);
  }

  size_t height() const
  {
    return height(const_iterator(root_));
  }

private:
  using Node = BSTNode<Key, Value>;
  using PairType = typename Node::PairType;

  Node *root_ = nullptr;
  size_t size_ = 0;
  Compare comp_;

  Node *findNode(const Key &key) const
  {
    Node *cur = root_;
    while (cur) {
      if (comp_(key, cur->data_.first)) {
        cur = cur->left_;
      } else if (comp_(cur->data_.first, key)) {
        cur = cur->right_;
      } else {
        return cur;
      }
    }
    return nullptr;
  }

  Node *minNode(Node *node) const
  {
    while (node && node->left_) {
      node = node->left_;
    }
    return node;
  }

  Node *maxNode(Node *node) const
  {
    while (node && node->right_) {
      node = node->right_;
    }
    return node;
  }

  Node *successor(Node *node) const
  {
    if (node->right_) {
      return minNode(node->right_);
    }
    auto *parent = node->parent_;
    while (parent && node == parent->right_) {
      node = parent;
      parent = parent->parent_;
    }
    return parent;
  }

  void destroySubtree(Node *node)
  {
    if (!node) {
      return;
    }
    destroySubtree(node->left_);
    destroySubtree(node->right_);
    delete node;
  }
};

}

#endif
