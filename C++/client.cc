#include "ActiveObject.h"
#include "TypePair.h"

#include <iostream>

using namespace std;

int main() {
  auto *obj1 = new ActiveObject<TypePair<int>>(new TypePair<int>(1, 2));
  cout << obj1->call(&TypePair<int>::sum) << "\n";
  obj1->call(&TypePair<int>::add, 5);
  cout << obj1->call(&TypePair<int>::prod) << "\n";

  delete obj1;
}
