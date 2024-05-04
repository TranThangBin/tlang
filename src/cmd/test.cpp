#include "datastructure/hashmap.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    return 1;
  }

  Hashmap<int> test = Hashmap<int>(50);
  test.Insert("Hello", 30);
  test.Insert("world", 20);
  test.Insert("foo", 10);
  test.Insert("bar", 2);
  test.Insert("baz", 3);
  test.Insert("buz", 5);
  test.Insert("cat", 1);
  test.Insert("bat", 2);
  test.Insert("rat", 3);
  test.Insert("act", 4);
  test.Insert("tab", 5);

  Pair<int> p = Pair<int>("", 0);

  if (test.Find(argv[1], &p)) {
    cout << p.key << " " << p.value;
  }

  return 0;
}
