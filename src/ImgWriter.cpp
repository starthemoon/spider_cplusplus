#include "inc/ImgWriter.h"

void ImgWriter::start() {
    try {
        std::ofstream ofs(_destFileName, ios::out | ios::binary);
        if (!ofs.is_open()) {
            throw std::exception();
        }
        
        cout << "start writing img codes to " << _destFileName << "\n";
        ofs.write(_imgCode.c_str(), _imgCode.size());
        cout << "finish writing img codes to " << _destFileName << "\n";
    } catch (std::exception & e) {
        cerr << e.what() << "\n";
    }
}

namespace test_img_writer {
    const char * readImgFileName = "imgs/wolf1.png";
    const char * writeImgFileName = "imgs/temp.png";
}

void testImgWriter() {
    using test_img_writer::readImgFileName;
    using test_img_writer::writeImgFileName;
    
    std::ifstream ifs(readImgFileName, ios::in | ios::binary);
    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char imgCode[length];
    ifs.read(imgCode, length);
    string destFileName = writeImgFileName;
    auto iw = ImgWriter(string(imgCode, imgCode + length), destFileName);
    auto a1 = std::async(&ImgWriter::start, &iw);

    cout << "*" << setw(48) << setfill('*') << left
         << "*" << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " TEST IMG WRITER" << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " read img from " + string(readImgFileName) << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " write img to " + string(writeImgFileName) << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " << "*\n";
    cout << "*" << setw(48) << setfill(' ') << right
         << __FILE__ << "*\n";
    cout << "*" << setw(48) << setfill('*') << left
         << "*" << "*\n\n";
}

void testImgReadAndWriteWithCStyle() {
    using test_img_writer::readImgFileName;
    using test_img_writer::writeImgFileName;
    
    FILE* fp;

    if ((fp = fopen(readImgFileName, "rb")) == nullptr) {
        cerr << __FILE__ << ":" << __LINE__ << ":\tfailed to read file " << readImgFileName;
        throw std::exception();
    }
    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    rewind(fp);
    char * imgBuffer = (char*)calloc(length, sizeof(char));
    fread(imgBuffer, length, sizeof(char), fp);
    fclose(fp);

    if ((fp = fopen(writeImgFileName, "wb")) == nullptr) {
        cerr << __FILE__ << ":" << __LINE__ << ":\tfailed to write file " << writeImgFileName;
        throw std::exception();
    }
    fwrite(imgBuffer, length, sizeof(char), fp);
    fclose(fp);

    free(imgBuffer);
}

void testImgReadAndWriteWithCPPStyle() {
    using test_img_writer::readImgFileName;
    using test_img_writer::writeImgFileName;
    
    std::ifstream ifs(readImgFileName, ios::in | ios::binary);
    if (!ifs.is_open()) {
        cerr << __FILE__ << ":" << __LINE__ << ":\tfailed to read file " << readImgFileName;
        throw std::exception();
    }
    ifs.seekg(0, ifs.end);
    int length = ifs.tellg();
    ifs.seekg(0, ios::beg);
    shared_ptr<char> imgBuffer(new char[length], [](char * p){delete [] p;});
    ifs.read(imgBuffer.get(), length);
    ifs.close();
    
    std::ofstream ofs(writeImgFileName, ios::out | ios::binary);
    if (!ofs.is_open()) {
        cerr << __FILE__ << ":" << __LINE__ << ":\tfailed to write file " << writeImgFileName;
        throw std::exception();
    }
    ofs.write(imgBuffer.get(), length);
    ofs.close();
}