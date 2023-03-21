#ifndef IMGNAMEGENERATOR_H
#define IMGNAMEGENERATOR_H

#include <ctime>
#include "Utils.h"

class ImgNameGenerator {
private:
    ImgNameGenerator() { }

protected:
    static ImgNameGenerator * _instance;
    static mutex _m;
    
public:
    string getTimeStr();
    static shared_ptr<ImgNameGenerator> getInstance() {
        auto ul = lock_guard(_m);
        if (_instance == nullptr) {
            _instance = new ImgNameGenerator();
        }
        return shared_ptr<ImgNameGenerator>(_instance);
    }
};

void testImgNameGenerator();

#endif