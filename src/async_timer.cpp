#include <asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

using asio::chrono::seconds;
using asio::chrono::milliseconds;
using asio::io_context;
using asio::steady_timer;
using asio::strand;
using std::cout;
using std::endl;
using std::cerr;
using std::exception;

class Printer {
public:
    Printer(io_context & ic): st(asio::make_strand(ic)), t1(ic, seconds(1)), t2(ic, seconds(1)) {
        t1.async_wait(asio::bind_executor(st, boost::bind(&Printer::print1, this)));
        t1.async_wait(asio::bind_executor(st, boost::bind(&Printer::print2, this)));
    }

private:
    void print1() {
        if (count < 10) {
            cout << "Timer 1: " << count << endl;
            ++count;
            t1.expires_from_now(milliseconds(rand() % 100));
            t1.async_wait(asio::bind_executor(st, boost::bind(&Printer::print1, this)));
        }
    }

    void print2() {
        if (count < 10) {
            cout << "Timer 2: " << count << endl;
            ++count;
            t2.expires_after(milliseconds(rand() % 100));
            t2.async_wait(asio::bind_executor(st, boost::bind(&Printer::print2, this)));
        }
    }

    steady_timer t1;
    steady_timer t2;
    strand<io_context::executor_type> st;
    int count = 0;
};

int main(int argc, char **argv) {
    try {
        io_context ic;
        Printer p(ic);
        asio::thread t(boost::bind(&io_context::run, &ic));
        ic.run();
        t.join();
    } catch (exception & e) {
        cerr << e.what() << endl;
    }
    return 0;
}