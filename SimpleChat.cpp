// SimpleChat.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SimpleChat.h"
#include "communicate.h"
#include "resource.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' \
processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

HINSTANCE hgInst;
int WINAPI WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hgInst = hInstace;
    HWND hdlg = CreateDialog(hInstace, MAKEINTRESOURCE(IDD_MAIN), GetDesktopWindow(),(DLGPROC)DlgProc);//显示对话框
    if (!hdlg)
    {
	return 0;
    }
    ShowWindow(hdlg, SW_SHOW);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }
    return 0;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
	    {
		// 设置对话框的图标     
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hgInst, MAKEINTRESOURCE(IDI_SIMPLECHAT)));
		break;
	    }
    case WM_COMMAND:
	    {
		int wmId = LOWORD(wParam);
		switch (wmId) {
			case IDOK:
				{
				communicate sign_in(L"222.20.105.51", 12000);//test
				sign_in.sconnect();
				sign_in.sendmsg("123");
				break;
				}
		}
	    }
	
    case WM_SYSCOMMAND:
	    {
		if (wParam == SC_CLOSE)
		{
			PostQuitMessage(0);//退出     
		}
		break;
	    }
	
    default:
	return DefWindowProc(hDlg, message, wParam, lParam);
    }

    return (INT_PTR)FALSE;
}