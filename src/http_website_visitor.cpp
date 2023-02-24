#include <asio.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>
#include <vector>

// ssl for https, using openssl
#include <asio/ssl.hpp>

#define HTTPPORT "80"
#define HTTPSPORT "443"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::vector;
using std::exception;
using asio::ip::tcp;
using asio::io_context;

void handleHTTPSRequestAndResponse(io_context &ic, asio::ip::basic_resolver_results<tcp> &endpoints,
                                   asio::streambuf &request, asio::streambuf &response) {
    // construct ssl context
    asio::ssl::context sslContext = asio::ssl::context(asio::ssl::context::sslv23_client);
    // construct ssl stream
    asio::ssl::stream sslStream = asio::ssl::stream<tcp::socket>(ic, sslContext);
    // construct ssl connection based on tcp connection
    asio::connect(sslStream.lowest_layer(), endpoints);
    sslStream.handshake(asio::ssl::stream_base::client);

    // send request to host
    asio::write(sslStream, request);

    // read response
    asio::error_code ec;
    std::istream responseStream(&response);
    auto len = asio::read(sslStream, response, ec);
    if (ec != asio::error::eof) {
        throw asio::system_error(ec);
    }

    // 建立socket
    // #include <openssl/ssl.h>
    // #include <openssl/err.h>
    // int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // if (client < 0) {
    //     cerr << "create socket failed" << endl;
    //     throw exception();
    // }
    // string host = "www.baidu.com";
    // ushort port = 443;
    // hostent *ip = gethostbyname2(host.c_str(), AF_INET);
    // sockaddr_in sin;
    // sin.sin_addr = *(in_addr *)ip->h_addr_list[0];
    // sin.sin_family = htons(port);
    // sin.sin_port = AF_INET;
    // if (connect(client, (sockaddr *)&sin, sizeof(sin)) < 0) {
    //     cerr << "create tcp connection failed" << endl;
    //     throw exception();
    // }
    // 添加SSL的加密/hash算法
    // SSL_library_init(); // SSLeay_add_ssl_algorithms();
    // // 选择SSL的算法，客户端选择client，服务端选择server
    // const SSL_METHOD *meth = SSLv23_client_method();
    // if (meth == nullptr) {
    //     cerr << "craete SSL method failed" << endl;
    //     throw exception();
    // }
    // // 建立新的SSL context
    // SSL_CTX * ctx = SSL_CTX_new(meth);
    // if (ctx == nullptr) {
    //     cerr << "craete SSL context failed" << endl;
    //     throw exception();
    // }
    // // 建立新的SSL
    // SSL * ssl = SSL_new(ctx);
    // if (ssl == nullptr) {
    //     cerr << "craete SSL failed" << endl;
    //     throw exception();
    // }
    // // 将SSL与TCP socket连接
    // auto res = SSL_set_fd(ssl, client);
    // if (res < 0) {
    //     cerr << "connect SSL and tcp socket failed" << endl;
    //     throw exception();
    // }
    // // 建立SSL连接
    // res = SSL_connect(ssl);
    // if (res < 0) {
    //     cerr << "create SSL connection failed" << endl;
    //     throw exception();
    // }
    // // SSL发送数据
    // std::istream requestStream(&request);
    // string sendData(asio::buffers_begin(request), asio::buffers_end(request));
    // res = SSL_write(ssl, sendData.c_str(), sendData.size());
    // if (res <= 0) {
    //     cerr << "ssl send data failed" << endl;
    //     throw exception();
    // }
    // // SSL接受数据
    // #define RECVLEN 1024
    // char recvData[RECVLEN];
    // std::stringstream ss;
    // while (SSL_read(ssl, recvData, RECVLEN) > 0) {
    //     ss << recvData;
    // }
    // cout << ss.str() << endl;
    // // 关闭SSL
    // res = SSL_shutdown(ssl);
    // if (res < 0) {
    //     cerr << "shutdown ssl failed" << endl;
    //     throw exception();
    // }
    // // 释放SSL
    // SSL_free(ssl);
    // // 释放SSL会话环境/上下文
    // SSL_CTX_free(ctx);
    // // 关闭tcp socket
    // res = close(client);
    // if (res < 0) {
    //     cerr << "close tcp socket failed" << endl;
    //     throw exception();
    // }
}

void handleHTTPRequestAndResponse(tcp::socket &sock, asio::ip::basic_resolver_results<tcp> &endpoints,
                                  asio::streambuf &request, asio::streambuf &response) {
    // construct tcp connection
    asio::connect(sock, endpoints);

    // send request to host
    asio::write(sock, request);

    // read http response
    asio::error_code ec;
    std::istream responseStream(&response);
    auto len = asio::read(sock, response, ec);
    if (ec != asio::error::eof) {
        throw asio::system_error(ec);
    }
    // asio::read_until(sock, response, "\r\n");
    // string httpVersion;
    // responseStream >> httpVersion;
    // uint statusCode;
    // responseStream >> statusCode;
    // string statusMessage;
    // std::getline(responseStream, statusMessage);
    // if (!responseStream || httpVersion.substr(0, 5) != "HTTP/") {
    //     cerr << "Invalid response\n";
    //     return 6;
    // }
    // if (statusCode != 200) {
    //     cerr << "Response returned with status code " << statusCode << endl;
    //     return 7;
    // }
    // asio::read_until(sock, response, "\r\n\r\n");
    // string header;
    // while (getline(responseStream, header) && header != "\r") {
    //     cout << header << endl;
    // }
    // cout << endl;
    // if (response.size()) cout << &response;

    // while (asio::read(sock, response, asio::transfer_at_least(1), ec)) {
    //     cout << &response;
    // }
    // if (ec != asio::error::eof) {
    //     throw asio::system_error(ec);
    // }
    // cout << endl;
}

bool isProtocolChoiceValid(char *choice) {
    return *(choice + 1) == '\0' && (*choice == '1' || *choice == '0');
}

bool isNameValid(string & name) {
    for (auto na : name) {
        if ((na < 'a' || na > 'z') &&
            (na < 'A' || na > 'Z') &&
            (na < '0' && na > '9')) return false;
    }
    return true;
}

vector<string> splitPathBySlash(char *path) {
    vector<string> res;
    string str;
    while (*path != '\0') {
        if (*path == '/') {
            res.emplace_back(str);
            str.clear();
        } else {
            str += *path;
        }
        ++path;
    }
    if (str.size()) res.emplace_back(str);
    return res;
}

bool isPathValid(char *path) {
    if (*path != '/') return false;
    auto paths = splitPathBySlash(path);
    if (paths.size() == 1 && paths[0] == "") return true;
    for (auto name : paths) {
        if (name.empty() || !isNameValid(name)) return false;
    }
    return true;
}

vector<string> splitHostByDot(char *host) {
    vector<string> res;
    string str;
    while (*host != '\0') {
        if (*host == '.') {
            res.emplace_back(str);
            str.clear();
        } else {
            str += *host;
        }
        ++host;
    }
    res.emplace_back(str);
    return res;
}

bool isHostValid(char *host) {
    auto names = splitHostByDot(host);
    for (auto name : names) {
        if (name.empty() || !isNameValid(name)) return false;
    }
    return true;
}

class Visitor {
public:
    Visitor(string h, string pa, string po): host(h), path(pa), port(po) {
        // TODO: resolve, connect, send, read
    }

private:
    asio::ip::basic_resolver_results<tcp> resolve() {
        tcp::resolver resolver(ic);
        asio::ip::basic_resolver_results<tcp> endpoints = resolver.resolve(host, port);
        return endpoints;
    }

    string host, path, port;
    io_context ic;
};

int main(int argc, char ** argv) {
    try {
        if (argc != 4) {
            cerr << "Usage: website_visiter <host> <path> <protocol>[0|1]" << endl;
            return 1;
        } else if (!isHostValid(argv[1])) {
            cerr << "host must be like \"abc.def.ghi\"" << endl;
            return 2;
        } else if (!isPathValid(argv[2])) {
            cerr << "path must be like \"/a/b/c/d\"" << endl;
            return 3;
        } else if (!isProtocolChoiceValid(argv[3])) {
            cerr << "protocol must be \"1\" or \"0\"" << endl;
        }
        string host(argv[1]), path(argv[2]), choice(argv[3]);
        string port = choice == "0" ? HTTPPORT : HTTPSPORT;

        io_context ic;
        tcp::resolver resolver(ic);
        asio::ip::basic_resolver_results endpoints = resolver.resolve(host, port);

        tcp::socket sock(ic);
        asio::error_code ec;

        // write http request
        asio::streambuf request, response;
        std::ostream requestStream(&request);
        requestStream << "GET " << path << " HTTP/1.1\r\n";
        requestStream << "Host: " << host << "\r\n";
        requestStream << "Accept: */*\r\n";
        requestStream << "Connection: close\r\n\r\n";

        // 1. send request and read response using tcp socket only
        if (choice == "0") {
            handleHTTPRequestAndResponse(sock, endpoints, request, response);
        }
        // 2. send request and read response using SSL
        else {
            handleHTTPSRequestAndResponse(ic, endpoints, request, response);
        }

        // print response
        cout << &response << endl;
    } catch (exception & e) {
        cerr << e.what() << endl;
        return 10;
    }
    return 0;
}