#ifndef RESPONSEPARSER_H
#define RESPONSEPARSER_H

#include "Consts.h"
#include "Utils.h"

#include <set>
using std::set;

#include <sstream>

class ResponseParser {
public:
    ResponseParser() {
        vector<string> formats{JPGFORMAT, JPEGFORMAT, PNGFORMAT, GIFFORMAT};
        for (auto const format : formats) _imgFormats.insert(format);
    }

    void parseResponse(string, vector<string>&, vector<string>&);

private:
    set<string> _imgFormats;
    mutex _m_address;
    mutex _m_img;
};

void testResponseParser();

#endif // ResponseParser.h