#define n m
#include "iostream"
#include "atomic"
#include "cassert"
#include "thread"

using namespace std;

template <typename T>
class MyClass {
private:
    int privateData;
protected:
    int protectedData;
public:
    int publicData;
    
    MyClass(T t) {}

    void getData();
};