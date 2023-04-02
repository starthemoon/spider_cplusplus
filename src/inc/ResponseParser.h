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
        init_img_formats();
    }

    bool parseResponse(string, vector<string>&, vector<string>&);

private:
    void init_img_formats() {
        _imgFormats = set<string>{
            JPGFORMAT, to_upper(JPGFORMAT), JPEGFORMAT, to_upper(JPEGFORMAT),
            PNGFORMAT, to_upper(PNGFORMAT), GIFFORMAT, to_upper(GIFFORMAT)
        };    
    }

    string to_upper(const string str) {
        string res;
        for (auto const ch : str) {
            res += toupper(ch);
        }
        return res;
    }

    void parse_html(string, vector<string>&);

    set<string> _imgFormats;
    mutex _m_address;
    mutex _m_img;
};

void testResponseParser();

#endif // ResponseParser.h