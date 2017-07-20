/** 
   *@file         MtIni.hpp
   *@brief        MtIni.hpp Metus Ini 
                  可支持unicode utf8  非windows api 多次读写文件
                  注意 key 一定不能含有=字符
   *@version      1.3.0
*/


#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <Windows.h>
#include <io.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <Windows.h>
#include <io.h>
#include <stdlib.h>
#include "SimpleIni.hpp"
#include "MtIni.h"



class CSimpleIniAP : public CSimpleIniA
{

};

class CSimpleIniWP : public CSimpleIniW
{

};


/////////////////////////////////////////////////////////////////////////////////////
MtIniA::MtIniA() 
{
    m_bAutoSave = true;
    m_bUseUtf8 = false;
    m_nDefDoubleBitNum = 6;
    m_szFilePath[0] = 0;
    m_bOpen = false;
	m_pSimpleIni = new CSimpleIniAP;
    SetSection("Defalut");
}

MtIniA::MtIniA(const char* szFilePath) 
{
    m_bAutoSave = true;
    m_bUseUtf8 = false;
    m_nDefDoubleBitNum = 6;
    m_bOpen = false;
    m_szFilePath[0] = 0;
    m_pSimpleIni = new CSimpleIniAP;
    SetSection("Defalut");
    LoadFile(szFilePath);
}

MtIniA::MtIniA(const char* szFilePath,const char* szSection)
{
    m_bAutoSave = true;
    m_bUseUtf8 = false;
    m_nDefDoubleBitNum = 6;
    m_bOpen = false;
    m_szFilePath[0] = 0;
    m_pSimpleIni = new CSimpleIniAP;
    SetSection(szSection);
    LoadFile(szFilePath);
}

MtIniA::~MtIniA() 
{
    if (m_bAutoSave)
    {
        SaveFile();
    }
    delete m_pSimpleIni;
}       

/**
 * @brief LoadFile Ini File
 * @param szFilePath - ini File Path Name Load.
 * @return true is sucess , false is fail.
 */
bool MtIniA::LoadFile(const char* szFilePath,bool bAutoSave/*=true*/,bool bUseUtf8/*=false*/)
{
    if (m_bAutoSave && m_bOpen)
    {
        SaveFile();
    }
    ::GetFullPathNameA(szFilePath,MAX_PATH,m_szFilePath,NULL);
    
    m_bAutoSave = bAutoSave;
    m_bUseUtf8 = bUseUtf8;
    m_pSimpleIni->Reset();
    m_pSimpleIni->SetUnicode(m_bUseUtf8);

    if(_access(m_szFilePath,0) == EOF)
    {
        m_pSimpleIni->SaveFile(m_szFilePath,m_bUseUtf8);
        m_bOpen = true;
    }
    else
    {
        m_bOpen = (m_pSimpleIni->LoadFile(m_szFilePath)==SI_OK);
        if (!m_bOpen)
        {
            m_pSimpleIni->Reset();
            m_pSimpleIni->SetUnicode(m_bUseUtf8);
            m_pSimpleIni->SaveFile(m_szFilePath,m_bUseUtf8);
            m_bOpen = true;
        }
    }

    SetSection(m_szSection);
    return m_bOpen;
}

bool MtIniA::SaveFile()
{
    if (!m_bOpen)
    {
        return true;
    }
    if (m_szFilePath[0] != 0)
    {
        m_pSimpleIni->SaveFile(m_szFilePath,m_bUseUtf8);
    }        
    return true;
}

void MtIniA::SetSection(const char* szSection)
{
    strcpy_s(m_szSection,szSection);
    return ;
}

bool MtIniA::DelSection(const char* szSection)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->Delete(szSection,NULL);
}

bool MtIniA::DelKeyName(const char* szKeyName)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->Delete(m_szSection,szKeyName);
}

const char* MtIniA::GetSection()
{
    return m_szSection;
}

bool MtIniA::GetSectionS(std::vector<std::string>& vecSec)
{
    CSimpleIniA::TNamesDepend secs;
    m_pSimpleIni->GetAllSections(secs);

    // dump all of the key names
    CSimpleIniA::TNamesDepend::const_iterator iter = secs.begin();
    for ( ; iter != secs.end(); iter++ )
    {
        const CHAR *pszItem = iter->pItem;
        if (pszItem)
        {
            vecSec.push_back(pszItem);
        }
    }
    return true;
}

bool MtIniA::GetKeyS(const char* szSection,std::vector<std::string>& vecSec)
{
    CSimpleIniA::TNamesDepend keys;
    m_pSimpleIni->GetAllKeys(szSection, keys);

    // dump all of the key names
    CSimpleIniA::TNamesDepend::const_iterator iKey = keys.begin();
    const CHAR *pItem = nullptr;
    for ( ; iKey != keys.end(); ++iKey )
    {
        pItem = iKey->pItem;
        if (pItem)
        {
            vecSec.push_back(pItem);
        }
    }
    return true;
}


bool MtIniA::SetInt(const char* szKey,int nVal)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->SetLongValue(m_szSection, szKey,nVal,nullptr,false,true)==SI_OK;
}

int MtIniA::GetInt(const char* szKey,int nDefVal/* = 0*/)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->GetLongValue(m_szSection,szKey, nDefVal);
}

bool MtIniA::SetString(const char* szKey,const char* szVal)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->SetValue(m_szSection,szKey,szVal,nullptr,true)==SI_OK;
}

const char* MtIniA::GetString(const char* szKey,const char* szDefVal/* = NULL*/)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->GetValue(m_szSection,szKey, szDefVal);
}

bool MtIniA::SetDouble(const char* szKey,double dVal)
{
    switch(m_nDefDoubleBitNum)
    {
    case 1:  {sprintf_s(m_szValTemp,("%.1f"),dVal);break;}
    case 2:  {sprintf_s(m_szValTemp,("%.2f"),dVal);break;}
    case 3:  {sprintf_s(m_szValTemp,("%.3f"),dVal);break;}
    case 4:  {sprintf_s(m_szValTemp,("%.4f"),dVal);break;}
    case 5:  {sprintf_s(m_szValTemp,("%.5f"),dVal);break;}
    case 6:  {sprintf_s(m_szValTemp,("%.6f"),dVal);break;}
    case 7:  {sprintf_s(m_szValTemp,("%.7f"),dVal);break;}
    case 8:  {sprintf_s(m_szValTemp,("%.8f"),dVal);break;}
    case 9:  {sprintf_s(m_szValTemp,("%.9f"),dVal);break;}
    default: {sprintf_s(m_szValTemp,("%.6f"),dVal);break;}
    }
    return  SetString(szKey,m_szValTemp);
}

double MtIniA::GetDouble(const char* szKey,double dDefVal/*=0*/)
{
    const char* m_pVal = GetString(szKey,0);
    if(m_pVal && (m_pVal[0] != 0))
    {
        double dVal = atof(m_pVal);
        return dVal;
    }
    else
    {
        return dDefVal;
    }
    return 0.0f;
}

void MtIniA::SerialInt(const char* szKey, int& nVal, bool bSerialorNot, int nDefVal/* = 0*/)
{
    if (bSerialorNot)
    {
        SetInt(szKey, nVal);
    }
    else
    {
        nVal = GetInt(szKey, nDefVal);
    }
}

void MtIniA::SerialDouble(const char* szKey, double& nVal, bool bSerialorNot, double dDefVal/* = 0*/)
{
    if (bSerialorNot)
    {
        SetDouble(szKey, dDefVal);
    }
    else
    {
        nVal = GetDouble(szKey, dDefVal);
    }
}

void MtIniA::SerialString(const char* szKey, std::string& strVal, bool bSerialorNot, const char* szDefVal/* = ""*/)
{
    if (bSerialorNot)
    {
        SetString(szKey, strVal.c_str());
    }
    else
    {
        strVal = GetString(szKey, szDefVal);
    }
}

void MtIniA::SerialVecInt(const char* szKey, std::vector<int>& nVecIntVal, bool bSerialorNot)
{
    if (bSerialorNot)
    {
        DelKeyName(szKey);
        char szBuf[MAX_BUFF_STRING];

        sprintf_s(szBuf, "%s_Count", szKey);
        SetInt(szBuf, nVecIntVal.size());

        for (size_t i = 0; i < nVecIntVal.size(); i++)
        {
            sprintf_s(szBuf, "%s_%03d", szKey,i);
            SetInt(szBuf, nVecIntVal[i]);
        }
    }
    else
    {
        nVecIntVal.clear();

        char szBuf[MAX_BUFF_STRING];

        sprintf_s(szBuf, "%s_Count", szKey);
        int nCount = GetInt(szBuf,0);

        for (size_t i = 0; i < nCount; i++)
        {
            sprintf_s(szBuf, "%s_%03d", szKey, i);
            int nVal = GetInt(szBuf, 0);

            nVecIntVal.push_back(nVal);
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////////


MtIniW::MtIniW() 
{
    m_bAutoSave = true;
    m_bUseUtf8 = false;
    m_nDefDoubleBitNum = 6;
    m_szFilePath[0] = 0;
    m_bOpen = false;
	m_pSimpleIni = new CSimpleIniWP;
    SetSection(L"Defalut");
}

MtIniW::MtIniW(const wchar_t* szFilePath) 
{
    m_bAutoSave = true;
    m_bUseUtf8 = false;
    m_nDefDoubleBitNum = 6;
    m_bOpen = false;
    m_szFilePath[0] = 0;
    m_pSimpleIni = new CSimpleIniWP;
    SetSection(L"Defalut");
    LoadFile(szFilePath);
}

MtIniW::MtIniW(const wchar_t* szFilePath,const wchar_t* szSection)
{
    m_bAutoSave = true;
    m_bUseUtf8 = false;
    m_nDefDoubleBitNum = 6;
    m_bOpen = false;
    m_szFilePath[0] = 0;
    m_pSimpleIni = new CSimpleIniWP;
    SetSection(szSection);
    LoadFile(szFilePath);
}

MtIniW::~MtIniW() 
{
    if (m_bAutoSave)
    {
        SaveFile();
    }
    delete m_pSimpleIni;
}       

/**
 * @brief LoadFile Ini File
 * @param szFilePath - ini File Path Name Load.
 * @return true is sucess , false is fail.
 */
bool MtIniW::LoadFile(const wchar_t* szFilePath,bool bAutoSave/*=true*/,bool bUseUtf8/*=false*/)
{
    if (m_bAutoSave && m_bOpen)
    {
        SaveFile();
    }
    ::GetFullPathNameW(szFilePath,MAX_PATH,m_szFilePath,NULL);

    m_bAutoSave = bAutoSave;
    m_bUseUtf8 = bUseUtf8;
    m_pSimpleIni->Reset();
    m_pSimpleIni->SetUnicode(m_bUseUtf8);

    if(_waccess(m_szFilePath,0) == EOF)
    {
        m_pSimpleIni->SaveFile(m_szFilePath,m_bUseUtf8);
        m_bOpen = true;
    }
    else
    {
        m_bOpen = (m_pSimpleIni->LoadFile(m_szFilePath)==SI_OK);
        if (!m_bOpen)
        {
            m_pSimpleIni->Reset();
            m_pSimpleIni->SetUnicode(m_bUseUtf8);
            m_pSimpleIni->SaveFile(m_szFilePath,m_bUseUtf8);
            m_bOpen = true;
        }
    }

    SetSection(m_szSection);
    return m_bOpen;
}

bool MtIniW::SaveFile()
{
    if (!m_bOpen)
    {
        return true;
    }
    if (m_szFilePath[0] != 0)
    {
        m_pSimpleIni->SaveFile(m_szFilePath,m_bUseUtf8);
    }
    return true;
}

void MtIniW::SetSection(const wchar_t* szSection)
{
    wcscpy_s(m_szSection,szSection);
    return ;
}

bool MtIniW::DelSection(const wchar_t* szSection)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->Delete(szSection,NULL);
}

bool MtIniW::DelKeyName(const wchar_t* szKeyName)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->Delete(m_szSection,szKeyName);
}

const wchar_t* MtIniW::GetSection()
{
    return m_szSection;
}

bool MtIniW::GetSectionS(std::vector<std::wstring>& vecSec)
{
    CSimpleIniW::TNamesDepend secs;
    m_pSimpleIni->GetAllSections(secs);

    // dump all of the key names
    CSimpleIniW::TNamesDepend::const_iterator iter = secs.begin();
    for ( ; iter != secs.end(); iter++ )
    {
        const WCHAR *pszItem = iter->pItem;
        if (pszItem)
        {
            vecSec.push_back(pszItem);
        }
    }
    return true;
}

bool MtIniW::GetKeyS(const wchar_t* szSection,std::vector<std::wstring>& vecSec)
{
    CSimpleIniW::TNamesDepend keys;
    m_pSimpleIni->GetAllKeys(szSection, keys);

    // dump all of the key names
    CSimpleIniW::TNamesDepend::const_iterator iKey = keys.begin();
    const WCHAR *pItem = nullptr;
    for ( ; iKey != keys.end(); ++iKey )
    {
        pItem = iKey->pItem;
        if (pItem)
        {
            vecSec.push_back(pItem);
        }
    }
    return true;
}

bool MtIniW::SetInt(const wchar_t* szKey,int nVal)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->SetLongValue(m_szSection, szKey,nVal,nullptr,false,true)==SI_OK;
}

int MtIniW::GetInt(const wchar_t* szKey,int nDefVal/* = 0*/)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->GetLongValue(m_szSection,szKey, nDefVal);
}

bool MtIniW::SetString(const wchar_t* szKey,const wchar_t* szVal)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->SetValue(m_szSection,szKey,szVal,nullptr,true)==SI_OK;
}

const wchar_t* MtIniW::GetString(const wchar_t* szKey,const wchar_t* szDefVal/* = NULL*/)
{
    if(!m_bOpen) return FALSE;
    return m_pSimpleIni->GetValue(m_szSection,szKey, szDefVal);
}

bool MtIniW::SetDouble(const wchar_t* szKey,double dVal)
{
    switch(m_nDefDoubleBitNum)
    {
    case 1:  {swprintf_s(m_szValTemp,(L"%.1f"),dVal);break;}
    case 2:  {swprintf_s(m_szValTemp,(L"%.2f"),dVal);break;}
    case 3:  {swprintf_s(m_szValTemp,(L"%.3f"),dVal);break;}
    case 4:  {swprintf_s(m_szValTemp,(L"%.4f"),dVal);break;}
    case 5:  {swprintf_s(m_szValTemp,(L"%.5f"),dVal);break;}
    case 6:  {swprintf_s(m_szValTemp,(L"%.6f"),dVal);break;}
    case 7:  {swprintf_s(m_szValTemp,(L"%.7f"),dVal);break;}
    case 8:  {swprintf_s(m_szValTemp,(L"%.8f"),dVal);break;}
    case 9:  {swprintf_s(m_szValTemp,(L"%.9f"),dVal);break;}
    default: {swprintf_s(m_szValTemp,(L"%.6f"),dVal);break;}
    }
    return  SetString(szKey,m_szValTemp);
}

double MtIniW::GetDouble(const wchar_t* szKey,double dDefVal/*=0*/)
{
    const wchar_t* m_pVal = GetString(szKey,0);
    if(m_pVal && (m_pVal[0] != 0))
    {
        double dVal = _wtof(m_pVal);
        return dVal;
    }
    else
    {
        return dDefVal;
    }
    return 0.0f;
}

void MtIniW::SerialInt(const wchar_t* szKey, int& nVal, bool bSerialorNot, int nDefVal/* = 0*/)
{
    if (bSerialorNot)
    {
        SetInt(szKey, nVal);
    }
    else
    {
        nVal = GetInt(szKey, nDefVal);
    }
}

void MtIniW::SerialDouble(const wchar_t* szKey, double& nVal, bool bSerialorNot, double dDefVal/* = 0*/)
{
    if (bSerialorNot)
    {
        SetDouble(szKey, dDefVal);
    }
    else
    {
        nVal = GetDouble(szKey, dDefVal);
    }
}

void MtIniW::SerialString(const wchar_t* szKey, std::wstring& strVal, bool bSerialorNot, const wchar_t* szDefVal/* = L""*/)
{
    if (bSerialorNot)
    {
        SetString(szKey, strVal.c_str());
    }
    else
    {
        strVal = GetString(szKey, szDefVal);
    }
}

void MtIniW::SerialVecInt(const wchar_t* szKey, std::vector<int>& nVecIntVal, bool bSerialorNot)
{
    if (bSerialorNot)
    {
        DelKeyName(szKey);
        wchar_t szBuf[MAX_BUFF_STRING];

        swprintf_s(szBuf, L"%s_Count", szKey);
        SetInt(szBuf, nVecIntVal.size());

        for (size_t i = 0; i < nVecIntVal.size(); i++)
        {
            swprintf_s(szBuf, L"%s_%03d", szKey, i);
            SetInt(szBuf, nVecIntVal[i]);
        }
    }
    else
    {
        nVecIntVal.clear();

        wchar_t szBuf[MAX_BUFF_STRING];

        swprintf_s(szBuf, L"%s_Count", szKey);
        int nCount = GetInt(szBuf, 0);

        for (size_t i = 0; i < nCount; i++)
        {
            swprintf_s(szBuf, L"%s_%03d", szKey, i);
            int nVal = GetInt(szBuf, 0);

            nVecIntVal.push_back(nVal);
        }
    }
}