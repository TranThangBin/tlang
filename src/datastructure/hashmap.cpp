#include "datastructure/hashmap.h"
#include <string>

int GetHashCode(std::string key) {
  int hashCode = 0;

  int keyLen = key.length();

  int powerOf31 = 1;

  for (int i = keyLen - 1; i >= 0; i--) {
    hashCode += key[i] * powerOf31;
    powerOf31 *= 31;
  }

  return hashCode;
}
