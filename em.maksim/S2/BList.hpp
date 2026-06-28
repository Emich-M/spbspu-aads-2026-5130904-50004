#ifndef BLIST_HPP
#define BLIST_HPP

#include "BList-iterators.hpp"
#include <cstddef>
#include <utility>

namespace em {

  template <class T>
  class List {
    Node<T>* head_;
    Node<T>* tail_;
    size_t size_;

    void clear_nodes() noexcept {
      Node<T>* current = head_;
      while (current) {
        Node<T>* next = current->next;
        delete current;
        current = next;
      }
    }

    void copy_from(const List& other) {
      Node<T>* new_head = nullptr;
      Node<T>* new_tail = nullptr;
      size_t new_size = 0;

      for (auto it = other.cbegin(); it != other.cend(); ++it) {
        Node<T>* node = new Node<T>(*it);
        if (!new_head) {
          new_head = new_tail = node;
        } else {
          new_tail->next = node;
          node->prev = new_tail;
          new_tail = node;
        }
        ++new_size;
      }

      clear_nodes();
      head_ = new_head;
      tail_ = new_tail;
      size_ = new_size;
    }

  public:
    List() noexcept
      : head_(nullptr),
        tail_(nullptr),
        size_(0) {}

    ~List() {
      clear_nodes();
    }

    List(const List& other)
      : head_(nullptr),
        tail_(nullptr),
        size_(0) {
      copy_from(other);
    }

    List(List&& other) noexcept
      : head_(other.head_),
        tail_(other.tail_),
        size_(other.size_) {
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    List& operator=(const List& other) {
      if (this != &other) {
        List tmp(other);
        std::swap(head_, tmp.head_);
        std::swap(tail_, tmp.tail_);
        std::swap(size_, tmp.size_);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept {
      if (this != &other) {
        clear_nodes();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    void push_front(const T& value) {
      Node<T>* node = new Node<T>(value);
      if (!head_) {
        head_ = tail_ = node;
      } else {
        node->next = head_;
        head_->prev = node;
        head_ = node;
      }
      ++size_;
    }

    void push_back(const T& value) {
      Node<T>* node = new Node<T>(value);
      if (!tail_) {
        head_ = tail_ = node;
      } else {
        node->prev = tail_;
        tail_->next = node;
        tail_ = node;
      }
      ++size_;
    }

    void pop_front() {
      if (!head_) {
        return;
      }
      Node<T>* node = head_;
      head_ = head_->next;
      if (head_) {
        head_->prev = nullptr;
      } else {
        tail_ = nullptr;
      }
      delete node;
      --size_;
    }

    void pop_back() {
      if (!tail_) {
        return;
      }
      Node<T>* node = tail_;
      tail_ = tail_->prev;
      if (tail_) {
        tail_->next = nullptr;
      } else {
        head_ = nullptr;
      }
      delete node;
      --size_;
    }

    void clear() noexcept {
      clear_nodes();
      head_ = nullptr;
      tail_ = nullptr;
      size_ = 0;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

    T& front() { return head_->data; }
    const T& front() const { return head_->data; }
    T& back() { return tail_->data; }
    const T& back() const { return tail_->data; }

    LIter<T> begin() { return LIter<T>(head_); }
    LIter<T> end() { return LIter<T>(nullptr); }
    LCIter<T> cbegin() const { return LCIter<T>(head_); }
    LCIter<T> cend() const { return LCIter<T>(nullptr); }

    LIter<T> insert(LIter<T> pos, const T& value) {
      Node<T>* node = new Node<T>(value);
      if (pos.node_ == head_) {
        if (!head_) {
          head_ = tail_ = node;
        } else {
          node->next = head_;
          head_->prev = node;
          head_ = node;
        }
      } else {
        node->next = pos.node_;
        node->prev = pos.node_->prev;
        pos.node_->prev->next = node;
        pos.node_->prev = node;
      }
      ++size_;
      return LIter<T>(node);
    }
  };

}

#endif
