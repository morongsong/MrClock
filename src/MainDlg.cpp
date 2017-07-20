#include "stdafx.h"
#include "TheApp.h"
#include "MainDlg.h"
#include "MtIni.h"
#include "shellapi.h"

#include <TlHelp32.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  IDC_TRAYICON     2012
#define  IDT_CHECKTIME    1093
#define  IDT_CHECKCLOSE   1094
#define  NUM_CHECK_DISTIME        1000
#define  TIMEDIS_IDT_CHECKCLOSE   1000
#define  UM_TRAYNOTIFY	(WM_USER + 1)

#define TIME_DIS_CHECK 150

class CAboutDlg : public CDialog
{
public:
    CAboutDlg() : CDialog(CAboutDlg::IDD) {}
    enum { IDD = IDD_ABOUTBOX };
protected:
    virtual void DoDataExchange(CDataExchange* pDX) {
        CDialog::DoDataExchange(pDX);
    }
protected:
    DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMainDlg 对话框
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CMainDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // Initialize NOTIFYICONDATA
    memset(&m_nid, 0, sizeof(m_nid));
    m_nid.cbSize = sizeof(m_nid);
    m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
    m_bShowDigitWnd = false;
    m_bFullScreen = false;
}

CMainDlg::~CMainDlg()
{
    m_nid.hIcon = NULL;
    Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT1, m_edtText);
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //ON_COMMAND(ID_TRAYICON_CLOSE, OnTrayiconClose)
    //}}AFX_MSG_MAP
    ON_WM_CLOSE()
    ON_WM_TIMER()
    ON_BN_CLICKED(IDOK, &CMainDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CMainDlg::OnBnClickedCancel)
    ON_MESSAGE(UM_TRAYNOTIFY, OnTrayNotify)
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDC_BUTTON_FULL_SCREEN, &CMainDlg::OnBnClickedButtonFullScreen)
    ON_BN_CLICKED(IDC_BUTTON_SHOW_TEXT, &CMainDlg::OnBnClickedButtonShowText)
    ON_BN_CLICKED(IDC_BUTTON_SHOW_CLOCK, &CMainDlg::OnBnClickedButtonShowClock)
    ON_BN_CLICKED(IDC_BUTTON_UNLOCK, &CMainDlg::OnBnClickedButtonUnlock)
END_MESSAGE_MAP()


//字符串分割函数
std::vector<std::tstring> split(std::tstring& str, std::tstring& pattern)
{
    std::tstring::size_type pos;
    std::vector<std::tstring> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();

    for (int i = 0; i<size; i++)
    {
        pos = str.find(pattern, i);
        if (pos<size)
        {
            std::tstring s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

BOOL CMainDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    m_oResizeCtrl.Create(GetSafeHwnd());
    CRect rct;
    GetClientRect(&rct);
    m_oResizeCtrl.SetInitialSize(rct.Size());
    m_oResizeCtrl.SetMinimumTrackingSize();
    CResizeInfo oResizeInfo[] =
    {
        { IDOK, 100, 100, 0, 0 },
        { IDCANCEL, 100, 100, 0, 0 },
        //---
        { IDC_STATIC_TIME, 0, 0, 0, 0 },
        //---
        { IDC_EDIT1, 0, 0, 100, 100 },
        { IDC_STATIC_PASSWORD, 100, 100, 0, 0 },
        { IDC_EDIT_PASSWORKD,100, 100, 0, 0 },
        //---
        { IDC_BUTTON_UNLOCK, 100, 100, 0, 0 },
        { IDC_BUTTON_FULL_SCREEN, 100, 0, 0, 0 },
        //---
        { IDC_BUTTON_SHOW_CLOCK, 100, 0, 0, 0 },
        { IDC_BUTTON_SHOW_TEXT, 100, 100, 0, 0 },//显示文本
    };
    m_oResizeCtrl.Add(oResizeInfo);


    SetTimer(IDT_CHECKTIME, TIME_DIS_CHECK, NULL);

    CButton* btnUnLock = (CButton*)GetDlgItem(IDC_BUTTON1);
    btnUnLock->ShowWindow(SW_SHOW);

    m_nid.hWnd = GetSafeHwnd();
    m_nid.uCallbackMessage = UM_TRAYNOTIFY;
    m_nid.hIcon = m_hIcon;

    CString strToolTip = _T("MrClock");
    _tcsncpy_s(m_nid.szTip, strToolTip, strToolTip.GetLength());

    Shell_NotifyIcon(NIM_ADD, &m_nid);

    MtIni oIni(_T("MrClock.ini"));
    oIni.SetSection(_T("Default"));
    m_strLockMin1 = oIni.GetString(_T("LockMin1"), _T("58,59,00,01,02"));
    m_strLockMin2 = oIni.GetString(_T("LockMin2"), _T("28,29,30,31,32"));
    oIni.SetString(_T("LockMin1"), m_strLockMin1.c_str());
    oIni.SetString(_T("LockMin2"), m_strLockMin2.c_str());
    
    m_bUsePt = oIni.GetInt(_T("bUsePt"), 0);
    oIni.SetInt(_T("bUsePt"), m_bUsePt);

    vectstring as;
    as = split(m_strLockMin1, tstring(_T(",")));
    for (auto& str : as)
    {
        m_setMinNeedLock.insert(_ttoi(str.c_str()));
    }
    as = split(m_strLockMin2, tstring(_T(",")));
    for (auto& str : as)
    {
        m_setMinNeedLock.insert(_ttoi(str.c_str()));
    }

    //m_nNeedLockMin
    return TRUE;
}

void CMainDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else if (nID == SC_MINIMIZE)
    {
        ShowWindow(SW_HIDE);
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

void CMainDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CRect   rect;
        CPaintDC   dc(this);
        GetClientRect(rect);
        dc.FillSolidRect(rect, RGB(0, 0, 0));   //设置为绿色背景
        CDialog::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CMainDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMainDlg::OnClose()
{
    ShowWindow(SW_HIDE);
    //CDialog::OnClose();
}

void CMainDlg::UpdateStaticTm()
{
    CStatic* pTime = (CStatic*)GetDlgItem(IDC_STATIC_TIME);
    time_t rawtime;
    time(&rawtime);
    struct tm timeinfo;
    localtime_s(&timeinfo, &rawtime);
    TCHAR szTime[260];
    _stprintf_s(szTime, _T("现在时间:%02d:%02d:%02d"), timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    pTime->SetWindowText(szTime);
}

void CMainDlg::LockScreen()
{
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    ShowWindow(SW_SHOW);
    SetForegroundWindow();
    if (!IsFullScreen())
    {
        FullScreenView();
    }
    m_digitWnd.SetAlwaysTop(false);
    
    GetDlgItem(IDC_BUTTON_FULL_SCREEN)->ShowWindow(SW_HIDE);
    return;
}


void CMainDlg::UnLockScreen()
{
    ::SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    GetDlgItem(IDC_BUTTON_FULL_SCREEN)->ShowWindow(SW_SHOW);
    if (IsFullScreen())
    {
        FullScreenView();
    }
    if (m_bShowDigitWnd)
    {
        m_digitWnd.SetAlwaysTop(true);
    }
    ShowWindow(SW_HIDE);
    return;
}

bool CMainDlg::IsNeedLockTm(SYSTEMTIME& oTmLoc)
{
    if (m_setMinNeedLock.find(oTmLoc.wMinute) != m_setMinNeedLock.end())
    {
        return true;
    }
    return false;
}


bool traverseProcesses(std::map<std::tstring, int>& _nameID)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        //std::cout << "CreateToolhelp32Snapshot Error!" << std::endl;;
        return false;
    }

    BOOL bResult = Process32First(hProcessSnap, &pe32);

    int num(0);

    while (bResult)
    {
        std::tstring name = pe32.szExeFile;
        int id = pe32.th32ProcessID;

        //std::cout << "[" << ++num << "] : " << "Process Name:"
        //    << name << "  " << "ProcessID:" << id << std::endl;

        _nameID.insert(std::pair<tstring, int>(name, id)); //字典存储  
        bResult = Process32Next(hProcessSnap, &pe32);
    }

    CloseHandle(hProcessSnap);

    return true;
}

void CheckAndRunProcQk()
{
    std::map<std::tstring, int> mapNameID;

    if (!traverseProcesses(mapNameID))
    {
        return;
    }

    if (mapNameID.find(_T("MrClockPt.exe")) != mapNameID.end())
    {
        return;
    }
    //HANDLE handleApp = ::OpenMutex(NULL, FALSE, _T("MrClock[[Guid(C4BCFF7C-AC06-43FA-AE26-F6C0217CD657)]]"));
    //if (NULL == handleApp)
    {
        SHELLEXECUTEINFO si;
        memset(&si, 0, sizeof(si));
        si.cbSize = sizeof(SHELLEXECUTEINFO);
        si.fMask = SEE_MASK_NOCLOSEPROCESS;
        si.lpFile = _T("cmd.exe ");
        si.lpVerb = _T("open");
        si.nShow = SW_HIDE;
        si.lpParameters = _T("/c start \"\" MrClockPt");
        ::ShellExecuteEx(&si);
        WaitForSingleObject(si.hProcess, 60000);
        CloseHandle(si.hProcess);
    }
    return;
}

void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == IDT_CHECKTIME)
    {
        KillTimer(IDT_CHECKTIME);

        UpdateStaticTm();

        SYSTEMTIME oTmloc;
        GetLocalTime(&oTmloc);

        if (m_bUsePt)
        {
            CheckAndRunProcQk();
        }
        
        if (IsNeedLockTm(oTmloc))
        {
            m_bInLocking = true;

            if (m_setUnLockMin.find(oTmloc.wMinute) == m_setUnLockMin.end())
            {
                LockScreen();
            }
        }
        else
        {
            if (m_bInLocking)
            {
                m_setUnLockMin.clear();
                UnLockScreen();
            }
            m_bInLocking = false;
        }

        SetTimer(IDT_CHECKTIME, TIME_DIS_CHECK,0);
    }
    CDialog::OnTimer(nIDEvent);
}


void CMainDlg::OnBnClickedOk()
{
    ShowWindow(SW_HIDE);
}

void CMainDlg::OnBnClickedCancel()
{
    ShowWindow(SW_HIDE);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
        {
            UINT nID = GetFocus()->GetDlgCtrlID();
            if (nID != IDC_EDIT1)
            {
                return TRUE;
            }
        }
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    CStatic* pstcTime = (CStatic*)GetDlgItem(IDC_STATIC_BUILDTIME);

    TCHAR szBuildTime[100];
    _stprintf_s(szBuildTime, _T("Version:1.1   BuildTime: %s %s"), _T(__DATE__), _T(__TIME__));
    pstcTime->SetWindowText(szBuildTime);

    return TRUE;
}


LRESULT CMainDlg::OnTrayNotify(WPARAM /*wp*/, LPARAM lp)
{
    UINT uiMsg = (UINT)lp;

    switch (uiMsg)
    {
    case WM_RBUTTONUP:
        //OnTrayContextMenu();
        return 1;
    case WM_LBUTTONDBLCLK:
        ShowWindow(SW_SHOWNORMAL);
        return 1;
    }
    return 0;
}

void CMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    if (IsFullScreen())
    {
        lpMMI->ptMaxSize.y = m_rcFullScreenRect.Height();
        lpMMI->ptMaxTrackSize.y = lpMMI->ptMaxSize.y;
        lpMMI->ptMaxSize.x = m_rcFullScreenRect.Width();
        lpMMI->ptMaxTrackSize.x = lpMMI->ptMaxSize.x;
    }
    CDialogEx::OnGetMinMaxInfo(lpMMI);
}

void CMainDlg::FullScreenView()
{
    RECT rectDesktop;
    WINDOWPLACEMENT wpNew;
    if (!IsFullScreen())
    {
        // We'll need these to restore the original state.
        GetWindowPlacement(&m_wpPrev);
        //Adjust RECT to new size of window
        ::GetWindowRect(::GetDesktopWindow(), &rectDesktop);
        ::AdjustWindowRectEx(&rectDesktop, GetStyle(), FALSE, GetExStyle());
        // Remember this for OnGetMinMaxInfo()
        m_rcFullScreenRect = rectDesktop;
        wpNew = m_wpPrev;
        wpNew.showCmd = SW_SHOWNORMAL;
        wpNew.rcNormalPosition = rectDesktop;
        m_bFullScreen = true;

        SetMenu(NULL);//设置菜单为空

        ModifyStyle(0, WS_POPUP);
        ModifyStyle(WS_CAPTION, 0);
        ModifyStyle(WS_BORDER, 0);
        ModifyStyle(WS_THICKFRAME, 0);
    }
    else
    {
        // 退出全屏幕时恢复到原来的窗口状态
        m_bFullScreen = false;
        wpNew = m_wpPrev;

        ModifyStyle(0, WS_CAPTION);
        ModifyStyle(0, WS_BORDER);
        ModifyStyle(0, WS_THICKFRAME);
    }
    SetWindowPlacement(&wpNew);
}


void CMainDlg::OnBnClickedButtonFullScreen()
{
    FullScreenView();
}


void CMainDlg::OnBnClickedButtonShowText()
{
    CButton* pBtnCtrl = (CButton*)GetDlgItem(IDC_BUTTON3);
    static bool bShow = true;
    if (!bShow)
    {
        m_edtText.ShowWindow(SW_SHOW);
        bShow = true;
        pBtnCtrl->SetWindowText(_T("隐藏文本"));
    }
    else
    {
        m_edtText.ShowWindow(SW_HIDE);
        bShow = false;
        pBtnCtrl->SetWindowText(_T("显示文本"));
    }
}


void CMainDlg::OnBnClickedButtonShowClock()
{
    CButton* btnClock = (CButton*)GetDlgItem(IDC_BUTTON2);
    if (!m_bShowDigitWnd)
    {
        m_digitWnd.Show();
        m_bShowDigitWnd = true;
        btnClock->SetWindowText(_T("隐藏时钟"));
    }
    else
    {
        m_digitWnd.Hide();
        m_bShowDigitWnd = false;
        btnClock->SetWindowText(_T("显示时钟"));
    }
}


void CMainDlg::OnBnClickedButtonUnlock()
{
    if (m_bInLocking == false)
    {
        return;
    }
    return;
    CStatic* pStcPwd = (CStatic*)GetDlgItem(IDC_STATIC_PASSWORD);
    CEdit* pEdtPwd = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORKD);
    CButton* pBtnUnLock = (CButton*)GetDlgItem(IDC_BUTTON1);
    if (!pStcPwd->IsWindowVisible())
    {
        pStcPwd->ShowWindow(SW_SHOW);
        pEdtPwd->ShowWindow(SW_SHOW);
        pBtnUnLock->SetWindowText(_T("确定解锁"));
    }
    else
    {
        pStcPwd->ShowWindow(SW_HIDE);
        pEdtPwd->ShowWindow(SW_HIDE);
        CString strPwd;
        pEdtPwd->GetWindowText(strPwd);

        if (strPwd == _T("168888"))
        {
            SYSTEMTIME oTm;
            GetLocalTime(&oTm);

            m_setUnLockMin.clear();

            for (size_t i = 0; i < 60; i++)
            {
                int nCurMin = oTm.wMinute+i;
                if (nCurMin >= 60) nCurMin = nCurMin-60;
                if (m_setMinNeedLock.find(nCurMin) != m_setMinNeedLock.end())
                {
                    m_setUnLockMin.insert(nCurMin);
                }
                else
                {
                    break;
                }
            }

            ::SetWindowPos(m_hWnd,HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        }

        pEdtPwd->SetWindowText(_T(""));
        pBtnUnLock->SetWindowText(_T("解锁"));
    }
    return;
}
