#include <string>
#include <time.h>

using std::string;

string GetFullPath(const string & localFile);
string GetFullPathFromUrl(const string & url);
string GetFullUrl(const string & relativeUrl, const string & fullUrl);

string GetCurrentTime(const string & fmt = "%Y%m%d%H%M%S");

bool IsUrlMatchPattern(const string & url, const string & filter);
