#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H

#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <type_traits>
#include <thread>
#include "thread_pool.h"

namespace dp {
  template <typename T>
  class active_object {
  public:

     // move constructor
    active_object(std::unique_ptr<T>&& ptr);

     // synchronous call
    template <typename Function, typename... Args>
    auto call(Function func, Args&& ... args) -> std::result_of_t<Function&&(T&, Args&&...)>;

    // asynchronous call
    template <typename Function, typename... Args>
    auto async_call(thread_pool& pool, Function func, Args&& ... args) -> std::future<std::result_of_t<Function&&(T&, Args&&...)>>;

  private:
    template <typename ResultType>
    auto make_async_call(thread_pool& pool, std::function<ResultType()> func, std::false_type);

    template <typename ResultType>
    auto make_async_call(thread_pool& pool, std::function<ResultType()> func, std::true_type);

    std::unique_ptr<T> ptr;
  };

  template <typename T>
  active_object<T>::active_object(std::unique_ptr<T>&& ptr) : ptr(std::move(ptr)) {}

  template <typename T>
  template <typename Function, typename... Args>
  auto active_object<T>::call(Function func, Args&& ... args) -> std::result_of_t<Function&&(T&, Args&&...)> {
    return ((ptr.get())->*func)(args...);
  }

  template<typename T>
  template <typename ResultType>
  auto active_object<T>::make_async_call(thread_pool& pool, std::function<ResultType()> func, std::false_type) {
    auto result_promise = std::shared_ptr<std::promise<ResultType>>(new std::promise<ResultType>());
    auto result_future = result_promise->get_future();

    pool.submit([promise=std::move(result_promise), function=std::move(func)]() mutable {
      promise->set_value(function());
    });

    return result_future;
  }

  template<typename T>
  template <typename ResultType>
  auto active_object<T>::make_async_call(thread_pool& pool, std::function<ResultType()> func, std::true_type) {
    auto result_promise = std::shared_ptr<std::promise<ResultType>>(new std::promise<ResultType>());
    auto result_future = result_promise->get_future();

    pool.submit([promise=std::move(result_promise), function=std::move(func)]() mutable {
      function();
      promise->set_value();
    });

    return result_future;
  }

  template <typename T>
  template <typename Function, typename... Args>
  auto active_object<T>::async_call(thread_pool& pool, Function func, Args&& ... args) -> std::future<std::result_of_t<Function&&(T&, Args&&...)>> {
    using ResultType = std::result_of_t<Function&&(T&, Args&&...)>;
    constexpr bool is_void = std::is_same<ResultType, void>::value;
    auto bound_call = std::bind(func, ptr.get(), args...);
    return make_async_call<ResultType>(pool, bound_call, std::is_same<ResultType, void>());
  }

  template <typename T, typename... Args>
  auto make_active(Args&& ... args) {
    return std::make_shared<active_object<T>>(std::make_unique<T>(args...));
  }
}

#endif
