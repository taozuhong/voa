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

#include <iostream>
#include <curl.h>
#include "voa_curl.h"
#include "voa_html.h"
#include "args.hxx"
#include "voa_common.h"

int DeepDownload(const string & url, const string & extension, int depthLevel = 1, const string filter = "");

int main(int argc, const char** argv)
{

    args::ArgumentParser parser("VOA English audio & video downloader", "Welcome to contribute and share with friends.");
    args::HelpFlag help(parser, "Help", "Display this help menu", {'h', "help"});
    //args::ValueFlag<int> integer(parser, "integer", "The integer flag", {'i'});
    args::ValueFlag<std::string> argExt(parser, "Extension", "The file extension", {'e', "ext"}, "mp3");
    args::ValueFlag<std::string> argFilter(parser, "Filter", "The file filter", {'f', "filter"}, "");
    args::Positional<std::string> argUrl(parser, "Url", "the target url location");
    //args::PositionalList<double> numbers(parser, "numbers", "The numbers position list");
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser << std::endl;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser << std::endl;
        return -1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser << std::endl;
        return -2;
    }
    
    //if (integer) { std::cout << "i: " << args::get(integer) << std::endl; }
    //if (characters) { for (const auto ch: args::get(characters)) { std::cout << "c: " << ch << std::endl; } }
    if (argUrl) { std::cout << "Url: " << args::get(argUrl) << std::endl; }
    if (argExt) { std::cout << "Extension: " << args::get(argExt) << std::endl; }
    if (argFilter) { std::cout << "Filter: " << args::get(argFilter) << std::endl; }
    //if (numbers) { for (const auto nm: args::get(numbers)) { std::cout << "n: " << nm << std::endl; } } 


    string strUrl = args::get(argUrl); // "https://learningenglish.voanews.com/p/5631.html";
    string strExt = args::get(argExt); // "mp4";
    string strFilter = args::get(argFilter); // "_fullhd";
	if (strUrl.empty())
	{
		std::cout << parser << std::endl;
		return -3;
	}

    return DeepDownload(strUrl, strExt, 2, strFilter);
}


int DeepDownload(const string & url, const string & extension, int depthLevel, const string filter)
{
    CVoaCurl voaCurl;
    voaCurl.Initialize(url);

    string strHtmlSource;
    if(CURLE_OK != voaCurl.DownloadText(strHtmlSource))
    {
        return 0;
    }

    CVoaHtml voaHtml;
    if (! voaHtml.Initialize(url, strHtmlSource))
    {
        return 0;
    }


    // find target resource in page
    vector<string> resLinkVector;
    voaHtml.GetLinks(resLinkVector, extension, filter);

    // start the multi level digg
    vector<string> webLinkVector;
    vector<string> webLinksAgain;
    //voaHtml.GetLinks(webLinkVector, "htm");
    voaHtml.GetLinks(webLinkVector, "html");
    //voaHtml.GetLinks(webLinkVector, "shtml");
    for (int level = 1; level < depthLevel; ++level)
    {
        for (auto it : webLinkVector)
        {
            voaCurl.Initialize(it);
            if(CURLE_OK != voaCurl.DownloadText(strHtmlSource))
            {
                continue;
            }

            // collect all page to deep into next level
            if (voaHtml.Initialize(url, strHtmlSource))
            {
                // find target resource in page
                voaHtml.GetLinks(resLinkVector, extension, filter);

                // find new page in page
                //voaHtml.GetLinks(webLinksAgain, "htm");
                voaHtml.GetLinks(webLinksAgain, "html");
                //voaHtml.GetLinks(webLinksAgain, "shtml");
            }
        }

        webLinkVector.clear();
        webLinkVector.swap(webLinksAgain);
        webLinksAgain.clear();
    }


    // download the resource files in batch
    uint32_t fileCount = 0;
    for(auto it : resLinkVector)
    {
        voaCurl.Initialize(it);
        if (CURLE_OK == voaCurl.DownloadFile(GetFullPathFromUrl(it)))
        {
            ++fileCount;
        }
    }

    return fileCount;
}
