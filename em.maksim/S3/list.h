#ifndef EM_LIST_H
#define EM_LIST_H

#include <cstddef>

namespace em {

  template <class T>
  class List {
  private:
    struct Node {
      T data;
      Node* next;
      Node* prev;

      Node(const T& d);
    };

    Node* head_;
    Node* tail_;
    size_t size_;

  public:
    List();
    ~List();
    List(const List& other);
    List(List&& other) noexcept;
    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();
    void clear();
    size_t size() const;
    bool empty() const;

    class Iterator;
    class ConstIterator;

    Iterator begin();
    Iterator end();
    ConstIterator begin() const;
    ConstIterator end() const;
    ConstIterator cbegin() const;
    ConstIterator cend() const;
  };

  template <class T>
  class List<T>::Iterator {
  private:
    Node* node_;

  public:
    Iterator();
    explicit Iterator(Node* node);

    T& operator*();
    T* operator->();
    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);
    bool operator==(const Iterator& other) const;
    bool operator!=(const Iterator& other) const;
  };

  template <class T>
  class List<T>::ConstIterator {
  private:
    const Node* node_;

  public:
    ConstIterator();
    explicit ConstIterator(const Node* node);

    const T& operator*() const;
    const T* operator->() const;
    ConstIterator& operator++();
    ConstIterator operator++(int);
    ConstIterator& operator--();
    ConstIterator operator--(int);
    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;
  };

}

#endif
