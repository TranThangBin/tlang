#include "datastructure/hashmap.h"
#include "datastructure/queue.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  Queue<int> test;

  test.Enqueue(10);
  test.Enqueue(20);
  test.Enqueue(-1);
  test.Enqueue(-5);
  test.Enqueue(2);

  while (!test.IsEmpty()) {
    cout << test.Dequeue() << endl;
    cout << test.IsEmpty() << endl;
  }

  return 0;
}
