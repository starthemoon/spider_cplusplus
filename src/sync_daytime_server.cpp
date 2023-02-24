#include <asio.hpp>
#include <iostream>
#include <ctime>

#define DAYTIMEPORT 13

using asio::ip::tcp;
using std::string;
using std::exception;
using std::cerr;
using std::endl;

string getCurrentTimeString() {
    using namespace std;
    time_t now = time(nullptr);
    return ctime(&now);
}

int main() {
    try {
        // 1. create io_context
        asio::io_context ioContext;
        // 2. create accpetor wrapping io_context and (ip, port), waiting to serve connection
        tcp::acceptor acceptor(ioContext, tcp::endpoint(tcp::v4(), DAYTIMEPORT));
        while (true) {
            // 3.1 create socket(file descriptor) for each connection
            tcp::socket sock(ioContext);
            // 3.2 accpetor places the new connection into the socket
            acceptor.accept(sock);

            string message = getCurrentTimeString();

            // 3.3 write message with err_code to set
            asio::error_code ignoredErrCode;
            sock.write_some(asio::buffer(message), ignoredErrCode);
        }
    } catch (exception &e) {
        cerr << e.what() << endl;
    }

    return 0;
}