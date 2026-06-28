#ifndef LIST_HPP
#define LIST_HPP

#include "List.h"
#include <stdexcept>
#include <utility>
#include <limits>
#include <cstddef>

namespace em {

  template <class T>
  List<T>::Node::Node(const T& val, Node* nxt)
    : data(val), next(nxt) {}

  template <class T>
  List<T>::Node::Node(T&& val, Node* nxt)
    : data(std::move(val)), next(nxt) {}

  template <class T>
  List<T>::List()
    : first_(nullptr), last_(nullptr), size_(0) {}

  template <class T>
  List<T>::~List() {
    clear();
  }

  template <class T>
  List<T>::List(const List& other)
    : first_(nullptr), last_(nullptr), size_(0) {
    for (Node* curr = other.first_; curr != nullptr; curr = curr->next) {
      push_back(curr->data);
    }
  }

  template <class T>
  List<T>::List(List&& other) noexcept
    : first_(other.first_), last_(other.last_), size_(other.size_) {
    other.first_ = nullptr;
    other.last_ = nullptr;
    other.size_ = 0;
  }

  template <class T>
  List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template <class T>
  List<T>& List<T>::operator=(List&& other) noexcept {
    if (this != &other) {
      clear();
      first_ = other.first_;
      last_ = other.last_;
      size_ = other.size_;
      other.first_ = nullptr;
      other.last_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  template <class T>
  void List<T>::swap(List& other) noexcept {
    Node* temp_first = first_;
    Node* temp_last = last_;
    size_t temp_size = size_;

    first_ = other.first_;
    last_ = other.last_;
    size_ = other.size_;

    other.first_ = temp_first;
    other.last_ = temp_last;
    other.size_ = temp_size;
  }

  template <class T>
  LIter<T> List<T>::begin() noexcept {
    return LIter<T>(first_);
  }

  template <class T>
  LIter<T> List<T>::end() noexcept {
    return LIter<T>(nullptr);
  }

  template <class T>
  LCIter<T> List<T>::cbegin() const noexcept {
    return LCIter<T>(first_);
  }

  template <class T>
  LCIter<T> List<T>::cend() const noexcept {
    return LCIter<T>(nullptr);
  }

  template <class T>
  bool List<T>::empty() const noexcept {
    return size_ == 0;
  }

  template <class T>
  size_t List<T>::size() const noexcept {
    return size_;
  }

  template <class T>
  T& List<T>::front() {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return first_->data;
  }

  template <class T>
  const T& List<T>::front() const {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return first_->data;
  }

  template <class T>
  T& List<T>::back() {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return last_->data;
  }

  template <class T>
  const T& List<T>::back() const {
    if (empty()) {
      throw std::out_of_range("List is empty");
    }
    return last_->data;
  }

  template <class T>
  void List<T>::push_front(const T& value) {
    Node* new_node = new Node(value, first_);
    first_ = new_node;
    if (last_ == nullptr) {
      last_ = new_node;
    }
    size_++;
  }

  template <class T>
  void List<T>::push_front(T&& value) {
    Node* new_node = new Node(std::move(value), first_);
    first_ = new_node;
    if (last_ == nullptr) {
      last_ = new_node;
    }
    size_++;
  }

  template <class T>
  void List<T>::pop_front() {
    if (empty()) {
      return;
    }
    Node* temp = first_;
    first_ = first_->next;
    if (first_ == nullptr) {
      last_ = nullptr;
    }
    delete temp;
    size_--;
  }

  template <class T>
  void List<T>::push_back(const T& value) {
    Node* new_node = new Node(value);
    if (empty()) {
      first_ = new_node;
      last_ = new_node;
    } else {
      last_->next = new_node;
      last_ = new_node;
    }
    size_++;
  }

  template <class T>
  void List<T>::push_back(T&& value) {
    Node* new_node = new Node(std::move(value));
    if (empty()) {
      first_ = new_node;
      last_ = new_node;
    } else {
      last_->next = new_node;
      last_ = new_node;
    }
    size_++;
  }

  template <class T>
  void List<T>::pop_back() {
    if (empty()) {
      return;
    }
    if (first_ == last_) {
      delete first_;
      first_ = nullptr;
      last_ = nullptr;
    } else {
      Node* temp = first_;
      while (temp->next != last_) {
        temp = temp->next;
      }
      delete last_;
      last_ = temp;
      last_->next = nullptr;
    }
    size_--;
  }

  template <class T>
  LIter<T> List<T>::insert_after(LIter<T> pos, const T& value) {
    Node* curr = pos.current_;
    if (curr == nullptr) {
      throw std::out_of_range("Cannot insert after nullptr");
    }
    Node* new_node = new Node(value, curr->next);
    curr->next = new_node;
    if (curr == last_) {
      last_ = new_node;
    }
    size_++;
    return LIter<T>(new_node);
  }

  template <class T>
  LIter<T> List<T>::insert_after(LIter<T> pos, T&& value) {
    Node* curr = pos.current_;
    if (curr == nullptr) {
      throw std::out_of_range("Cannot insert after nullptr");
    }
    Node* new_node = new Node(std::move(value), curr->next);
    curr->next = new_node;
    if (curr == last_) {
      last_ = new_node;
    }
    size_++;
    return LIter<T>(new_node);
  }

  template <class T>
  LIter<T> List<T>::erase_after(LIter<T> pos) {
    Node* curr = pos.current_;
    if (curr == nullptr || curr->next == nullptr) {
      throw std::out_of_range("Cannot erase after nullptr");
    }
    Node* temp = curr->next;
    curr->next = temp->next;
    if (temp == last_) {
      last_ = curr;
    }
    delete temp;
    size_--;
    return LIter<T>(curr->next);
  }

  template <class T>
  void List<T>::clear() {
    while (first_ != nullptr) {
      pop_front();
    }
  }

  template <class T>
  LIter<T>::LIter() noexcept
    : current_(nullptr) {}

  template <class T>
  LIter<T>::LIter(typename List<T>::Node* p) noexcept
    : current_(p) {}

  template <class T>
  T& LIter<T>::operator*() const {
    if (current_ == nullptr) {
      throw std::out_of_range("Dereference null iterator");
    }
    return current_->data;
  }

  template <class T>
  T* LIter<T>::operator->() const {
    if (current_ == nullptr) {
      throw std::out_of_range("Access null iterator");
    }
    return &(current_->data);
  }

  template <class T>
  LIter<T>& LIter<T>::operator++() noexcept {
    if (current_ != nullptr) {
      current_ = current_->next;
    }
    return *this;
  }

  template <class T>
  bool LIter<T>::operator==(const LIter& other) const noexcept {
    return current_ == other.current_;
  }

  template <class T>
  bool LIter<T>::operator!=(const LIter& other) const noexcept {
    return current_ != other.current_;
  }

  template <class T>
  LCIter<T>::LCIter() noexcept
    : current_(nullptr) {}

  template <class T>
  LCIter<T>::LCIter(const typename List<T>::Node* p) noexcept
    : current_(p) {}

  template <class T>
  const T& LCIter<T>::operator*() const {
    if (current_ == nullptr) {
      throw std::out_of_range("Dereference null const iterator");
    }
    return current_->data;
  }

  template <class T>
  const T* LCIter<T>::operator->() const {
    if (current_ == nullptr) {
      throw std::out_of_range("Access null const iterator");
    }
    return &(current_->data);
  }

  template <class T>
  LCIter<T>& LCIter<T>::operator++() noexcept {
    if (current_ != nullptr) {
      current_ = current_->next;
    }
    return *this;
  }

  template <class T>
  bool LCIter<T>::operator==(const LCIter& other) const noexcept {
    return current_ == other.current_;
  }

  template <class T>
  bool LCIter<T>::operator!=(const LCIter& other) const noexcept {
    return current_ != other.current_;
  }

  template <class T>
  inline void sum(T& a, const T& b) {
    if (std::numeric_limits<T>::max() - b < a) {
      throw std::overflow_error("Overflow");
    }
    a += b;
  }

}

#endif
