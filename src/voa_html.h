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
    static int GetLinks(const string & url, const GumboNode* node, vector<string> & linkVector, const string & filter = "mp3");
        
    public:
    CVoaHtml();
    ~CVoaHtml();
    
    bool Initialize(const string & url, const string & htmlSource);
    int GetLinks(vector<string> & linkVector, const string & filter = "mp3");
};

