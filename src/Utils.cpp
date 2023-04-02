#include "inc/Utils.h"
#include "inc/Consts.h"

/* deprecated */
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

string get_content_type(string response) {
    auto start = response.find(CONTENTTYPEHEADER);
    if (start == string::npos) {
        start = response.find(CONTENTTYPEHEADERUPPERCASE);
    }
    if (start == string::npos) return "";
    start += CONTENTTYPEHEADER.size();

    auto end = response.find("\r", start);
    if (end == string::npos) return "";

    auto content_type = response.substr(start, end - start);
    start = content_type.find("/");
    if (start == string::npos) return content_type;
    return content_type.substr(start + 1);
}

// site must be <service>://<host>/<path>
vector<string> splitSite(string site) {
    auto serviceEnd = site.find(ServiceHostSeperator);
    if (serviceEnd == string::npos) {
        serviceEnd = site.find(ServiceHostSeperatorWithEscape);
    }
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
    if (path.empty()) path = "/";

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