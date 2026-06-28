#ifndef BLIST_ITERATORS_HPP
#define BLIST_ITERATORS_HPP

#include "node.hpp"

namespace em {

  template <class T>
  class List;

  template <class T>
  class LIter {
    friend class List<T>;

    Node<T>* node_;

    explicit LIter(Node<T>* node) : node_(node) {}

  public:
    LIter() noexcept : node_(nullptr) {}

    T& operator*() { return node_->data; }
    T* operator->() { return &node_->data; }

    LIter& operator++() {
      node_ = node_->next;
      return *this;
    }

    LIter operator++(int) {
      LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    LIter& operator--() {
      node_ = node_->prev;
      return *this;
    }

    LIter operator--(int) {
      LIter tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator==(const LIter& other) const {
      return node_ == other.node_;
    }

    bool operator!=(const LIter& other) const {
      return node_ != other.node_;
    }
  };

  template <class T>
  class LCIter {
    friend class List<T>;

    const Node<T>* node_;

    explicit LCIter(const Node<T>* node) : node_(node) {}

  public:
    LCIter() noexcept : node_(nullptr) {}

    LCIter(const LIter<T>& iter) : node_(iter.node_) {}

    const T& operator*() const { return node_->data; }
    const T* operator->() const { return &node_->data; }

    LCIter& operator++() {
      node_ = node_->next;
      return *this;
    }

    LCIter operator++(int) {
      LCIter tmp = *this;
      ++(*this);
      return tmp;
    }

    LCIter& operator--() {
      node_ = node_->prev;
      return *this;
    }

    LCIter operator--(int) {
      LCIter tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator==(const LCIter& other) const {
      return node_ == other.node_;
    }

    bool operator!=(const LCIter& other) const {
      return node_ != other.node_;
    }
  };

}

#endif
