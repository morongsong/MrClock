
// The.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TheApp.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTheApp

BEGIN_MESSAGE_MAP(CTheApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTheApp ����

CTheApp::CTheApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTheApp ����

CTheApp theApp;


// CTheApp ��ʼ��

BOOL CTheApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    AfxOleInit();
    AfxInitRichEdit2();

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMainDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}


class CGlobalVarInit
{
public:
    CGlobalVarInit()
    {
        wstring strPATHPath;
        {
            size_t reqsize;
            _wgetenv_s(&reqsize, nullptr, 0, L"PATH");
            WCHAR* pszBuf = new WCHAR[reqsize + 10];
            _wgetenv_s(&reqsize, pszBuf, reqsize + 10, L"PATH");
            strPATHPath = pszBuf;
            delete pszBuf;
        }

        wstring strExePath;
        {
            WCHAR szBuff[MAX_PATH];
            GetModuleFileNameW(NULL, szBuff, MAX_PATH);
            int i = wcslen(szBuff);
            for (; i; i--)
            {
                if (szBuff[i] == L'\\')
                {
                    szBuff[i] = L'\0';
                    break;
                }
            }
            strExePath = szBuff;
        }

        wstring strNewPATHPath = strExePath;
#ifndef _WIN64
        strNewPATHPath += L";";
#else
        strNewPATHPath += L";";
#endif

        strNewPATHPath += strPATHPath;
        _wputenv_s(L"PATH", strNewPATHPath.c_str());


        {
            size_t reqsize;
            _wgetenv_s(&reqsize, nullptr, 0, L"PATH");
            WCHAR* pszBuf = new WCHAR[reqsize + 10];
            _wgetenv_s(&reqsize, pszBuf, reqsize + 10, L"PATH");
            std::wstring strPath = pszBuf;
            delete pszBuf;
        }

        ::SetLastError(NO_ERROR);
        HANDLE handleApp = ::CreateMutex(NULL, TRUE, _T("MrClock[[Guid(C4BCFF7C-AC06-43FA-AE26-F6C0217CD657)]]"));
        if (NULL == handleApp)
        {
            MessageBox(NULL, _T("Create MrClock Error!"), _T("NoPrev"), MB_OK);
            exit(0);
            return;
        }
        if (GetLastError() == ERROR_ALREADY_EXISTS)
        {
            MessageBox(NULL, _T("MrClock Already Run!"), _T("MrClock"), MB_OK);
            exit(0);
            return;
        }
    }
};
#pragma warning(push)
#pragma warning(disable:4378 4075)
#pragma const_seg(".CRT$XCT")
#pragma init_seg(".CRT$XCT")
#pragma warning(pop)
const CGlobalVarInit gCGlobalVarInit;