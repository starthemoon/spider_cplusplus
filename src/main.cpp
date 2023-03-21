#include "inc/Consts.h"
#include "inc/Utils.h"
#include "inc/ImgNameGenerator.h"
#include "inc/ImgWriter.h"
#include "inc/ResponseParser.h"
#include "inc/WebsiteVisitor.h"

void test() {
    // test ImgNameGenerator
    testImgNameGenerator();

    // test ImgWriter
    testImgWriter();

    // test ResponseParser
    testResponseParser();
}

int main(int argc, char **argv) {
    // test
    test();

    try {
        if (argc != 2) {
            cerr << __FILE__ << ":" << __LINE__ << ":\tusage: <program> <site>\n";
            throw exception();
        }
        string site(argv[1]);

        if (!isSiteValid(site)) {
            throw exception();
        }

        io_context ic;

        auto generateImgName = std::bind(&ImgNameGenerator::getTimeStr,
                                ImgNameGenerator::getInstance());
        
        vector<string> addresses{site}, newAddrs;

        vector<string> imgCodes;
        int iwi = 0;
        vector<std::future<void>> iwrs;

        auto responseParser = ResponseParser();
        function<void(string)> rf = std::bind(&ResponseParser::parseResponse,
            &responseParser, std::placeholders::_1, newAddrs, imgCodes);

        while (addresses.size()) {
            auto visitors = vector<shared_ptr<Visitor>>(addresses.size());
            for (int i = 0; i < addresses.size(); ++i) {
                visitors[i] = shared_ptr<Visitor>(new Visitor(ic, addresses[i]));
                visitors[i]->registerResponseParserCallBack(rf);
                visitors[i]->start();
            }
            ic.run();

            addresses = newAddrs;
            newAddrs.clear();

            for (; iwi < imgCodes.size(); ++iwi) {
                auto iw = ImgWriter(imgCodes[iwi], generateImgName());
                auto iwr = std::async(&ImgWriter::start, &iw);
                iwrs.emplace_back(std::move(iwr));
            }
        }

        for (auto & iwr : iwrs) {
            iwr.wait();
        }
    } catch (exception &e) {
        cerr << __FILE__ << ":" << __LINE__ << ":\t" << e.what() << "\n";
    }

    return 0;
}