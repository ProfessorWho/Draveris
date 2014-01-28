#define	WIN32_LEAN_AND_MEAN

#include "const.h"

#pragma comment(linker,"-merge:.rdata=.text")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_GETMINMAXINFO, OnGetMinMaxInfo);
	case	WM_TIMER:
	{
						switch (wParam)
						{
						case 1:
							OnTimer(hwnd, msg, wParam, lParam);
							return 0;
						case 2:
							DrawGLScene();
							SwapBuffers(hDC);
							return 0;
						}
	}
		HANDLE_MSG(hwnd, WM_KEYDOWN, OnKeyDown);
	default: return(DefWindowProc(hwnd, msg, wParam, lParam));
	}
}

LRESULT CALLBACK WndStokProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_NCPAINT:
		return 0;
	case WM_ERASEBKGND:
		return 0;
	case WM_TIMER:
		return 0;
	default: return(DefWindowProc(hwnd, msg, wParam, lParam));
	}
}

GLvoid KillGLWindow(GLvoid)            
{
	if (hRC)             
	{
		if (!wglMakeCurrent(NULL, NULL))      
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		if (!wglDeleteContext(hRC))        
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;             
	}
	if (hDC && !ReleaseDC(g_hStok, hDC))
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;  
	}
	if (g_hStok && !DestroyWindow(g_hStok))
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		g_hStok = NULL;
	}
	if (!UnregisterClass("OpenGL", g_hinstance))  
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		g_hinstance = NULL;      
	}
}

char CreateGLWnd()
{
	GLuint    PixelFormat;
	static  PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if (!(hDC = GetDC(g_hStok)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC)))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC))
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	InitGL();
	return TRUE;
}

int WINAPI WinMain(HINSTANCE h, HINSTANCE hi, LPSTR l, int i)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	TCHAR szClassName[] = TEXT("MainWindows");
	TCHAR szTitle[] = TEXT("Draveris Score:0");
	DWORD    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;      
	DWORD    dwStyle = WS_CHILD;
	MSG	Msg;
	WNDCLASS wc;

	g_hinstance = hInst;
	hRC = NULL;
	hDC = NULL;
	ZeroMemory(&wc, sizeof(wc));
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(240, 240, 240));
	wc.lpszClassName = szClassName;
	RegisterClass(&wc);
	wc.lpszClassName = TEXT("WndFigure");
	wc.lpfnWndProc = WndStokProc;
	RegisterClass(&wc);
	wc.lpszClassName = TEXT("WndStok");
	RegisterClass(&wc);

	g_hwnd = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW,
		0, 0, SIZE_WINDOW_X, SIZE_WINDOW_Y, HWND_DESKTOP, NULL, hInst, NULL);

	g_hStok = CreateWindow(TEXT("WndStok"), szTitle, WS_VISIBLE | WS_CHILD, 20, 20, SIZE_WINDOW_STOK_X, SIZE_WINDOW_STOK_Y, g_hwnd, NULL, hInst, NULL);

	if (!CreateGLWnd())
		return 0;

	g_hFigure = CreateWindow(TEXT("WndFigure"), szTitle, WS_VISIBLE | WS_BORDER | WS_CHILD,
		SIZE_WINDOW_X - 200, 30, 100, 100, g_hwnd, NULL, hInst, NULL);

	ShowWindow(g_hwnd, SW_SHOWNORMAL);
	SetForegroundWindow(g_hStok);  
	ReSizeGLScene(SIZE_WINDOW_STOK_X, SIZE_WINDOW_STOK_Y);

	PlaySound(L"Draven.wav", 0, SND_FILENAME | SND_ASYNC);
	SetTimer(g_hwnd, 2, 50, NULL);

	while (GetMessage(&Msg, NULL, 0, 0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	ExitProcess(0);
	//return 0;
}
