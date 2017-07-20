
// The.cpp : 定义应用程序的类行为。
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


// CTheApp 构造

CTheApp::CTheApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTheApp 对象

CTheApp theApp;


// CTheApp 初始化

BOOL CTheApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

    AfxOleInit();
    AfxInitRichEdit2();

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CMainDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
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