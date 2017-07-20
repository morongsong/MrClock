/** 
   *@file         MtIni.hpp
   *@brief        MtIni.hpp Metus Ini 
                  可支持unicode utf8  非windows api 多次读写文件
                  注意 key 一定不能含有=字符
   *@version      1.4.0
 */
#ifndef __MTINI_HPP___
#define __MTINI_HPP___
#pragma once


class MtIniW;
class MtIniA;

#if (defined(_UNICODE) || defined (UNICODE))
typedef MtIniW MtIni ;
#else
typedef MtIniA MtIni ;
#endif

class CSimpleIniAP;
class CSimpleIniWP;

/// MtIniA
class MtIniA
{
public:
    MtIniA();                           
    MtIniA(const char* szFilePath);
    MtIniA(const char* szFilePath,const char* szSection);
    ~MtIniA();                       
public:
    bool LoadFile(const char* szFilePath,bool bAutoSave=true,bool bUseUtf8=false);
    bool SaveFile();
    const char* GetFile(){return m_szFilePath;}
public:
    void SetSection(const char* szSection);
    bool DelSection(const char* szSection);
    bool DelKeyName(const char* szKeyName);
    const char* GetSection();
    bool GetSectionS(std::vector<std::string>& vecSec);
    bool GetKeyS(const char* szSection,std::vector<std::string>& vecSec);
public:
    bool SetInt(const char* szKey,int nVal);
    int  GetInt(const char* szKey,int nDefVal = 0);
public:
    bool SetString(const char* szKey,const char* szVal);
    const char* GetString(const char* szKey,const char* szDefVal = NULL);
public:
    bool SetDouble(const char* szKey,double dVal);
    double GetDouble(const char* szKey,double dDefVal=0);
    void SetDefDoubleBitNum(int num) {m_nDefDoubleBitNum = num; }
public:
    void SerialInt(const char* szKey, int& nVal, bool bSerialorNot, int nDefVal = 0);
    void SerialDouble(const char* szKey, double& nVal, bool bSerialorNot, double dDefVal = 0);
    void SerialString(const char* szKey, std::string& strVal, bool bSerialorNot, const char* szDefVal = "");
public:
    void SerialVecInt(const char* szKey, std::vector<int>& nVecIntVal, bool bSerialorNot);
private:
    static const int MAX_BUFF_STRING  = 2048; ///< 缓冲区大小
    char m_szFilePath[MAX_BUFF_STRING];
    char m_szSection[MAX_BUFF_STRING];
    char m_szValTemp[MAX_BUFF_STRING];
    int m_nDefDoubleBitNum;
    bool m_bAutoSave;                ///< 设置AutoSave
    bool m_bUseUtf8;                 ///< 设置UseUft8
    bool m_bOpen;                    ///< Ini文件是否打开
    CSimpleIniAP* m_pSimpleIni;       ///< 实现的类
};


/// MtIniW
class MtIniW
{
public:
    MtIniW();                           
    MtIniW(const wchar_t* szFilePath);
    MtIniW(const wchar_t* szFilePath,const wchar_t* szSection);
    ~MtIniW();                       
public:
    bool LoadFile(const wchar_t* szFilePath,bool bAutoSave=true,bool bUseUtf8=false);
    bool SaveFile();
    const wchar_t* GetFile(){return m_szFilePath;}
public:
    void SetSection(const wchar_t* szSection);
    bool DelSection(const wchar_t* szSection);
    bool DelKeyName(const wchar_t* szKeyName);
    const wchar_t* GetSection();
    bool GetSectionS(std::vector<std::wstring>& vecSec);
    bool GetKeyS(const wchar_t* szSection,std::vector<std::wstring>& vecSec);
public:
    bool SetInt(const wchar_t* szKey,int nVal);
    int  GetInt(const wchar_t* szKey,int nDefVal = 0);
public:
    bool SetString(const wchar_t* szKey,const wchar_t* szVal);
    const wchar_t* GetString(const wchar_t* szKey,const wchar_t* szDefVal = NULL);
public:
    bool SetDouble(const wchar_t* szKey,double dVal);
    double GetDouble(const wchar_t* szKey,double dDefVal=0);
    void SetDefDoubleBitNum(int num) {m_nDefDoubleBitNum = num; }
public:
    void SerialInt(const wchar_t* szKey, int& nVal, bool bSerialorNot, int nDefVal = 0);
    void SerialDouble(const wchar_t* szKey, double& nVal, bool bSerialorNot, double dDefVal = 0);
    void SerialString(const wchar_t* szKey, std::wstring& strVal, bool bSerialorNot, const wchar_t* szDefVal = L"");
public:
    void SerialVecInt(const wchar_t* szKey, std::vector<int>& nVecIntVal, bool bSerialorNot);
private:
    static const int MAX_BUFF_STRING  = 2048; ///< 缓冲区大小
    wchar_t m_szFilePath[MAX_BUFF_STRING];
    wchar_t m_szSection[MAX_BUFF_STRING];
    wchar_t m_szValTemp[MAX_BUFF_STRING];
    int m_nDefDoubleBitNum;
    bool m_bAutoSave;                ///< 设置AutoSave
    bool m_bUseUtf8;                 ///< 设置UseUft8
    bool m_bOpen;                    ///< Ini文件是否打开
    CSimpleIniWP* m_pSimpleIni;       ///< 实现的类
};

#endif //__MTINI_HPP___
