#include "ActiveObject.h"
#include "TypePair.h"

#include <iostream>

using namespace std;

int main() {
  auto obj1 = std::make_shared<ActiveObject<TypePair<int>>>(std::make_unique<TypePair<int>>(1,2));
  cout << obj1->call(&TypePair<int>::sum) << "\n";
  //obj1->call(&TypePair<int>::add, 5);
  cout << obj1->call(&TypePair<int>::prod) << "\n";

  auto result_future_1 = obj1->async_call(&TypePair<int>::sum);
  result_future_1.wait();
  cout << result_future_1.get() << "\n";
  auto result_future_2 = obj1->async_call(&TypePair<int>::add, 5);
  result_future_2.wait();
  auto result_future_3 = obj1->async_call(&TypePair<int>::prod);
  result_future_3.wait();
  cout << result_future_3.get() << "\n";
}
