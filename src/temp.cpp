#include "myTemplate.h"
#include "unistd.h"
#include "wait.h"

struct Student {
    string name;
    int grade;
};

struct StudentDerived : Student {
    string introduction;
};

class Teacher {
public:
    string name;

    Teacher() {
        cout << "Teacher is created" << endl;
    }

    void say() {
        cout << "I am a teacher" << endl;
    }
};

class TeacherDerived: Teacher {
public:
    string introduction;
};

extern "C" {
    int add(const int, const int);
    void getString();
}

inline int getNumOfOnes(int n) {
    if (!n) return 0;
    return 1 + getNumOfOnes(n & (n - 1));
}

void func(void *i) {
    cout << "func1" << endl;
}

void func(int i) {
    cout << "func2" << endl;
}

// auto tryAutoParameter(auto a) {
//     auto i = a;
//     cout << i << endl;

//     return i;
// }

void tryUndefinedBehavior() {
    int a = 10;
    a = ++a + a++;
    cout << a << endl;

    int b = 20, c = 30, arr[10] = {0}, i = 0;
    int d = c + c++;
    arr[i] = ++i;
    cout << b++ << " " << b << endl;
    cout << d << endl;
    cout << arr[0] << " " << arr[1] << endl;
}

atomic<string*> ptr;
int data1;
atomic<int> x = 0, y = 0;

void producer() {
    string* p = new string("Hello");
    data1 = 42;
    ptr.store(p, memory_order_release);
}

void consumer() {
    string *p;
    while (!(p = ptr.load(memory_order_consume)));
    assert(*p == "Hello");
    assert(data1 == 42);
}

void thread1() {
    x.store(1, memory_order_seq_cst); // A
    y.store(1, memory_order_release); // B
}

void thread2() {
    auto r1 = y.fetch_add(1, memory_order_seq_cst); // C
    auto r2 = y.load(memory_order_relaxed); // D
    cout << "r1 = " << r1 << endl;
    cout << "r2 = " << r2 << endl;
}

void thread3() {
    y.store(3, memory_order_seq_cst); // E
    auto r3 = x.load(memory_order_seq_cst);
    cout << "r3 = " << r3 << endl;
}

class Base1 {
private:
    int i;
public:
    Base1(int i): i(i) {
        cout << "Base1 is constructed with i=" << i << endl;
    }

    virtual ~Base1() {
        cout << "Base1 is destructed with i=" << i << endl;
    }
};

class Base2 {
private:
    int i;
public:
    Base2(int i): i(i) {
        cout << "Base2 is constructed with i=" << i << endl;
    }

    virtual ~Base2() {
        cout << "Base2 is destructed with i=" << i << endl;
    }
};

class VirtualBase1 {
private:
    int i;
public:
    VirtualBase1(int i): i(i) {
        cout << "VirtualBase1 is constructed with i=" << i << endl;
    }

    virtual ~VirtualBase1() {
        cout << "VirtualBase1 is destructed with i=" << i << endl;
    }
};

class VirtualBase2 {
private:
    int i;
public:
    VirtualBase2(int i): i(i) {
        cout << "VirtualBase2 is constructed with i=" << i << endl;
    }

    virtual ~VirtualBase2() {
        cout << "VirtualBase2 is destructed with i=" << i << endl;
    }

    virtual VirtualBase2* test() {
        cout << "test in VirtualBase2" << endl;
        return this;
    }
};

class Derived1:public virtual VirtualBase2, public Base2, public Base1, public virtual VirtualBase1 {
private:
    VirtualBase2 vb2;
    VirtualBase1 vb1;
public:
    Derived1(): VirtualBase1(-1), VirtualBase2(-2), Base1(1), Base2(2), vb2(-22), vb1(-11) {
        cout << "Derived1 is constructed" << endl;
    }

    virtual ~Derived1() {
        cout << "Derived1 is destructed" << endl;
    }

    virtual Derived1* test() {
        cout << "test in Derived1" << endl;
        return this;
    }
};

class first {
public:
    first() :num(new int(0)) {
        cout << "construct!" << endl;
    }
    //移动构造函数
    first(first &&d) :num(d.num) {
        d.num = NULL;
        cout << "first move construct!" << endl;
    }
public:    //这里应该是 private，使用 public 是为了更方便说明问题
    int *num;
};
class second {
public:
    second() :fir() {}
    //用 first 类的移动构造函数初始化 fir
    second(second && sec) :fir(move(sec.fir)) {
        cout << "second move construct" << endl;
    }
public:    //这里也应该是 private，使用 public 是为了更方便说明问题
    first fir;
};

template <typename TF, typename... TS>
void forArgs(TF&& f, TS&&... ms) {
    (f(ms), ...);
}

template <typename... TS>
void minusTemplate(TS... rest) {
    (cout << "parameters: " << ... << rest) << endl;
    cout << "unary left fold: "   << (... - rest) << endl;
    cout << "unary right fold: "  << (rest - ...) << endl;
    cout << "binary left fold: "  << (0 - ... - rest) << endl;
    cout << "binary right fold: " << (rest - ... - 0) << endl;
}

int main() {
    // int arr[5] = {0, 1, 2, 3, 4};
    // int *p1 = arr, *p2 = &(arr[0]);
    // int (*p3) [5] = &arr;
    // int i = 0;
    // cout << p1[1] << " " << *(p1 + 1) << endl;
    // cout << p2[1] << " " << *(p2 + 1) << endl;
    // cout << *(p3[0] + 4) << endl;
    // cout << getNumOfOnes(10) << endl;
    // cout << (++i = 2) << endl;

    // Teacher *tea1 = nullptr;
    // tea1->say();
    // cout << tea1->name <<  endl;

    // func(nullptr);

    // int m = 10;
    // cout << n << endl;

    // const int a = 10;
    // int const b = 20;
    // int c = 30;
    // int * const pc = &c;
    // const int * const pa1 = &a;
    // int const * const pa2 = &a;
    // cout << *pc << endl << *pa1 << " " << *pa2 << endl;

    // cout << tryAutoParameter(10) << endl;

    // tryUndefinedBehavior();

    // thread t1(producer);
    // thread t2(consumer);
    // t1.join(), t2.join();

    // thread t1(thread1), t2(thread2), t3(thread3);
    // t1.join();
    // t2.join();
    // t3.join();

    // VirtualBase2* d = new Derived1();
    // d->test();
    // delete d;

    // MyClass mci(1);
    // mci.getData();
    // MyClass mcd(1.2);
    // mcd.getData();

    // second oth;
    // second oth2 = move(oth);
    // cout << *oth2.fir.num << endl;   //程序报运行时错误
    // return 0;

    // forArgs([](auto a){cout << a;}, 1, 'a', "how");
    // minusTemplate(1, 2, 3);

    int fd[2];
    if (pipe(fd) == -1) {
        cerr << "create pipe failed" << endl;
        exit(1);
    }
    pid_t  pid = fork();
    if (pid < 0) {
        cerr << "fork failed" << endl;
        exit(2);
    }
    if (pid == 0) { // child process
        char buf[255];
        sprintf(buf, "dad, i am born!, i am process %d", getpid());
        write(fd[1], buf, 255);
        return -1;
    } else if (pid > 0) {
        sleep(1);
        char buf[255];
        read(fd[0], buf, 255);
        cout << buf << endl;
        int status;
        pid_t cid = wait(&status);
        cout << "child process " << cid << " terminated with status " << status << endl;
        cout << "parent process " << getpid() << " is terminating" << endl;
    }

    return 0;
}

extern "C" {
    #include <stdio.h>

    int add(const int a, const int b) {
        return a + b;
    }

    void getString() {
        char a[] = {'a', 'b'};
        printf("%lu\n%s\n", sizeof(a), a);
    }
}