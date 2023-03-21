#ifndef IMGWRITER_H
#define IMGWRITER_H

#include "Utils.h"
#include <ios>
#include <asio.hpp>

using std::ios;

class ImgWriter {
public:
    ImgWriter(const string imgCode, const string destFileName):
        _imgCode(imgCode), _destFileName(destFileName) { }

    void start();

private:
    string _imgCode, _destFileName;
};

void testImgWriter();
void testImgReadAndWriteWithCStyle();
void testImgReadAndWriteWithCPPStyle();

#endif