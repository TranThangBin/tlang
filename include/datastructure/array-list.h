#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <stdexcept>
template <typename T> struct ArrayList {
private:
  T *arr;
  int capacity;
  int count;

public:
  ArrayList<T>(int capacity)
      : capacity(capacity), arr(new T[capacity]), count(0) {}

  void Push(T elem) {
    if (count >= capacity) {
      capacity = count + 10;

      T *newArr = new T[capacity];

      for (int i = 0; i < count; i++) {
        newArr[i] = arr[i];
      }

      delete[] arr;

      arr = newArr;
    }

    arr[count++] = elem;
  }

  T At(int index) {
    if (index < 0 || index >= count) {
      throw std::runtime_error("index out of range");
    }

    return arr[index];
  }

  int Count() { return count; }

  ~ArrayList<T>() { delete[] arr; }
};

#endif // !ARRAY_LIST_H
