#include <Windows.h>
#include <Windowsx.h>
#include "const.h"

extern		HINSTANCE	g_hinstance;
extern		HWND		g_hwnd, g_hStok, g_hFigure; //windows
extern      BOOL		g_bNextFigur[4][4]; //Massives for figures
extern		int			g_iStok[HORIZONTAL_LINE][VERTICAL_LINE]; //Massive for glass
extern		Man*		g_man[5]; //Massive of men
extern		HBITMAP		hBitmap;

void DrawStok() //Draw glass
{
	HDC dc = GetDC(g_hStok);
	RECT	rectWndStok;
	GetWindowRect(g_hStok, &rectWndStok);
	int x = (rectWndStok.right - rectWndStok.left) / HORIZONTAL_LINE;
	int y = (rectWndStok.bottom - rectWndStok.top) / VERTICAL_LINE;
	int r;
	for (int c1 = 0; c1<HORIZONTAL_LINE; c1++)
	for (int c2 = 0; c2<VERTICAL_LINE; c2++)
	{
		r = g_iStok[c1][c2];
		HBRUSH hBrash = CreateSolidBrush(RGB(r, r, r));
		HBRUSH  hOldBrach = SelectBrush(dc, hBrash);
		Rectangle(dc, c1*x, c2*y, (c1*x) + x, (c2*y) + y);
		DeleteBrush(SelectBrush(dc, hOldBrach));
	}
	for (int i = 0; i < MAX_MEN; i++)
	{
		if (g_man[i] != NULL)
		{
			HBRUSH  hBrash = CreateSolidBrush(RGB(g_man[i]->r, g_man[i]->g, g_man[i]->b));
			HBRUSH  hOldBrach = SelectBrush(dc, hBrash);
			Rectangle(dc, g_man[i]->x, g_man[i]->y, g_man[i]->x + 20, g_man[i]->y + 20);
			DeleteBrush(SelectBrush(dc, hOldBrach));
		}
	}
	ReleaseDC(g_hFigure, dc);
	ValidateRect(g_hStok, &rectWndStok);
}

void DrawFigure() //Draw next figure
{
	HDC dc = GetDC(g_hFigure);
	RECT	rectWndFigure;
	GetWindowRect(g_hFigure, &rectWndFigure);
	HBRUSH  hOldBrach;
	HBRUSH	hBrash = CreateSolidBrush(RGB(100, 100, 100));
	HBRUSH	hBlackBrash = GetStockBrush(BLACK_BRUSH);
	int x = (rectWndFigure.right - rectWndFigure.left) / 4;
	int y = (rectWndFigure.bottom - rectWndFigure.top) / 4;
	for (int c1 = 0; c1<4; c1++)
	for (int c2 = 0; c2<4; c2++)
	{
		if (g_bNextFigur[c1][c2])
		{
			hOldBrach = SelectBrush(dc, hBrash);
			Rectangle(dc, c1*x, c2*y, ((c1 + 1)*x) + x, (c2*y) + y);
			hBrash = SelectBrush(dc, hOldBrach);
		}
		else
		{
			hOldBrach = SelectBrush(dc, hBlackBrash);
			Rectangle(dc, c1*x, c2*y, ((c1 + 1)*x) + x, (c2*y) + y);
			SelectBrush(dc, hOldBrach);
		}
	}
	DeleteBrush(SelectBrush(dc, hOldBrach));
	ReleaseDC(g_hFigure, dc);
	ValidateRect(g_hFigure, &rectWndFigure);
}

void DrawPicture(HWND hwnd)
{
	PAINTSTRUCT		ps;
	HDC 			hdc;
	BITMAP 			bitmap;
	HDC 			hdcMem;
	HGDIOBJ 		oldBitmap;
	hdc = BeginPaint(hwnd, &ps);
	hdcMem = CreateCompatibleDC(hdc);
	oldBitmap = SelectObject(hdcMem, hBitmap);
	GetObject(hBitmap, sizeof(bitmap), (LPSTR)&bitmap);
	BitBlt(hdc, 350, 350, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
	EndPaint(hwnd, &ps);
}
