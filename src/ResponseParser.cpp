#include "inc/ResponseParser.h"

void errWithLocationAndStatusCode(int statusCode,
    const string statusMessage, const string location) {
    cerr << __FILE__ << ":" << __LINE__ << ":\tinvalid response with status code [" << statusCode
         << "], message [" + statusMessage + "] and Location [" << location << "]\n";
}

bool ResponseParser::parseResponse(string response,
    vector<string>& addresses, vector<string>& imgCodes) {
    std::stringstream ss(response);

    // get statusCode
    uint statusCode = 0;
    string httpVersion, statusMessage;
    ss >> httpVersion >> statusCode;
    std::getline(ss, statusMessage, '\r');

    // get location in header
    string location;
    auto loc = response.find(LOCATIONINHEADER);
    if (loc != string::npos) {
        loc += LOCATIONINHEADER.size();
        location = response.substr(loc, response.find('\r', loc) - loc);
    }

    // branches from status code first and then location
    if ((statusCode == HTTPREDIRECTTEMPARATELY ||
        statusCode == HTTPREDIRECTPERMANENTLY) && location.size()) {
        auto lg = lock_guard(_m_address);
        addresses.emplace_back(location);
        return true;
    } else if (statusCode == HTTPOK) {
        auto content_type = get_content_type(response);
        auto responseBody = response.substr(response.find("\r\n\r\n") + 4);
        if (_imgFormats.count(content_type)) {
            /*
             * TODO: some may not return the img code, find out why
             */
            if (responseBody.size()) {
                auto lg = lock_guard(_m_img);
                imgCodes.emplace_back("." + content_type);
                imgCodes.emplace_back(responseBody);
            }
            return true;
        } else {
            parse_html(responseBody, addresses);
            return true;
        }
    }
    errWithLocationAndStatusCode(statusCode, statusMessage, location);
    return false;
}

void ResponseParser::parse_html(string responseBody, vector<string>& addresses) {
    unordered_set<string> addr;
    while (responseBody.size()) {
        int pos1 = responseBody.find("\"");
        if (pos1 == string::npos) break;
        int pos2 = responseBody.find("\"", pos1 + 1);
        if (pos2 == string::npos) break;

        auto location = responseBody.substr(pos1 + 1, pos2 - pos1 - 1);
        responseBody = responseBody.substr(pos2 + 1);

        if (location.size() < 4 ||
        (!_imgFormats.count(location.substr(location.size() - 3)) &&
        !_imgFormats.count(location.substr(location.size() - 4)))) {
            continue;
        }
        if (location.substr(0, 4) != HTTPSERVICE) {
            if (location.substr(0, 1) != ":") {
                if (location.substr(0, 2) != "//") {
                    location = HTTPSERVICE + ":" + "//" + location;
                } else {
                    location = HTTPSERVICE + ":" + location;
                }
            } else {
                location = HTTPSERVICE + location;
            }
        }

        location = trim_address(location);

        if (!addr.count(location)) {
            auto lg = lock_guard(_m_address);
            addresses.emplace_back(location);
            addr.insert(location);
        }
    }
}

namespace test_response_parser {
    const char * readImgFileName = "imgs/wolf1.png";
    const char * writeImgFileName = "imgs/temp.png";
}

void testResponseParser() {
    using test_response_parser::readImgFileName;
    using test_response_parser::writeImgFileName;
    
    ResponseParser rp;

    vector<string> addrs, imgCodes;

    std::ifstream ifs(readImgFileName, std::ios::in | std::ios::binary);
    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char imgCode[length];
    ifs.read(imgCode, length);
    std::stringstream ssImg;
    ssImg << "HTTP/1.1 200 OK\r\n"
            << "Content-Type: image/png\r\n"
            << "Access-Control-Allow-Origin: *\r\n"
            // << "Content-Location: https://img.s-msn.com/tenant/amp/entityid/AAYdygI?w=60&h=60&q=60&m=6&f=png&u=t\r\n"
            << "Content-Location: https://img.s-msn.com/tenant/amp/entityid/AAYdygI.png?w=60&h=60&q=60&m=6&f=png&u=t\r\n"
            << "Last-Modified: Thu, 16 Mar 2023 00:56:20 GMT\r\n"
            << "X-Source-Length: 300156\r\n"
            << "X-Datacenter: westus\r\n"
            << "X-ActivityId: 4f55dbdc-041a-4c5a-a293-59ac7b5ecbf6\r\n"
            << "Timing-Allow-Origin: *\r\n"
            << "X-Frame-Options: DENY\r\n"
            << "X-ResizerVersion: 1.0\r\n"
            << "Content-Length: 13384\r\n"
            << "Cache-Control: public, max-age=117080\r\n"
            << "Expires: Tue, 21 Mar 2023 00:55:27 GMT\r\n"
            << "Date: Sun, 19 Mar 2023 16:24:07 GMT\r\n"
            << "Connection: close\r\n"
            << "\r\n";
    ssImg.write(imgCode, length);

    std::stringstream ssAddr;
    ssAddr << "HTTP/1.1 301 Moved Permanently\r\n"
            << "Location: https://image.baidu.com/\r\n"
            << "Date: Sun, 19 Mar 2023 17:26:46 GMT\r\n"
            << "Content-Length: 59\r\n"
            << "Content-Type: text/html; charset=utf-8\r\n";

    rp.parseResponse(ssImg.str(), addrs, imgCodes);
    rp.parseResponse(ssAddr.str(), addrs, imgCodes);

    assert(addrs.size() == 1);
    assert(imgCodes.size() == 1);

    std::ofstream ofs(writeImgFileName, std::ios::out | std::ios::binary);
    ofs.write(imgCodes[0].c_str(), imgCodes[0].size());

    cout << "*" << setw(48) << setfill('*') << left
         << "*" << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " TEST RESPONSE PARSER" << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " 1. get 301 address: " + addrs[0] << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " 2. write img code to " + string(writeImgFileName) << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " << "*\n";
    cout << "*" << setw(48) << setfill(' ') << right
         << __FILE__ << "*\n";
    cout << "*" << setw(48) << setfill('*') << left
         << "*" << "*\n\n";
}