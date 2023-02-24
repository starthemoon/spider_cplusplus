#include "myTemplate.h"

template <typename T>
void MyClass<T>::getData() {
    cout << typeid(T).name() << endl;
}

template class MyClass<int>;
template class MyClass<double>;