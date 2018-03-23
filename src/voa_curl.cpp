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

#include "voa_curl.h"
#include "voa_common.h"
#include <iostream>
#include <algorithm>
    
CVoaCurl::CVoaCurl()
{
    curl_global_init(CURL_GLOBAL_ALL);
    m_CurlObj = curl_easy_init();
}

CVoaCurl::~CVoaCurl()
{
    if (nullptr != m_CurlObj)
    {
        curl_easy_cleanup(m_CurlObj);
    }

    curl_global_cleanup();
}

void CVoaCurl::Initialize(const string & url)
{        
    m_Url = url;
    string strTemp = m_Url;
    std::transform(strTemp.begin(), strTemp.end(),strTemp.begin(), ::toupper);
    
    curl_easy_setopt(m_CurlObj, CURLOPT_URL, m_Url.c_str());   
    if (0 == strTemp.find("HTTP")) {
        curl_easy_setopt(m_CurlObj, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(m_CurlObj, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        if (0 == strTemp.find("HTTPS")) {
            curl_easy_setopt(m_CurlObj, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(m_CurlObj, CURLOPT_SSL_VERIFYHOST, 0L);
        }
    }

    
    /* register callback function and pass the key argument */ 
    curl_easy_setopt(m_CurlObj, CURLOPT_WRITEDATA, (void *)this);    
    curl_easy_setopt(m_CurlObj, CURLOPT_WRITEFUNCTION, &CVoaCurl::Write_Callback);

    // register callback function and pass the key argument
    curl_easy_setopt(m_CurlObj, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(m_CurlObj, CURLOPT_PROGRESSDATA, (void *)this);
    curl_easy_setopt(m_CurlObj, CURLOPT_PROGRESSFUNCTION, &CVoaCurl::Progress_Callback);
}


CURLcode CVoaCurl::DownloadFile(const string & localFile)
{
    m_saveToFile = true;
    m_fileStream.open(localFile, fstream::out);
    if (! m_fileStream.is_open())
    {
        std::cerr << "File create failure: " << localFile << std::endl;
        return CURLE_FAILED_INIT;    
    }
    
    std::cout << "Downloading: " << localFile << std::endl;
    curl_easy_setopt(m_CurlObj, CURLOPT_NOPROGRESS, 0L);
    CURLcode ret_code = curl_easy_perform(m_CurlObj);
    if (CURLE_OK == ret_code)
    {
        std::cout << "[done]" << std::endl;
    }
    else
    {
        std::cerr << std::endl << "Download error: " << curl_easy_strerror(ret_code) << std::endl;
    }

    //close file
    m_fileStream.flush();
    m_fileStream.close();
    
    return ret_code;
}

CURLcode CVoaCurl::DownloadText(string & textSource)
{
    m_saveToFile = false;
    m_pTextSource = &textSource;

    // close progress when ddownload file to source
    std::cout << "Downloading: " << m_Url << ".....";
    curl_easy_setopt(m_CurlObj, CURLOPT_NOPROGRESS, 1L);
    CURLcode ret_code = curl_easy_perform(m_CurlObj);
    if (CURLE_OK != ret_code) {
        std::cerr << "Perform error: " << curl_easy_strerror(ret_code) << std::endl;
    }
    
    std::cout << "[done]" << std::endl;
    
    return ret_code;
}

size_t CVoaCurl::Write_Callback(void *contents, size_t size, size_t nmemb, void *userObj)
{
    return static_cast<CVoaCurl *>(userObj)->ReadDataFromNet(contents, size, nmemb);
}

size_t CVoaCurl::ReadDataFromNet(void *contents, size_t size, size_t nmemb)
{     
    size_t writeSize = 0;
    size_t dataSize = size * nmemb;
    //std::cout << "Enter callback..." << size << "->" << nmemb << "->" << dataSize << std::endl;    
    if (m_saveToFile)
    {
        if (m_fileStream.is_open())
        {
            m_fileStream.write((const char*)contents, dataSize);
            writeSize = dataSize;
            // std::cout << ".";
        }
    }
    else
    {
        m_pTextSource->append((const char*)contents, dataSize);
        writeSize = dataSize;
        // std::cout << ".";
    }    

    
    return writeSize;
}

int CVoaCurl::Progress_Callback(CVoaCurl *curlObj, double dltotal,  double dlnow, double ultotal, double ulnow)
{
    if (dltotal > 0)
    {
        double dbPercent = dlnow * 100.0 / dltotal;
        uint32_t  ulPercent = static_cast<uint32_t>(round(dbPercent));
        // printf("%d / %d (%g %%)\n", dlnow, dltotal, dlnow * 100.0 / dltotal);

        // show the progress infomation
        string strProgress;
        strProgress.append(ulPercent, '=');
        strProgress.append("[");
        strProgress.append(std::to_string(ulPercent));
        strProgress.append("%]");


        std::cout << "\r" << strProgress ;
    }

    return 0;
}
