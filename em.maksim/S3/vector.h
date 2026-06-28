#ifndef EM_VECTOR_H
#define EM_VECTOR_H

#include <cstddef>
#include <string>
#include <utility>

namespace em {

  template <class T>
  class Vector {
  private:
    T* data_;
    size_t size_;
    size_t capacity_;

  public:
    Vector()
      : data_(nullptr),
        size_(0),
        capacity_(0) {
    }

    ~Vector() {
      delete[] data_;
    }

    Vector(const Vector& other)
      : data_(nullptr),
        size_(0),
        capacity_(0) {
      for (size_t i = 0; i < other.size_; ++i) {
        push_back(other.data_[i]);
      }
    }

    Vector& operator=(const Vector& other) {
      if (this != &other) {
        Vector temp(other);
        std::swap(data_, temp.data_);
        std::swap(size_, temp.size_);
        std::swap(capacity_, temp.capacity_);
      }
      return *this;
    }

    void push_back(const T& value) {
      if (size_ == capacity_) {
        size_t new_cap = capacity_ ? capacity_ * 2 : 8;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < size_; ++i) {
          new_data[i] = data_[i];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
      }
      data_[size_++] = value;
    }

    T& operator[](size_t index) {
      return data_[index];
    }

    const T& operator[](size_t index) const {
      return data_[index];
    }

    size_t size() const {
      return size_;
    }
  };

  void sort_strings(Vector<std::string>& vec);
  void sort_uints(Vector<unsigned int>& vec);

}

#endif
