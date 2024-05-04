#ifndef QUEUE_H
#define QUEUE_H

#include "datastructure/list-node.h"
#include <stdexcept>

template <typename T> struct Queue {
private:
  ListNode<T> *front, *rear;

public:
  Queue<T>() : front(nullptr), rear(nullptr) {}

  bool IsEmpty() { return front == nullptr && rear == nullptr; }

  T GetFront() {
    if (IsEmpty()) {

      throw std::runtime_error("cannot get front because the queue is empty");
    }
    return front->data;
  }

  void Enqueue(T data) {
    if (IsEmpty()) {
      front = rear = new ListNode<T>(data, nullptr);
      return;
    }

    rear->next = new ListNode<T>(data, nullptr);
    rear = rear->next;
  }

  T Dequeue() {
    if (IsEmpty()) {
      throw std::runtime_error("cannot dequeue because the queue is empty");
    }

    ListNode<T> *temp = front;

    T data = temp->data;

    front = front->next;

    delete temp;

    temp = nullptr;

    return data;
  }

  ~Queue<T>() {
    while (!IsEmpty()) {
      Dequeue();
    }
  }
};

#endif // !QUEUE_H
