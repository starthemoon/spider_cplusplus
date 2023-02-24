#include <boost/array.hpp>
#include <asio.hpp>
#include <iostream>

#define HTTPPORT "80"

using asio::ip::tcp;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;

int main(int argc, char **argv) {
    try {
        if (argc != 3) {
            cerr << "Usage: website_vister <host> <service>" << endl;
            return 1;
        }

        // 1. create io_context
        asio::io_context ioContext;
        // 2. create tcp resolver wrapping io_context, like dns
        tcp::resolver resolver(ioContext);
        // 3. get ip addresses by resolving host name
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        for (auto it = endpoints.begin(); it != endpoints.end(); ++it) {
            cout << "host name: " << it->host_name() << ", service name: " << it->service_name();
            cout << ", endpoint: " << it->endpoint() << endl;
        }
        // 4. create socket wrapping io_context
        tcp::socket socket(ioContext);
        // 5. connect socket and each ip address in endpoints to create a connection
        auto res = asio::connect(socket, endpoints);
        cout << res << endl;

        while (true) {
            asio::error_code errCode;
            boost::array<char, 128> buf;
            string readStr;

            // write data
            
            auto len = socket.write_some(asio::buffer(readStr), errCode);
            if (errCode == asio::error::eof) {
                break;
            } else if (errCode) {
                throw asio::system_error(errCode);
            }

            // read data
            len = socket.read_some(asio::buffer(buf), errCode);
            if (errCode == asio::error::eof) {
                break;
            } else if (errCode) {
                throw asio::system_error(errCode);
            }

            cout.write(buf.data(), len);
        }
    } catch (exception &e) {
        cerr << e.what() << endl;
    }
    return 0;
}