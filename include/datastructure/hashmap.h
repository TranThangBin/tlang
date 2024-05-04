#ifndef HASHMAP_H
#define HASHMAP_H

#include "datastructure/list-node.h"
#include <stdexcept>
#include <string>

int GetHashCode(std::string);

template <typename TValue> struct Pair {
  std::string key;
  TValue value;
  Pair<TValue>(std::string key, TValue value) : key(key), value(value) {}
};

template <typename TValue> struct Hashmap {
private:
  ListNode<Pair<TValue>> **table;
  int size;

public:
  Hashmap<TValue>(int size)
      : size(size), table(new ListNode<Pair<TValue>> *[size]) {}

  void Insert(std::string key, TValue value) {
    int hashCode = GetHashCode(key);

    int index = hashCode % size;

    ListNode<Pair<TValue>> *head = table[index];

    if (head == nullptr) {
      table[index] =
          new ListNode<Pair<TValue>>(Pair<TValue>(key, value), nullptr);
      return;
    }

    while (head != nullptr) {
      Pair<TValue> pair = head->data;

      if (pair.key == key) {
        throw std::runtime_error("duplicate key " + key + " in hashmap");
      }

      if (head->next == nullptr) {
        break;
      }

      head = head->next;
    }

    head->next = new ListNode<Pair<TValue>>(Pair<TValue>(key, value), nullptr);
  }

  bool Find(std::string key, Pair<TValue> *value) {
    int hashCode = GetHashCode(key);

    int index = hashCode % size;

    ListNode<Pair<TValue>> *head = table[index];

    if (head == nullptr) {
      return false;
    }

    while (head != nullptr) {
      Pair<TValue> pair = head->data;

      if (pair.key == key) {
        *value = pair;
        return true;
      }

      head = head->next;
    }

    return false;
  }

  ~Hashmap<TValue>() {

    for (int i = 0; i < size; i++) {
      ListNode<Pair<TValue>> *head = table[i];

      while (head != nullptr) {
        ListNode<Pair<TValue>> *temp = head;

        head = head->next;

        delete temp;

        temp = nullptr;
      }
    }

    delete[] table;

    table = nullptr;
  }
};

#endif // !HASHMAP_H
