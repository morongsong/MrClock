/**
  *@file         DigitNumWnd.h
  *@brief        DigitNumWnd
  *@author       jimsmorong
  *@date         2014年10月14日
*/

#ifndef DigitNumWnd_h__
#define DigitNumWnd_h__
#pragma once


#include <GdiPlus.h>
using namespace Gdiplus;
#include <atlimage.h>


/// 倒计时窗口
class DigitNumWnd : public CWnd
{
    DECLARE_DYNAMIC(DigitNumWnd)
public:
    DigitNumWnd();
    virtual ~DigitNumWnd();
protected:
    DECLARE_MESSAGE_MAP()
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
private:
    CImage m_oImageBkg;
    CImage m_oImage;
public:
    int Show();
    Image *m_pImageNum;
    HDC m_hdcMemory;
    BLENDFUNCTION m_Blend;

    void DrawDigitNum();
    POINT m_ptSrc;

    typedef BOOL (WINAPI *MYFUNC)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);
    MYFUNC UpdateLayeredWindow;

    int Hide();
    void SetAlwaysTop(bool bTopAlways);

    bool GetAlwaysTop();
    bool m_bAlwaysTop;
};



#endif //DigitNumWnd_h__