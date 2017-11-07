#ifndef MY_CLASS_H
#define MY_CLASS_H

template <typename T>
class TypePair {
public:
  TypePair(T a, T b);
  T sum();
  T prod();
  void add(T x);
private:
  int a;
  int b;
};

template <typename T>
TypePair<T>::TypePair(T a, T b) : a(a), b(b) {}

template <typename T>
T TypePair<T>::sum() {
  return a + b;
}

template <typename T>
T TypePair<T>::prod() {
  return a * b;
}

template <typename T>
void TypePair<T>::add(T x) {
  a += x;
  b += x;
}

#endif
