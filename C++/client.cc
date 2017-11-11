#include "active_object.h"
#include "thread_pool.h"
#include "TypePair.h"
#include <iostream>

using namespace std;

int main() {

  dp::thread_pool pool;

  auto obj1 = dp::make_active<TypePair<int>>(1,2);//std::make_shared<dp::active_object<TypePair<int>>>(std::make_unique<TypePair<int>>(1,2));
  cout << obj1->call(&TypePair<int>::sum) << "\n";
  //obj1->call(&TypePair<int>::add, 5);
  cout << obj1->call(&TypePair<int>::prod) << "\n";

  auto result_future_1 = obj1->async_call(std::ref(pool), &TypePair<int>::sum);
  auto result_future_3 = obj1->async_call(std::ref(pool),&TypePair<int>::prod);
  result_future_1.wait();
  cout << result_future_1.get() << "\n";

  result_future_3.wait();
  cout << result_future_3.get() << "\n";
}
