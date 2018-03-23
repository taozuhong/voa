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

#include <iostream>
#include <curl.h>
#include "voa_curl.h"
#include "voa_html.h"
#include "args.hxx"
#include "voa_common.h"

int DeepDownload(const string & url, const string & ext, int depthLevel = 1);

int main(int argc, char** argv)
{

    args::ArgumentParser parser("VOA English audio & video downloader", "This goes after the options.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    //args::ValueFlag<int> integer(parser, "integer", "The integer flag", {'i'});
    args::ValueFlag<std::string> argType(parser, "type", "The file extension", {'t', "type"});
    args::Positional<std::string> argUrl(parser, "url", "the target url location");
    //args::PositionalList<double> numbers(parser, "numbers", "The numbers position list");
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return -1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return -2;
    }
    
    //if (integer) { std::cout << "i: " << args::get(integer) << std::endl; }
    //if (characters) { for (const auto ch: args::get(characters)) { std::cout << "c: " << ch << std::endl; } }
    if (argUrl) { std::cout << "Url: " << args::get(argUrl) << std::endl; }
    if (argType) { std::cout << "Type: " << args::get(argType) << std::endl; }
    //if (numbers) { for (const auto nm: args::get(numbers)) { std::cout << "n: " << nm << std::endl; } } 

    /*
    if(argc < 2) {
        std::cout << parser;
        return 0;
    }
    */

    

    string strUrl = args::get(argUrl); // "https://learningenglish.voanews.com/a/4283319.html";
    string strExt = args::get(argType); // "mp3";
    return DeepDownload(strUrl, strExt, 2);
}


int DeepDownload(const string & url, const string & ext, int depthLevel)
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
    voaHtml.GetLinks(resLinkVector, ext);

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
                voaHtml.GetLinks(resLinkVector, ext);

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


    // download the mp3 audio file in batch
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
