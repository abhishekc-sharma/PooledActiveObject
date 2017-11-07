#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include <functional>
#include <future>
#include <queue>
#include <type_traits>

template <typename T>
class ActiveObject {
public:
  // constructor
  ActiveObject(T *obj);
  // copy constructor
  // assignment
  // move
  // destructor
  ~ActiveObject();
  // call
  template <typename Function, typename... Args>
  auto call(Function func, Args&& ... args) -> std::result_of_t<Function&&(T*, Args&&...)>;
  // asynchronous call
  template<typename Function, typename... Args>
  auto async_call(Function func, Args&& ... args) ->

private:
  // scheduler
  T *ptr;
};

template <typename T>
ActiveObject<T>::ActiveObject(T *obj) : ptr(obj) {}

template <typename T>
ActiveObject<T>::~ActiveObject() {
  delete ptr;
}

template <typename T>
template <typename Function, typename... Args>
auto ActiveObject<T>::call(Function func, Args&& ... args) -> std::result_of_t<Function&&(T*, Args&&...)> {
  return (ptr->*func)(args...);
}

#endif
