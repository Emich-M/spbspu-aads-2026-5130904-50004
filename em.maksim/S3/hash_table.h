#ifndef EM_HASH_TABLE_H
#define EM_HASH_TABLE_H

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace em {

template <class Key, class Value, class Hash, class Equal>
class HashTable {
public:
  enum class State {
    kEmpty,
    kOccupied,
    kTombstone
  };

  struct Slot {
    Key key;
    Value value;
    State state;

    Slot()
      : state(State::kEmpty) {
    }
  };

private:
  Slot* slots_;
  size_t capacity_;
  size_t size_;
  Hash hasher_;
  Equal equal_;

  size_t findSlot(const Key& key) const {
    size_t idx = hasher_(key) % capacity_;
    size_t start = idx;
    while (slots_[idx].state != State::kEmpty) {
      if (slots_[idx].state == State::kOccupied && equal_(slots_[idx].key, key)) {
        return idx;
      }
      idx = (idx + 1) % capacity_;
      if (idx == start) {
        throw std::overflow_error("HashTable is full");
      }
    }
    return idx;
  }

  size_t findInsertSlot(const Key& key, bool& exists) const {
    size_t idx = hasher_(key) % capacity_;
    size_t start = idx;
    size_t tombstone_idx = capacity_;
    while (slots_[idx].state != State::kEmpty) {
      if (slots_[idx].state == State::kTombstone) {
        if (tombstone_idx == capacity_) {
          tombstone_idx = idx;
        }
      } else if (equal_(slots_[idx].key, key)) {
        exists = true;
        return idx;
      }
      idx = (idx + 1) % capacity_;
      if (idx == start) {
        throw std::overflow_error("HashTable is full");
      }
    }
    exists = false;
    return (tombstone_idx != capacity_) ? tombstone_idx : idx;
  }

public:
  explicit HashTable(size_t capacity = 16)
    : slots_(new Slot[capacity]),
      capacity_(capacity),
      size_(0),
      hasher_(),
      equal_() {
  }

  ~HashTable() {
    delete[] slots_;
  }

  HashTable(const HashTable& other)
    : slots_(new Slot[other.capacity_]),
      capacity_(other.capacity_),
      size_(0),
      hasher_(other.hasher_),
      equal_(other.equal_) {
    for (size_t i = 0; i < capacity_; ++i) {
      if (other.slots_[i].state == State::kOccupied) {
        add(other.slots_[i].key, other.slots_[i].value);
      }
    }
  }

  HashTable& operator=(const HashTable& other) {
    if (this != &other) {
      HashTable temp(other);
      std::swap(slots_, temp.slots_);
      std::swap(capacity_, temp.capacity_);
      std::swap(size_, temp.size_);
    }
    return *this;
  }

  void add(const Key& key, const Value& value) {
    if (size_ >= capacity_) {
      throw std::overflow_error("HashTable is full");
    }
    bool exists = false;
    size_t idx = findInsertSlot(key, exists);
    if (!exists) {
      slots_[idx].key = key;
      slots_[idx].value = value;
      slots_[idx].state = State::kOccupied;
      ++size_;
    } else {
      slots_[idx].value = value;
    }
  }

  Value drop(const Key& key) {
    size_t idx = findSlot(key);
    if (slots_[idx].state == State::kOccupied) {
      Value val = slots_[idx].value;
      slots_[idx].state = State::kTombstone;
      --size_;
      return val;
    }
    throw std::out_of_range("Key not found");
  }

  bool has(const Key& key) const {
    size_t idx = findSlot(key);
    return slots_[idx].state == State::kOccupied;
  }

  Value& at(const Key& key) {
    size_t idx = findSlot(key);
    if (slots_[idx].state == State::kOccupied) {
      return slots_[idx].value;
    }
    throw std::out_of_range("Key not found");
  }

  const Value& at(const Key& key) const {
    size_t idx = findSlot(key);
    if (slots_[idx].state == State::kOccupied) {
      return slots_[idx].value;
    }
    throw std::out_of_range("Key not found");
  }

  void rehash(size_t new_capacity) {
    if (new_capacity <= capacity_) {
      return;
    }
    Slot* new_slots = new Slot[new_capacity];
    size_t old_capacity = capacity_;
    Slot* old_slots = slots_;
    slots_ = new_slots;
    capacity_ = new_capacity;
    size_ = 0;
    for (size_t i = 0; i < old_capacity; ++i) {
      if (old_slots[i].state == State::kOccupied) {
        add(old_slots[i].key, old_slots[i].value);
      }
    }
    delete[] old_slots;
  }

  void clear() {
    for (size_t i = 0; i < capacity_; ++i) {
      slots_[i].state = State::kEmpty;
    }
    size_ = 0;
  }

  size_t size() const {
    return size_;
  }

  size_t capacity() const {
    return capacity_;
  }

  bool empty() const {
    return size_ == 0;
  }

  class Iterator {
  private:
    Slot* slots_;
    size_t capacity_;
    size_t index_;

    void advance() {
      while (index_ < capacity_ && slots_[index_].state != State::kOccupied) {
        ++index_;
      }
    }

  public:
    Iterator(Slot* slots, size_t capacity, size_t index)
      : slots_(slots), capacity_(capacity), index_(index) {
      advance();
    }

    std::pair<const Key&, Value&> operator*() {
      return std::pair<const Key&, Value&>(slots_[index_].key, slots_[index_].value);
    }

    Iterator& operator++() {
      ++index_;
      advance();
      return *this;
    }

    bool operator==(const Iterator& other) const {
      return index_ == other.index_;
    }

    bool operator!=(const Iterator& other) const {
      return !(*this == other);
    }
  };

  class ConstIterator {
  private:
    const Slot* slots_;
    size_t capacity_;
    size_t index_;

    void advance() {
      while (index_ < capacity_ && slots_[index_].state != State::kOccupied) {
        ++index_;
      }
    }

  public:
    ConstIterator(const Slot* slots, size_t capacity, size_t index)
      : slots_(slots), capacity_(capacity), index_(index) {
      advance();
    }

    std::pair<const Key&, const Value&> operator*() const {
      return std::pair<const Key&, const Value&>(slots_[index_].key, slots_[index_].value);
    }

    ConstIterator& operator++() {
      ++index_;
      advance();
      return *this;
    }

    bool operator==(const ConstIterator& other) const {
      return index_ == other.index_;
    }

    bool operator!=(const ConstIterator& other) const {
      return !(*this == other);
    }
  };

  Iterator begin() {
    return Iterator(slots_, capacity_, 0);
  }

  Iterator end() {
    return Iterator(slots_, capacity_, capacity_);
  }

  ConstIterator begin() const {
    return ConstIterator(slots_, capacity_, 0);
  }

  ConstIterator end() const {
    return ConstIterator(slots_, capacity_, capacity_);
  }

  ConstIterator cbegin() const {
    return begin();
  }

  ConstIterator cend() const {
    return end();
  }
};

} // namespace em

#endif // EM_HASH_TABLE_H
