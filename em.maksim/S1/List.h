#ifndef LIST_H
#define LIST_H

#include <stdexcept>
#include <utility>
#include <limits>
#include <cstddef>

namespace em {

  template <class T>
  class List;

  template <class T>
  class LIter {
    friend class List<T>;

  public:
    LIter() noexcept;
    LIter(const LIter&) noexcept = default;
    LIter(LIter&&) noexcept = default;
    ~LIter() = default;

    LIter& operator=(const LIter&) noexcept = default;
    LIter& operator=(LIter&&) noexcept = default;

    T& operator*() const;
    T* operator->() const;
    LIter& operator++() noexcept;

    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;

  private:
    typename List<T>::Node* current_;

    explicit LIter(typename List<T>::Node* p) noexcept;
  };

  template <class T>
  class LCIter {
    friend class List<T>;

  public:
    LCIter() noexcept;
    LCIter(const LCIter&) noexcept = default;
    LCIter(LCIter&&) noexcept = default;
    ~LCIter() = default;

    LCIter& operator=(const LCIter&) noexcept = default;
    LCIter& operator=(LCIter&&) noexcept = default;

    const T& operator*() const;
    const T* operator->() const;
    LCIter& operator++() noexcept;

    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;

  private:
    const typename List<T>::Node* current_;

    explicit LCIter(const typename List<T>::Node* p) noexcept;
  };

  template <class T>
  class List {
    friend class LIter<T>;
    friend class LCIter<T>;

  protected:
    struct Node {
      T data;
      Node* next;

      explicit Node(const T& val, Node* nxt = nullptr);
      explicit Node(T&& val, Node* nxt = nullptr);
    };

    Node* first_;
    Node* last_;
    size_t size_;

  public:
    List();
    ~List();

    List(const List& other);
    List(List&& other) noexcept;

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    void swap(List& other) noexcept;

    LIter<T> begin() noexcept;
    LIter<T> end() noexcept;
    LCIter<T> cbegin() const noexcept;
    LCIter<T> cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    LIter<T> insert_after(LIter<T> pos, const T& value);
    LIter<T> insert_after(LIter<T> pos, T&& value);
    LIter<T> erase_after(LIter<T> pos);

    void clear();
  };

  template <class T>
  inline void sum(T& a, const T& b);

}

#include "List.hpp"

#endif
