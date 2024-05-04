#ifndef LIST_NODE_H
#define LIST_NODE_H

template <typename T> struct ListNode {
  T data;
  ListNode *next;
  ListNode<T>(T data, ListNode *next) : data(data), next(next) {}
};

#endif // !LIST_NODE_H
