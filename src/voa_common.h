/***********************************************************************/
/*
** 2018 March 23
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
**    Author: taozuhong@google.com (Andy Tao)
**
*************************************************************************/

#include <string>
#include <time.h>

using std::string;

string GetFullPath(const string & localFile);
string GetFullPathFromUrl(const string & url);
string GetFullUrl(const string & relativeUrl, const string & fullUrl);

string GetCurrentTime(const string & fmt = "%Y%m%d%H%M%S");

bool IsUrlMatchPattern(const string & url, const string & extension, const string & filter);
