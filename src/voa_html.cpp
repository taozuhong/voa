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

#include "voa_html.h"
#include <fstream>
#include <iostream>
#include <assert.h>
#include <gumbo.h>
#include "voa_common.h"

CVoaHtml::CVoaHtml()
{
    //m_HtmlFile.empty();
    m_pHtmlOutput = NULL;
}

CVoaHtml::~CVoaHtml()
{
    if (NULL != m_pHtmlOutput)
    {
        gumbo_destroy_output(&kGumboDefaultOptions, m_pHtmlOutput);
        m_pHtmlOutput = NULL;
    }
    
}

int CVoaHtml::GetLinks(vector<string> & linkVector, const string & filter)
{
    if ((NULL == m_pHtmlOutput) || (m_pHtmlOutput->root->type != GUMBO_NODE_ELEMENT)) 
    {
        return 0;
    }

    return GetLinks(m_Url, m_pHtmlOutput->root, linkVector, filter);
}

bool CVoaHtml::Initialize(const string & url, const string & htmlSource)
{
    m_Url = url;
    m_HtmlSource = htmlSource;

    if (NULL != m_pHtmlOutput)
    {
        gumbo_destroy_output(&kGumboDefaultOptions, m_pHtmlOutput);
    }
    
    m_pHtmlOutput = gumbo_parse(m_HtmlSource.c_str());    
    return NULL != m_pHtmlOutput;
}


bool CVoaHtml::LoadHtml(const string & htmlFile)
{
    // load html source from file
    std::ifstream in(htmlFile, std::ios::in | std::ios::binary);
    if (!in) 
    {
        std::cout << "File " << htmlFile << " not found!\n";
        return false;
    }

    in.seekg(0, std::ios::end);
    m_HtmlSource.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&m_HtmlSource[0], m_HtmlSource.size());
    in.close();
    
    return !m_HtmlSource.empty();
}

int CVoaHtml::GetLinks(const string & url, const GumboNode* node, vector<string> & linkVector, const string & filter)
{
    size_t linkCount = 0;
    GumboAttribute* href = NULL;
    assert(NULL != node);
    if ((GUMBO_TAG_A == node->v.element.tag) && (0 < node->v.element.attributes.capacity))
    {
        href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (NULL != href)
        {
            string strTempUrl = href->value;
            if (IsUrlMatchPattern(strTempUrl,filter))
            {
                // append link if it not exist
                bool linkFound = false;
                strTempUrl = GetFullUrl(strTempUrl, url);
                for(auto it : linkVector)
                {
                    if (0 == it.compare(strTempUrl))
                    {
                        linkFound = true;
                        break;
                    }
                }

                if (! linkFound)
                {
                    linkVector.push_back(strTempUrl);
                    ++linkCount;
                }
            }
        }
    }

    GumboNode* childNode = NULL;
    const GumboVector* children = &node->v.element.children;
    if ((children->capacity > 0) && (children->length > 0))
    {
        for (unsigned int i = 0; i < children->length; ++i)
        {
            childNode = static_cast<GumboNode *>(children->data[i]);
            if ((NULL != childNode) && (GUMBO_NODE_ELEMENT == childNode->type))
            {
                linkCount += GetLinks(url, childNode, linkVector, filter);
            }
        }
    }
    
    return linkCount;
}

