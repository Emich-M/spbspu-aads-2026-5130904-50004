#ifndef NODE_HPP
#define NODE_HPP

namespace em {

  template <class T>
  struct Node {
    T data;
    Node* prev;
    Node* next;

    explicit Node(const T& data, Node* prev_ptr = nullptr, Node* next_ptr = nullptr)
      : data(data),
        prev(prev_ptr),
        next(next_ptr) {}
  };

}

#endif
