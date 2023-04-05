#include "inc/ImgNameGenerator.h"

ImgNameGenerator* ImgNameGenerator::_instance = nullptr;
mutex ImgNameGenerator::_m;

string ImgNameGenerator::getTimeStr() {
    auto t = time(nullptr);
    auto e = ctime(&t);
    string name = ctime(&t);
    name = trim_address(name);
    name = add_img_path_prefix(name);
    return name;
}

void testImgNameGenerator() {
    auto str = trimString(ImgNameGenerator::getInstance()->getTimeStr());

    cout << "*" << setw(48) << setfill('*') << left
         << "*" << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " TEST IMG NAME GENERATOR" << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " + str << "*\n";
    cout << "*" << setw(48) << setfill(' ') << left
         << " " << "*\n";
    cout << "*" << setw(48) << setfill(' ') << right
         << __FILE__ << "*\n";
    cout << "*" << setw(48) << setfill('*') << left
         << "*" << "*\n\n";
}