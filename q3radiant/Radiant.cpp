/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/


#include "stdafx.h"
#include "Radiant.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "RadiantDoc.h"
#include "RadiantView.h"
#include "PrefsDlg.h"
#include "ccall.h"

BEGIN_MESSAGE_MAP(CRadiantApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

CRadiantApp::CRadiantApp() {
}

CRadiantApp theApp;

const char g_szClassName[] = "myWindowClass";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


class MFC_Tutorial_Window :public CFrameWnd
{
public:
    MFC_Tutorial_Window()
    {
        Create(NULL,"MFC Tutorial Part 1 CoderSource Window");
    }

};

class MyApp2 :public CWinApp
{
   //MFC_Tutorial_Window *wnd; 
   MFC_Tutorial_Window *wnd; 
public:
   BOOL InitInstance()
   {
       //wnd = new MFC_Tutorial_Window();
       //m_pMainWnd = wnd;
       //m_pMainWnd->ShowWindow(SW_SHOW);
       wnd = new MFC_Tutorial_Window();
       m_pMainWnd = wnd;
       m_pMainWnd->ShowWindow(SW_SHOW);
       return TRUE;
   }
};

extern int AFXAPI AfxWinInit(HINSTANCE hInstance, HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, int nCmdShow);

CCALL void init_app() {

	int nCmdShow = 10;


    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);



	HINSTANCE hInstance = (HINSTANCE) GetWindowLong(hwnd, GWL_HINSTANCE);


	//theApp = new CRadiantApp();
	MyApp2 *myApp = new MyApp2;
	//AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 10);
	AfxWinInit(hInstance, NULL, ::GetCommandLine(), 10);
	//CWinThread* pThread = AfxGetThread();
	//CWinApp* pApp = AfxGetApp();

	//HINSTANCE hInstance = NULL;
	//HINSTANCE hPrevInstance = NULL;
	//LPTSTR lpCmdLine = "";
	//int nCmdShow = 10;
	//AfxWinInit(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	//theApp = new CRadiantApp();
	//theApp->m_pszAppName = "radiant";
#if 0
	theApp->InitApplication();
	theApp->InitInstance();
	theApp->Run();
#else
	myApp->InitApplication();
	myApp->InitInstance();
	myApp->Run();
#endif

	AfxWinTerm();

    // Step 3: The Message Loop
#if 0
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
		theApp->OnIdle(1);
    }
#endif
   // return Msg.wParam;
}

HINSTANCE g_hOpenGL32 = NULL;
HINSTANCE g_hOpenGL = NULL;
bool g_bBuildList = false;

BOOL CRadiantApp::InitInstance() {
	// If there's a .INI file in the directory use it instead of registry
	char RadiantPath[_MAX_PATH];
	GetModuleFileName( NULL, RadiantPath, _MAX_PATH );

	// search for exe
	CFileFind Finder;
	Finder.FindFile( RadiantPath );
	Finder.FindNextFile();
	// extract root
	CString Root = Finder.GetRoot();
	// build root\*.ini
	CString IniPath = Root + "\\REGISTRY.INI";
	// search for ini file
	Finder.FindNextFile();
	if (Finder.FindFile( IniPath ))
	{
		Finder.FindNextFile();
		// use the .ini file instead of the registry
		free((void*)m_pszProfileName);
		m_pszProfileName=_tcsdup(_T(Finder.GetFilePath()));
		// look for the registry key for void* buffers storage ( these can't go into .INI files )
		int i=0;
		CString key;
		HKEY hkResult;
		DWORD dwDisp;
		DWORD type;
		char iBuf[3];
		do
		{
			sprintf( iBuf, "%d", i );
			key = "Software\\Q3Radiant\\IniPrefs" + CString(iBuf);
			// does this key exists ?
			if ( RegOpenKeyEx( HKEY_CURRENT_USER, key, 0, KEY_ALL_ACCESS, &hkResult ) != ERROR_SUCCESS )
			{
				// this key doesn't exist, so it's the one we'll use
				strcpy( g_qeglobals.use_ini_registry, key.GetBuffer(0) );
				RegCreateKeyEx( HKEY_CURRENT_USER, key, 0, NULL, 
					REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, &dwDisp );
				RegSetValueEx( hkResult, "RadiantName", 0, REG_SZ, reinterpret_cast<CONST BYTE *>(RadiantPath), strlen( RadiantPath )+1 );
				RegCloseKey( hkResult );
				break;
			}
			else
			{
				char RadiantAux[ _MAX_PATH ];
				unsigned long size = _MAX_PATH;
				// the key exists, is it the one we are looking for ?
				RegQueryValueEx( hkResult, "RadiantName", 0, &type, reinterpret_cast<BYTE *>(RadiantAux), &size );
				RegCloseKey( hkResult );
				if ( !strcmp( RadiantAux, RadiantPath ) )
				{
					// got it !
					strcpy( g_qeglobals.use_ini_registry, key.GetBuffer(0) );
					break;
				}
			}
			i++;
		} while (1);
		g_qeglobals.use_ini = true;
	}
	else
	{
		// Change the registry key under which our settings are stored.
		// You should modify this string to be something appropriate
		// such as the name of your company or organization.
		SetRegistryKey("Q3Radiant");
		g_qeglobals.use_ini = false;
	}

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	g_PrefsDlg.LoadPrefs();
	int nMenu = IDR_MENU1;
	CString strOpenGL = (g_PrefsDlg.m_bSGIOpenGL) ? "opengl.dll" : "opengl32.dll";
	CString strGLU = (g_PrefsDlg.m_bSGIOpenGL) ? "glu.dll" : "glu32.dll";
	if (!QGL_Init(strOpenGL, strGLU)) {
		g_PrefsDlg.m_bSGIOpenGL ^= 1;
		strOpenGL = (g_PrefsDlg.m_bSGIOpenGL) ? "opengl.dll" : "opengl32.dll";
		strGLU = (g_PrefsDlg.m_bSGIOpenGL) ? "glu.dll" : "glu32.dll";
		if (!QGL_Init(strOpenGL, strGLU)) {
			AfxMessageBox("Failed to load OpenGL libraries. \"OPENGL32.DLL\" and \"OPENGL.DLL\" were tried");
			return FALSE;
		}
		g_PrefsDlg.SavePrefs();
	}
	CString strTemp = m_lpCmdLine;
	strTemp.MakeLower();
	if (strTemp.Find("builddefs") >= 0)
		g_bBuildList = true;
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(nMenu))
		return FALSE;
	if (pMainFrame->m_hAccelTable)
		::DestroyAcceleratorTable(pMainFrame->m_hAccelTable);
	pMainFrame->LoadAccelTable(MAKEINTRESOURCE(IDR_MINIACCEL));
	m_pMainWnd = pMainFrame;
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	return TRUE;
}

int CRadiantApp::ExitInstance() {
	QGL_Shutdown();
	return CWinApp::ExitInstance();
}

BOOL CRadiantApp::OnIdle(LONG lCount) {
	if (g_pParentWnd)
		g_pParentWnd->RoutineProcessing();
	return CWinApp::OnIdle(lCount);
}

void CRadiantApp::OnHelp() {
}
