#include "inc/Utils.h"
#include "inc/Consts.h"

string getSuffix(string site) {
    for (int i = site.size() - 1; i >= 0; --i) {
        if (site[i] == '.') {
            auto end = site.find('?', i);
            if (end == string::npos) return site.substr(i);
            return site.substr(i, end - i);
        }
    }
    return "";
}

// site must be <service>://<host>/<path>
vector<string> splitSite(string site) {
    auto serviceEnd = site.find(ServiceHostSeperator);
    if (serviceEnd == string::npos) {
        cerr << __FILE__ << ":" << __LINE__
             << ":\tservice invalid with site \"" << site << "\"\n";
        return vector<string>{};
    }
    auto service = site.substr(0, serviceEnd);

    auto hostStart = serviceEnd + ServiceHostSeperator.size();
    auto hostEnd = site.find('/', hostStart);
    auto host = site.substr(hostStart, hostEnd - hostStart);
    if (hostEnd == string::npos) {
        host = site.substr(hostStart);
    }

    auto path = hostEnd == string::npos ? "/" : site.substr(hostEnd + 1);

    return vector<string>{service, host, path};
}

bool isSiteValid(string site) {
    return splitSite(site).size() == 3;
}

string trimString(string s) {
    int l = 0, r = s.size() - 1;
    while (s[l] == ' ' || s[l] == '\t' || s[l] == '\r' || s[l] == '\n') ++l;
    while (s[r] == ' ' || s[r] == '\t' || s[r] == '\r' || s[r] == '\n') --r;
    s.erase(r + 1, s.size());
    s.erase(0, l);
    return s;
}