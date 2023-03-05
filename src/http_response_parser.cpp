#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <set>

using std::istream;
using std::ostream;
using asio::streambuf;
using std::string;
using std::set;

const string jpgFormat = ".jpg";
const string jpegFormat = ".jpeg";
const string pngFormat = ".png";
const string gifFormat = ".gif";

const string TEMPFILENAME = "temp.html";

const int MAXSIZE = INT16_MAX;

class IMGParser {
private:

public:
    IMGParser(string fileName=TEMPFILENAME) {}

private:
    set<string> parseImgName(string fileName) {
        std::ifstream fs(fileName);
        char *t;
        fs.read(t, MAXSIZE);
        string data(t);
        string temp(data);
        for (auto format : {jpgFormat, jpegFormat, pngFormat, gifFormat}) {
            int i, j = 0;
            while (j != string::npos) {
                j = temp.find(jpgFormat);
                
            }
        }
    }
};