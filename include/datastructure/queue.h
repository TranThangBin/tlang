#ifndef QUEUE_H
#define QUEUE_H

#include "datastructure/list-node.h"
#include <stdexcept>

template <typename T> struct Queue {
private:
  ListNode<T> *front, *rear;

public:
  Queue<T>() : front(nullptr), rear(nullptr) {}

  T Enqueue(T data) {
    if (front == nullptr && rear == nullptr) {
      front = rear = new ListNode<T>(data, nullptr);
      return rear;
    }

    rear->next = new ListNode<T>(data, nullptr);
    return rear;
  }

  T Dequeue() {
    if (front == nullptr) {
      throw std::runtime_error("cannot dequeue because the queue is empty");
    }

    ListNode<T> *temp = front;

    front = front->next;

    delete temp;

    return front;
  }

  ~Queue<T>() {
    while (front != nullptr) {
      ListNode<T> *temp = front;

      front = front->next;

      delete temp;

      temp = nullptr;
    }
  }
};

#endif // !QUEUE_H
