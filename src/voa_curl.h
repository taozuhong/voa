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
*************************************************************************/

#include <curl.h>
#include <string>
#include <fstream>
#include <curl.h>

using std::string;
using std::fstream;

class CVoaCurl 
{
    private:
    string m_Url;
    bool m_saveToFile = false;
    string * m_pTextSource = nullptr;
    CURL* m_CurlObj = nullptr;
    fstream m_fileStream;
    
    size_t ReadDataFromNet(void *contents, size_t size, size_t nmemb);
    static size_t Write_Callback(void *contents, size_t size, size_t nmemb, void *userObj);
    static int Progress_Callback(CVoaCurl *curlObj, double dltotal,  double dlnow, double ultotal, double ulnow);
    
    public:
    CVoaCurl();
    ~CVoaCurl();
    
    void Initialize(const string & url);
    CURLcode DownloadFile(const string & localFile = "");
    CURLcode DownloadText(string & textSource);
};

