#include <asio.hpp>
#include <boost/array.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>

#define DAYTIMEPORT 13

using asio::ip::tcp;
using asio::ip::udp;
using std::exception;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

string getCurrentTimeString() {
    using namespace std;
    time_t now = time(nullptr);
    return ctime(&now);
}

class TcpConnenction: public boost::enable_shared_from_this<TcpConnenction> {
public:
    typedef boost::shared_ptr<TcpConnenction> pointer;

    static pointer create(asio::io_context & ic) {
        return pointer(new TcpConnenction(ic));
    }

    tcp::socket & getSocket() {
        return sock;
    }

    void start() {
        message = getCurrentTimeString();

        asio::async_write(sock, asio::buffer(message),
            boost::bind(&TcpConnenction::handleWrite, this)
        );
    }

private:
    TcpConnenction(asio::io_context & ic): sock(ic) { }

    void handleWrite() { }

    string message;
    tcp::socket sock;
};

class AsyncTcpServer {
public:
    AsyncTcpServer(asio::io_context & ic): ioContext(ic), acceptor(ic, tcp::endpoint(tcp::v4(), DAYTIMEPORT)) {
        startAccpet();
    }

private:
    void startAccpet() {
        auto tc = TcpConnenction::create(ioContext);

        acceptor.async_accept(tc->getSocket(),
            boost::bind(&AsyncTcpServer::handleAccept, this, tc, boost::placeholders::_1)
        );
    }

    void handleAccept(TcpConnenction::pointer tc, const asio::error_code & ec) {
        if (!ec) {
            tc->start();
        }

        startAccpet();
    }

    asio::io_context & ioContext;
    tcp::acceptor acceptor;
};

// UDP
class AsyncUdpServer {
public:
    AsyncUdpServer(asio::io_context& ic): socket_(ic, udp::endpoint(udp::v4(), 13)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(
            asio::buffer(recvBuf), remote_endpoint_,
            boost::bind(&AsyncUdpServer::handle_receive, this, boost::placeholders::_1)
        );
    }

    void handle_receive(const asio::error_code& error) {
        if (!error) {
            boost::shared_ptr<std::string> message(new std::string(getCurrentTimeString()));

            socket_.async_send_to(asio::buffer(*message), remote_endpoint_,
                boost::bind(&AsyncUdpServer::handle_send, this, message)
            );

        }
        start_receive();
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/) { }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1> recvBuf;
};

int main(int argc, char **argv) {
    try {
        asio::io_context ioContext;
        AsyncTcpServer ts(ioContext);
        AsyncUdpServer us(ioContext);
        ioContext.run();
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    return 0;
}