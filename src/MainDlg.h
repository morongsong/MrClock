
// MainDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "ResizeCtrl.h"
#include "DigitNumWnd.h"

class CMainDlg : public CDialogEx
{
public:
    CMainDlg(CWnd* pParent = NULL);	
    virtual ~CMainDlg();
    enum { IDD = IDD_MAIN_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
protected:
    HICON m_hIcon;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
    afx_msg void OnClose();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnTrayNotify(WPARAM /*wp*/, LPARAM lp);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
    afx_msg void OnBnClickedButtonFullScreen();
    afx_msg void OnBnClickedButtonShowText();
    afx_msg void OnBnClickedButtonShowClock();
    afx_msg void OnBnClickedButtonUnlock();
    DECLARE_MESSAGE_MAP()
public:
    bool m_bShowDigitWnd;

    bool m_bUsePt;

    int m_nCurUnLockMin = -1;


    DigitNumWnd m_digitWnd;
    CRichEditCtrl m_edtText;
    CResizeCtrl m_oResizeCtrl;
    NOTIFYICONDATA	m_nid;
	bool m_bFullScreen;
	WINDOWPLACEMENT m_wpPrev;
	CRect m_rcFullScreenRect;
	bool IsFullScreen(void) { return m_bFullScreen; }
	void FullScreenView();
public:
    void UpdateStaticTm();
    void LockScreen();
    void UnLockScreen();
    bool IsNeedLockTm(SYSTEMTIME& oTmLoc);
    bool m_bInLocking = false;

    tstring m_strLockMin1;
    tstring m_strLockMin2;
    std::set<int> m_setMinNeedLock;

    std::set<int> m_setUnLockMin;
};
