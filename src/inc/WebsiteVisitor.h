#ifndef _WEBSITE_VISITOR_H_
#define _WEBSITE_VISITOR_H_

#include "Utils.h"

// asio
#include <asio.hpp>

using asio::ip::tcp;
using asio::io_context;

// bind
#include <boost/bind/bind.hpp>

// ssl for https, using openssl
#include <asio/ssl.hpp>

using std::pair;
struct addressOrImgCode {
    enum infoTypes {
        address = 0,
        jpg = 1,
        jpeg = 2,
        png = 3,
        gif = 4
    };

    vector<pair<infoTypes, string>> infos;
};


class Visitor {
public:
    Visitor(io_context &ic_, string site):
        ic(ic_), resolver(ic), sock(ic),
        sslContext(asio::ssl::context::sslv23_client),
        sslStream(ic, sslContext) {
        
        auto data = splitSite(site);

        service = data[0];
        host = data[1];
        path = data[2];
    }

    void start();

    void registerResponseParserCallBack(function<void(string)>);

private:
    void resolve();

    void handleResolve(const asio::error_code &, tcp::resolver::results_type);

    void connect();

    void handleSSLConnectPart1(const asio::error_code &, const tcp::endpoint &);
    
    void handleConnect(const asio::error_code &, const tcp::endpoint &);

    void sendData();

    void handleSendCompleted(const asio::error_code& , std::size_t);

    void recvData();

    void handleRecvCompleted(const asio::error_code&, std::size_t);

    // io context, error_code
    io_context & ic;
    asio::error_code ec;
    // site, endpoint
    string host, path, service;
    asio::ip::basic_resolver_results<tcp> endpoints;
    // response
    asio::streambuf response;
    function<void(string)> _callback;
    // resolver
    tcp::resolver resolver;
    // tcp socket
    tcp::socket sock;
    // ssl
    asio::ssl::context sslContext;
    asio::ssl::stream<tcp::socket> sslStream;
};

#endif // WebsiteVisitor.h
