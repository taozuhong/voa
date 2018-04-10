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
**    Author: taozuhong@gmail.com (Andy Tao)
**
*************************************************************************/

#include <string>
#include <vector>
#include "gumbo.h"

using std::string;
using std::vector;

class CVoaHtml 
{
    private:
    string m_Url;
    string m_HtmlSource;
    GumboOutput* m_pHtmlOutput = NULL;
    
    bool LoadHtml(const string & htmlFile);
    static int GetLinks(const string & url, const GumboNode* node, vector<string> & linkVector, const string & extension = "mp3", const string & filter = "");
        
    public:
    CVoaHtml();
    ~CVoaHtml();
    
    bool Initialize(const string & url, const string & htmlSource);
    int GetLinks(vector<string> & linkVector, const string & extension = "mp3", const string & filter="");
};

