
// Test_CapDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Test_Cap.h"
#include "Test_CapDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTest_CapDlg 对话框




CTest_CapDlg::CTest_CapDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTest_CapDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_CapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTest_CapDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTest_CapDlg 消息处理程序
typedef BOOL (FAR WINAPI *LAYERFUNC)(HWND,COLORREF,BYTE,DWORD);
BOOL CTest_CapDlg::SetLayeredWindowAttributes(HWND hwnd,COLORREF crKey,BYTE bAlpha,DWORD dwFlags)
{
	LAYERFUNC SetLayer;
	HMODULE hmod = LoadLibrary(L"user32.dll");
	SetLayer=(LAYERFUNC)GetProcAddress(hmod,"SetLayeredWindowAttributes");
	BOOL bReturn = SetLayer(hwnd,crKey,bAlpha,dwFlags);
	FreeLibrary(hmod);
	return bReturn;
}

BOOL CTest_CapDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//设置窗体透明   
	LONG t = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	
	t |= WS_EX_LAYERED;
	
	SetWindowLong(m_hWnd, GWL_EXSTYLE, t);
	//
	SetTimer(0,20,NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTest_CapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTest_CapDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTest_CapDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//保存当前视频图像为bmp文件
bool CTest_CapDlg::SaveBmpFile(short sFrameWidth,short sFrameHeight,short sColorSpace,const char *apBmpData, TCHAR *pFileName,DWORD dwLen)
{
	FILE* lpFilePtr = NULL;
	BITMAPFILEHEADER lBmpFileHeader;
	BITMAPINFOHEADER lBmpInfoHeader;	//绘制视频图像用到的位图信息
	int liBmpFileSize = 0;
	int nSizeImage = sFrameWidth * sFrameHeight * sColorSpace / 8;

	//得到BMP文件大小
	liBmpFileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nSizeImage;

	//填充BMP文件头
	lBmpFileHeader.bfType = 19778;
	lBmpFileHeader.bfSize = liBmpFileSize;
	lBmpFileHeader.bfReserved1 = 0;
	lBmpFileHeader.bfReserved2 = 0;
	lBmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//填充BMP信息头
	lBmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	lBmpInfoHeader.biPlanes = 1;
	lBmpInfoHeader.biCompression = BI_RGB;
	lBmpInfoHeader.biXPelsPerMeter = lBmpInfoHeader.biYPelsPerMeter = 0;
	lBmpInfoHeader.biClrImportant = lBmpInfoHeader.biClrUsed = 0;
	lBmpInfoHeader.biSizeImage = nSizeImage;
	lBmpInfoHeader.biWidth = sFrameWidth;
	lBmpInfoHeader.biHeight = -sFrameHeight;
	lBmpInfoHeader.biBitCount = sColorSpace;

	//创建、写入和关闭BMP文件
	lpFilePtr = _tfopen(pFileName,_T("wb+"));
	if (lpFilePtr)
	{
		fwrite(&lBmpFileHeader,sizeof(char),sizeof(BITMAPFILEHEADER),lpFilePtr);
		fwrite(&lBmpInfoHeader,sizeof(char),sizeof(BITMAPINFOHEADER),lpFilePtr);
		fwrite(apBmpData,sizeof(char),nSizeImage,lpFilePtr);
		fclose(lpFilePtr);
	}
	return true;
}
 
void CTest_CapDlg::OnTimer(UINT_PTR nIDEvent)
{
	CRect re;
	::GetWindowRect(m_hWnd , re);

	switch(nIDEvent)
	{
	case 0:
		{
			//
		HDC hdcold = ::GetDC(GetDesktopWindow()->m_hWnd);
		
		HDC hdc = CreateCompatibleDC(hdcold);

		HBITMAP hBitmap = CreateCompatibleBitmap(hdcold,re.Width(),re.Height());

		HBITMAP hBmpOld = (HBITMAP) SelectObject(hdc,hBitmap);

		BOOL bre = BitBlt(hdc,0,0,re.Width(),re.Height(),hdcold,re.left,re.top,SRCCOPY);

		SelectObject(hdc,hBmpOld);
		//
		BITMAP bitmap;
		
		GetObject(hBitmap,sizeof(BITMAP),&bitmap);

		int iBmpLenTemp = ((bitmap.bmWidth * bitmap.bmBitsPixel + 31) / 32) * 4 * bitmap.bmHeight;

		BYTE *pbyBmpTemp = new BYTE[iBmpLenTemp];
		
		memset(pbyBmpTemp, 0, iBmpLenTemp);

		// 从BITMAP中获得数据
		CBitmap* pBmp = CBitmap::FromHandle(hBitmap);

		DWORD dwCopyLen = pBmp->GetBitmapBits(iBmpLenTemp, pbyBmpTemp);

		//
		bool br = SaveBmpFile(bitmap.bmWidth,bitmap.bmHeight,32,(const char*)pbyBmpTemp,L"c:\\quanwei\\BMP\\1.bmp",dwCopyLen);

		DeleteObject(hBitmap);
		
		DeleteDC(hdc);

		::ReleaseDC(NULL,hdcold);

		break;
		}
	default:
		break;
	}
}
