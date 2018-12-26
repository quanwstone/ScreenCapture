
// Test_CapDlg.h : 头文件
//

#pragma once


// CTest_CapDlg 对话框
class CTest_CapDlg : public CDialogEx
{
// 构造
public:
	CTest_CapDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_CAP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool SaveBmpFile(short sFrameWidth,short sFrameHeight,short sColorSpace,const char *apBmpData, TCHAR *pFileName,DWORD dwLen);

	BOOL SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags);

public:
	BYTE	*m_pbyBmpTemp ;
	bool	m_bFirst;
	int		m_iFrameCount;
	LARGE_INTEGER m_StatTime;
	LARGE_INTEGER m_EndTime;
	LARGE_INTEGER m_Frequency;

};
