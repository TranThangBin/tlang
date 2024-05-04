#include "datastructure/array-list.h"
#include "datastructure/queue.h"
#include "runtime/environment.h"
#include "runtime/runtime-value.h"
#include <iostream>
#include <memory>

using namespace std;

int main(int argc, char *argv[]) {
  ArrayList<int> arr = ArrayList<int>(0);

  arr.Push(10);
  arr.Push(2);
  arr.Push(-1);
  arr.Push(-5);
  arr.Push(20);
  arr.Push(555);

  cout << arr.Count() << endl;

  for (int i = 0; i < arr.Count(); i++) {
    cout << arr.At(i) << endl;
  }

  return 0;
}
