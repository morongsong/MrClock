

#include "stdafx.h"
#include <afx.h>
#include <afxwin.h>
#include <afxext.h>
#include <afxcontrolbarutil.h>
#include <atlimage.h>
#include "IDB_DIGIT_NUM.png.h"
#include "DigitNumWnd.h"

#define TIME_UPDATE_EVENT 2003
#define TIMEALWAYS_ONTOP  2004



IMPLEMENT_DYNAMIC(DigitNumWnd, CWnd)

DigitNumWnd::DigitNumWnd()
{
    m_bAlwaysTop = false;
}

DigitNumWnd::~DigitNumWnd()
{
}


BEGIN_MESSAGE_MAP(DigitNumWnd, CWnd)
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void DigitNumWnd::OnPaint()
{

    return CWnd::OnPaint();
}

void DigitNumWnd::DrawDigitNum()
{
#if 0
    CDC *DC = GetDC();
    CMemDC memdc(*DC,this);

    HDC hdcScreen = ::GetDC(m_hWnd);

    RECT rct;
    GetWindowRect(&rct);
    POINT ptWinPos= {rct.left,rct.top};
    HDC hdcmem = memdc.GetDC().m_hDC;
    ::SetStretchBltMode(hdcmem,HALFTONE); // 使用最高品質的方式



    if (m_oImage.GetBPP() == 32)
    {
        for (int i = 0; i < m_oImage.GetWidth(); i++)
        {
            for (int j = 0; j < m_oImage.GetHeight(); j++)
            {
                byte *pByte = (byte *)m_oImage.GetPixelAddress(i, j);
                pByte[0] = pByte[0] * pByte[3] / 255;
                pByte[1] = pByte[1] * pByte[3] / 255;
                pByte[2] = pByte[2] * pByte[3] / 255;
            }
        }
    }

    SYSTEMTIME SystemTime;   // address of system time structure
    GetLocalTime(&SystemTime);

    m_oImageBkg.Draw(hdcmem,0,0);

    //H
    m_oImage.Draw(hdcmem,0,   0,14,23,14*(SystemTime.wHour/10),0,14,23  /*,AC_SRC_ALPHA*/);
    m_oImage.Draw(hdcmem,20,0,14,23,14*(SystemTime.wHour%10),0,14,23    /*,AC_SRC_ALPHA*/);
    m_oImage.Draw(hdcmem,20*2,0,14,23,140,0,14,23                       /*,AC_SRC_ALPHA*/);

    //M
    m_oImage.Draw(hdcmem,20*3,0,14,23,14*(SystemTime.wMinute/10),0,14,23/*,AC_SRC_ALPHA*/);
    m_oImage.Draw(hdcmem,20*4,0,14,23,14*(SystemTime.wMinute%10),0,14,23/*,AC_SRC_ALPHA*/);
    m_oImage.Draw(hdcmem,20*5,0,14,23,140,0,14,23                       /*,AC_SRC_ALPHA*/);

    //S
    m_oImage.Draw(hdcmem,20*6,0,14,23,14*(SystemTime.wSecond/10),0,14,23/*,AC_SRC_ALPHA*/);
    m_oImage.Draw(hdcmem,20*7,0,14,23,14*(SystemTime.wSecond%10),0,14,23/*,AC_SRC_ALPHA*/);


    SIZE sizeWindow= {m_oImage.GetWidth(), m_oImage.GetHeight()};
    POINT ptPos = {0, 0};
    POINT ptSrc = {0, 0};
    ::UpdateLayeredWindow(m_hWnd, hdcScreen, &ptPos, &sizeWindow, hdcmem, &ptSrc, 0, &m_Blend, ULW_ALPHA);


#endif
    int Transparent = 255;
    int m_BakWidth = 160;
    int m_BakHeight = 30;
    HDC hdcTemp=GetDC()->m_hDC;
    m_hdcMemory=CreateCompatibleDC(hdcTemp);
    HBITMAP hBitMap=CreateCompatibleBitmap(hdcTemp,m_BakWidth,m_BakHeight);
    SelectObject(m_hdcMemory,hBitMap);
    if(Transparent<0||Transparent>100)	Transparent=100;

    m_Blend.SourceConstantAlpha=int(Transparent*2.55);//1~255
    HDC hdcScreen=::GetDC (m_hWnd);
    RECT rct;
    GetWindowRect(&rct);
    POINT ptWinPos= {rct.left,rct.top};
    Graphics graph(m_hdcMemory);

    Point points[] = { Point(0, 0),
                       Point(m_BakWidth, 0),
                       Point(0, m_BakHeight)
                     };

    SYSTEMTIME SystemTime;   // address of system time structure
    GetLocalTime(&SystemTime);
    //H
    graph.DrawImage(m_pImageNum,0, 0, 14*(SystemTime.wHour/10), 0,14,23,UnitPixel);
    graph.DrawImage(m_pImageNum,20,0, 14*(SystemTime.wHour%10), 0,14,23,UnitPixel);
    //:
    graph.DrawImage(m_pImageNum,20*2,0, 140, 0,14,23,UnitPixel);
    //M
    graph.DrawImage(m_pImageNum,20*3, 0, 14*(SystemTime.wMinute/10), 0,14,23,UnitPixel);
    graph.DrawImage(m_pImageNum,20*4,0, 14*(SystemTime.wMinute%10), 0,14,23,UnitPixel);
    //:
    graph.DrawImage(m_pImageNum,20*5,0, 140, 0,14,23,UnitPixel);
    //S
    graph.DrawImage(m_pImageNum,20*6, 0, 14*(SystemTime.wSecond/10), 0,14,23,UnitPixel);
    graph.DrawImage(m_pImageNum,20*7,0, 14*(SystemTime.wSecond%10), 0,14,23,UnitPixel);

    SIZE sizeWindow= {m_BakWidth,m_BakHeight};
    POINT ptSrc= {0,0};
    DWORD dwExStyle=GetWindowLong(m_hWnd,GWL_EXSTYLE);
    if((dwExStyle&0x80000)!=0x80000)
        SetWindowLong(m_hWnd,GWL_EXSTYLE,dwExStyle^0x80000);

    BOOL bRet=FALSE;
    bRet= UpdateLayeredWindow( m_hWnd,hdcScreen,&ptWinPos,
                               &sizeWindow,m_hdcMemory,&ptSrc,0,&m_Blend,2);
    graph.ReleaseHDC(m_hdcMemory);
    ::ReleaseDC(m_hWnd,hdcScreen);
    hdcScreen=NULL;
    ::ReleaseDC(m_hWnd,hdcTemp);
    hdcTemp=NULL;
    DeleteObject(hBitMap);
    DeleteDC(m_hdcMemory);
    m_hdcMemory=NULL;

    //::SetWindowPos(m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    return ;
}

void DigitNumWnd::OnTimer(UINT_PTR nIDEvent)
{
    if (nIDEvent == TIME_UPDATE_EVENT)
    {
        KillTimer(TIME_UPDATE_EVENT);
        DrawDigitNum();
        SetTimer(TIME_UPDATE_EVENT,1000,NULL);
    }
    else if (nIDEvent == TIMEALWAYS_ONTOP)
    {
        KillTimer(TIMEALWAYS_ONTOP);
        ::SetWindowPos(m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
        if (m_bAlwaysTop)
        {
            SetTimer(TIMEALWAYS_ONTOP,1000,NULL);
        }
    }

    CWnd::OnTimer(nIDEvent);
}

int DigitNumWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    HMODULE hFuncInst = LoadLibrary(_T("User32.DLL"));
    BOOL bRet=FALSE;
    if(hFuncInst)
        UpdateLayeredWindow=(MYFUNC)GetProcAddress(hFuncInst,("UpdateLayeredWindow"));
    else
    {
        AfxMessageBox(_T("Load User32.dll ERROR!"));
        exit(0);
    }

    {
        HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED,IDB_DIGIT_NUM_png_size);
        BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
        memcpy(pmem,IDB_DIGIT_NUM_png,IDB_DIGIT_NUM_png_size);

        IStream* pstm;
        CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);

        m_oImage.Load(pstm);
        m_pImageNum = Gdiplus::Image::FromStream(pstm);

        GlobalUnlock(m_hMem);
        pstm->Release();
    }
    m_oImageBkg.Create(155,24,32);

    //设置为图层窗口
    ::SetWindowLong(m_hWnd, GWL_EXSTYLE,
                    ::GetWindowLong(m_hWnd, GWL_EXSTYLE)
                    | WS_EX_TRANSPARENT //鼠标穿透（不会成为焦点）
                    | WS_EX_LAYERED);	//支持透明度
    //::SetLayeredWindowAttributes(m_hWnd,RGB(255,255,255),0x80, LWA_COLORKEY);

    //调整窗口大小，并设置为TopMost
    int x, y;
    x = (GetSystemMetrics(SM_CXSCREEN) - 160)/2;
    y = (GetSystemMetrics(SM_CYSCREEN) - 120)/2;
    ::SetWindowPos(m_hWnd, HWND_TOPMOST, x, y, m_oImage.GetWidth()+10, m_oImage.GetHeight()+10, SWP_NOACTIVATE);
    m_ptSrc.x = x;
    m_ptSrc.y = y;
    return 0;
}

BOOL DigitNumWnd::OnEraseBkgnd(CDC* pDC)
{
    return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

int DigitNumWnd::Show()
{
    if(m_hWnd == NULL)
    {
        CRect rc(0,0,160,30);
        DWORD exStyle = WS_EX_TOOLWINDOW;
        CreateEx(exStyle,
                 AfxRegisterWndClass(0), NULL,
                 WS_POPUP ,
                 0, 0, 0, 0,
                 NULL, (HMENU)0);

        m_Blend.BlendOp      = AC_SRC_OVER;
        m_Blend.BlendFlags   = 0;
        m_Blend.AlphaFormat  = AC_SRC_ALPHA;
        m_Blend.SourceConstantAlpha=255;
    }
    SetTimer(TIME_UPDATE_EVENT,1000,NULL);
    ShowWindow(SW_SHOW);
    ::SetWindowPos(m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    return 0;
}

int DigitNumWnd::Hide()
{
    KillTimer(TIME_UPDATE_EVENT);
    ShowWindow(SW_HIDE);
    return 1;
}

void DigitNumWnd::SetAlwaysTop(bool bTopAlways)
{
    if (bTopAlways)
    {
        if (!m_bAlwaysTop)
        {
            if (GetSafeHwnd())
            {
                ::SetWindowPos(m_hWnd,HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
                SetTimer(TIMEALWAYS_ONTOP,3000,NULL);
                m_bAlwaysTop = true;
            }

        }
    }
    else
    {
        if (m_bAlwaysTop)
        {
            KillTimer(TIMEALWAYS_ONTOP);
            m_bAlwaysTop = false;
        }
    }
}


bool DigitNumWnd::GetAlwaysTop()
{
    return m_bAlwaysTop;
}

