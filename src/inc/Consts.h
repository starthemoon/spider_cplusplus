#ifndef _CONSTS_H_
#define _CONSTS_H_

#define HTTPPORT "80"
#define HTTPSPORT "443"

#include <string>

using std::string;

const string CONTENTTYPEHEADER = "content-type: ";
const string CONTENTTYPEHEADERUPPERCASE = "Content-Type: ";
const string LOCATIONINHEADER = "Location: ";
const string ServiceHostSeperator = "://";
const string ServiceHostSeperatorWithEscape = ":\\/\\/";

const string HTTPSERVICE = "http";
const string HTTPSSERVICE = "https";
const uint HTTPOK = 200;
const uint HTTPREDIRECTPERMANENTLY = 301;
const uint HTTPREDIRECTTEMPARATELY = 302;

const string VHTTPPORT = "80";
const string VHTTPSPORT = "443";
const string tempFileName = "temp.html";

const string JPGFORMAT = "jpg";
const string JPEGFORMAT = "jpeg";
const string PNGFORMAT = "png";
const string GIFFORMAT = "gif";

#endif // Consts.h