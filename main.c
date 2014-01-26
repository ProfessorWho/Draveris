#define	WIN32_LEAN_AND_MEAN

#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include "resource.h"
#include <stdio.h>
#include "const.h"
#include "draw.h"

#pragma comment(linker,"-merge:.rdata=.text")
#pragma comment (lib, "winmm.lib")

//-----------------------------------------------------------------------------------------
HINSTANCE	g_hinstance;
HWND		g_hwnd, g_hStok, g_hFigure; //windows
HMENU		g_hMenu;
BOOL		g_bFigure[4][4], g_bNextFigur[4][4]; //Massives for figures
int		g_iStok[HORIZONTAL_LINE][VERTICAL_LINE]; //Massive for glass
POINT		g_pPozFigure;			//Position of figure at moment
int		g_iColor, g_iFigure, g_iNextFigure, g_iScore; //color, kind, kind of next, score counter
enum		enumFigure{ line = 0, cube, lright, lleft, tfigur, zleft, zright };//kindes of figures
int		man_counter;
Man*		g_man[5]; //Massive of men
int		Timer_id[5]; //massive for linking men and threads
HBITMAP		hBitmap;
//-----------------------------------------------------------------------------------------

void KillMan(Man* _man)
{
	free(_man);
	man_counter--;
}

void isPunch() //check for hit
{
	WCHAR	szScore[100];
	int maxim = 0;
	int start, end, c;
	char isHit = 0;
	c = 0;
	for (int c1 = 0; c1 < 4; c1++)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{
			if (g_bFigure[c1][c2])
				maxim = max(maxim, c2);
		}
	}
	start = g_pPozFigure.x * SIZE_RECT;
	while (!g_bFigure[c][maxim])
	{
		start += SIZE_RECT;
		c++;
	}
	end = start;
	while (g_bFigure[c][maxim])
	{
		end += SIZE_RECT;
		c++;
	}
	for (int i = 0; i < MAX_MEN; i++)
	{
		if (g_man[i] != NULL)
		{
			if ((g_man[i]->x <= end && g_man[i]->x >= start) || (g_man[i]->x + 20 >= start && g_man[i]->x + 20 <= end))
			{
				KillMan(g_man[i]);
				g_man[i] = NULL;
				g_iScore += 30;
				wsprintf(szScore, TEXT("Draveris Score:%d"), g_iScore);
				SetWindowText(g_hwnd, szScore);
				isHit++;
			}
		}
	}
	if (isHit)
	{
		isHit--;
			switch(isHit)
			{
			case 0:
				PlaySound(L"Draven1.wav", 0, SND_FILENAME | SND_ASYNC);
				break;
			case 1:
				PlaySound(L"Draven2.wav", 0, SND_FILENAME | SND_ASYNC);
				break;
			case 2:
				PlaySound(L"Draven3.wav", 0, SND_FILENAME | SND_ASYNC);
				break;
			case 3:
				PlaySound(L"Draven4.wav", 0, SND_FILENAME | SND_ASYNC);
				break;
			case 4:
				PlaySound(L"Draven5.wav", 0, SND_FILENAME | SND_ASYNC);
				break;
			default:
				break;
			}
	}
}


void ManControl(UINT_PTR idEvent) //Change of man's coordinate
{
	char isValue = 0;
	int i = 0;
	while (isValue == 0)
	{
		if (Timer_id[i] == (int)idEvent)
		{
			isValue = 1;
		}
		i++;
	}
	if (g_man[i - 1]->dir == 0)
	{
		g_man[i - 1]->x++;
		if (g_man[i - 1]->x == SIZE_WINDOW_STOK_X - 20 || (g_iStok[(g_man[i - 1]->x + 20) / SIZE_RECT][24] > 30 && (g_man[i - 1]->x + 20) % SIZE_RECT == 0))
			g_man[i - 1]->dir = 1;
	}
	else
	{
		g_man[i - 1]->x--;
		if (g_man[i - 1]->x == 0 || (g_iStok[(g_man[i - 1]->x / SIZE_RECT) - 1][24] > 30 && g_man[i - 1]->x % SIZE_RECT == 0))
			g_man[i - 1]->dir = 0;
	}
}

void CALLBACK Timer2Proc(HWND hwnd, UINT msg, UINT_PTR idEvent,  DWORD dwTime)
{
	ManControl(idEvent);
}


int CreateMan()
{
	Man* _man;
	char isValue = 0;
	int i = 0;
	_man = (Man*)malloc(sizeof(Man));
	srand(GetTickCount() % 100);
	_man->x = rand() % (SIZE_WINDOW_STOK_X - 20);
	if (g_iStok[_man->x / SIZE_RECT][24] > 30 || g_iStok[((_man->x + 20) / SIZE_RECT)][24] > 30)
	{
		
		if (_man->x < SIZE_WINDOW_STOK_X / 2)
		{
			_man->x = 0;
			while (g_iStok[_man->x / SIZE_RECT][24] > 30 || g_iStok[((_man->x + 20) / SIZE_RECT)][24] > 30)
				_man->x++;
			_man->x++;
		}
		else
		{
			_man->x = SIZE_WINDOW_STOK_X - 20;
			while (g_iStok[_man->x / SIZE_RECT][24] > 30 || g_iStok[((_man->x + 20) / SIZE_RECT)][24] > 30)
				_man->x--;
			_man->x--;
		}
	}
	_man->y = SIZE_WINDOW_STOK_Y - 20;
	srand((unsigned)time(NULL));
	_man->r = (rand() % 205) + 50;
	_man->g = (rand() % 205) + 50;
	_man->b = (rand() % 205) + 50;
	if (_man->x < SIZE_WINDOW_STOK_X / 2)
		_man->dir = 0;
	else
		_man->dir = 1;
	while (i < MAX_MEN && isValue == 0)
	{
		if (g_man[i] == NULL)
		{
			g_man[i] = _man;
			man_counter++;
			isValue = 1;
		}
		i++;
	}
	return i-1;
}

void ThreadMan()
{
	int man_id, speed;
	MSG _msg;
	man_id = CreateMan();
	srand((unsigned)time(NULL));
	speed = (rand() % 200);
	if (speed < 150)
		speed = 50;
	else if (speed >= 150)
		speed = 400;
	Timer_id[man_id] = SetTimer(NULL, 0, speed, (TIMERPROC)&Timer2Proc);
	while (GetMessage(&_msg, NULL, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
		if (g_man[man_id] == NULL)
			//KillTimer(NULL, Timer_id[man_id]);
			ExitThread(0);
	}
}

DWORD WINAPI CreateManThread(LPVOID lpParam)
{
	ThreadMan();
}

void GetFigure(int eFigure)
{
	switch (eFigure)
	{
	case line:
		for (int c = 0; c<4; c++)
		{
			g_bNextFigur[c][0] = TRUE;
		}
		break;
	case tfigur:
		for (int c = 0; c<3; c++)
			g_bNextFigur[c][0] = TRUE;
		g_bNextFigur[1][1] = TRUE;
		break;
	case cube:
		for (int c = 0; c<2; c++)
		for (int c2 = 0; c2<2; c2++)
			g_bNextFigur[c][c2] = TRUE;
		break;
	case lright:
		for (int c = 0; c<3; c++)
			g_bNextFigur[0][c] = TRUE;
		g_bNextFigur[1][0] = TRUE;
		break;
	case lleft:
		for (int c = 0; c<3; c++)
			g_bNextFigur[0][c] = TRUE;
		g_bNextFigur[1][2] = TRUE;
		break;
	case zleft:	g_bNextFigur[0][0] = TRUE;
		g_bNextFigur[1][0] = TRUE;
		g_bNextFigur[1][1] = TRUE;
		g_bNextFigur[2][1] = TRUE;
		break;
	case zright:
		g_bNextFigur[0][1] = TRUE;
		g_bNextFigur[1][0] = TRUE;
		g_bNextFigur[1][1] = TRUE;
		g_bNextFigur[2][0] = TRUE;
		break;
	}
	g_iColor = (rand() % 100) + 100; //Random gray color
}
//-----------------------------------------------------------------------------------------

void	Repaint()
{
	DrawStok();
	DrawFigure();
}
//-----------------------------------------------------------------------------------------
void ClearFigure(BOOL *iArray)
{
	for (int c1 = 0; c1<4 * 4; c1++)
		*(iArray + c1) = FALSE;
}
//-----------------------------------------------------------------------------------------
void CopyFigure(BOOL *iSrcArray, BOOL *iDstArray)
{
	for (int c1 = 0; c1<4 * 4; c1++)
		*(iDstArray + c1) = *(iSrcArray + c1);
}
//-----------------------------------------------------------------------------------------
void NextFigure()
{
	CopyFigure(&g_bNextFigur[0][0], &g_bFigure[0][0]);
	ClearFigure(&g_bNextFigur[0][0]);

	g_iFigure = g_iNextFigure;

	g_iNextFigure = rand() % FIGUR_COUNT;
	GetFigure(g_iNextFigure);

	g_pPozFigure.x = HORIZONTAL_LINE / 2;
	g_pPozFigure.y = 0;

	SetTimer(g_hwnd, 1, SPEED, NULL);
}
//-----------------------------------------------------------------------------------------
void NewGame()
{
	srand(GetTickCount() % 100);
	g_iScore = 0;
	ClearFigure(&g_bNextFigur[0][0]);
	g_iNextFigure = rand() % FIGUR_COUNT;
	GetFigure(g_iNextFigure);

	for (int c1 = 0; c1<HORIZONTAL_LINE; c1++)
	for (int c2 = 0; c2<VERTICAL_LINE; c2++)
		g_iStok[c1][c2] = 30;
	for (int i = 0; i < MAX_MEN; i++)
	{
		KillMan(g_man[i]);
		g_man[i] = NULL;
	}
	NextFigure();
	Repaint();
}

BOOL IsImpact() //Check for bottom or other figure
{
	if (g_pPozFigure.y == 15)
	{
		int f = 1;
	}
	for (int c1 = 0; c1 < 4; c1++)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{
			if (g_bFigure[c1][c2])
			{
				if (c2 + g_pPozFigure.y + 1 == VERTICAL_LINE)// Дно
				{
					isPunch();
					return TRUE;
				}
			}
		}
	}
	for (int c1 = 0; c1 < 4; c1++)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{
			if (g_bFigure[c1][c2] && !g_bFigure[c1][c2+1])
			{
				if (g_iStok[c1 + g_pPozFigure.x][c2 + g_pPozFigure.y + 1] > 30) //Линия
					return TRUE;
			}
		}
	}

	return FALSE;
}
//-----------------------------------------------------------------------------------------
void EraseOldFigur()
{
	for (int c1 = 0; c1<4; c1++)
	for (int c2 = 0; c2<4; c2++)
	if (g_bFigure[c1][c2])
	{
		g_iStok[c1 + g_pPozFigure.x][c2 + g_pPozFigure.y] = 30;
	}
}
//-----------------------------------------------------------------------------------------
void PaintNewFigur()
{
	for (int c1 = 0; c1<4; c1++)
	for (int c2 = 0; c2<4; c2++)
	if (g_bFigure[c1][c2])
	{
		g_iStok[c1 + g_pPozFigure.x][c2 + g_pPozFigure.y] = g_iColor;
	}
}

void LineClear() //Check for full line
{
	BOOL	bLineFill = FALSE;
	WCHAR	szScore[100];
	int c1, c2;
	do
	{
		for (c1 = 0; c1<VERTICAL_LINE; c1++)
		{
			for (c2 = 0; c2<HORIZONTAL_LINE; c2++)
			{
				if (g_iStok[c2][c1]>30)
				{
					bLineFill = TRUE;
				}
				else
				{
					bLineFill = FALSE;
					break;
				}
			}
			if (bLineFill)break;
			bLineFill = FALSE;
		}
		if (bLineFill)
		{
			for (; c1>0; c1--)
			{
				for (c2 = 0; c2<HORIZONTAL_LINE; c2++)
				{
					g_iStok[c2][c1] = g_iStok[c2][c1 - 1];
				}
			}
			g_iScore+=100;
			wsprintf(szScore, TEXT("Tetris Score:%d"), g_iScore);
			SetWindowText(g_hwnd, szScore);
		}
	} while (bLineFill);
}

void MoveRight()
{
	BOOL tms = FALSE;
	int maxim[4] = {0,0,0,0};
	int max_of_maxim = 0;
	for (int c1 = 0; c1<4; c1++)
	{
		for (int c2 = 0; c2<4; c2++)
		{
			if (g_bFigure[c1][c2] == TRUE)maxim[c2] = c1 + 1;
		}
	}
	for (int c = 0; c < 4; c++)
	{
		max_of_maxim = max(max_of_maxim, maxim[c]);
	}
	EraseOldFigur();
	for (int c = 0; c < 4; c++)
	{
		if (g_iStok[maxim[c] + g_pPozFigure.x][g_pPozFigure.y + c] > 30)
			tms = TRUE;
	}
	if (g_pPozFigure.x >= HORIZONTAL_LINE - max_of_maxim)
		tms = TRUE;
	if (!tms) g_pPozFigure.x++;
	PaintNewFigur();
	DrawStok();
}
//-----------------------------------------------------------------------------------------
void MoveLeft()
{
	BOOL tms = FALSE;
	int first[4] = { 0, 0, 0, 0 };
	for (int c1 = 0; c1<4; c1++)
	{
		for (int c2 = 0; c2<4; c2++)
		{
			if (g_bFigure[c1][c2] == TRUE && first[c2] == 0)first[c2] = c1 + 1;
		}
	}
	for (int c = 0; c < 4; c++)
	{
		if (g_iStok[first[c] + g_pPozFigure.x - 2][g_pPozFigure.y + c] > 30 && first[c] != 0)
			tms = TRUE;
	}
	if (g_pPozFigure.x == 0)
		tms = TRUE;
	EraseOldFigur();
	if (!tms)g_pPozFigure.x--;
	PaintNewFigur();
	DrawStok();
}
//-----------------------------------------------------------------------------------------
void MoveDown()
{
	if (IsImpact())return;
	EraseOldFigur();
	if (g_pPozFigure.y<VERTICAL_LINE)g_pPozFigure.y++;
	PaintNewFigur();
	DrawStok();
}
//-----------------------------------------------------------------------------------------
void OverRight() //Move left figure, if it went off the screen
{
	int tms = 0;
	int maxim = 0;
	for (int c1 = 0; c1<4; c1++)
	{
		for (int c2 = 0; c2<4; c2++)
		{
			if (g_bFigure[c2][c1] == TRUE)tms = c2;
		}
		maxim = max(maxim, tms);
		tms = 0;
	}
	if ((g_pPozFigure.x + maxim) >= HORIZONTAL_LINE)
	{
		int x = HORIZONTAL_LINE - g_pPozFigure.x;
		maxim++;
		maxim -= x;
		g_pPozFigure.x -= maxim;
	}
}

void Rotate()	//upheaval of figure
{
	BOOL	tmpFigur[4][4], tmpfigur2[4][4], isPlace;
	int left = 0;
	int first[4] = { 0, 0, 0, 0 };
	isPlace = FALSE;
	EraseOldFigur();
	switch (g_iFigure)
	{
	
	case cube:return;
	}

	for (int c1 = 0; c1<4; c1++)
	for (int c2 = 0; c2 < 4; c2++)
	{
		tmpFigur[c1][c2] = FALSE;
		tmpfigur2[c1][c2] = FALSE;
	}
	for (int j = 3, c = 0; j >= 0; j--, c++)
		for (int i = 0; i< 4; i++)
			tmpFigur[c][i] = g_bFigure[i][j];

	for (int c1 = 3; c1>=0; c1--)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{

			if (tmpFigur[c1][c2])
			{
				left = c1; break;
			}
		}
	}
	
	//Move left figure, if there is empty place

		for (int f = 0; f < 4 - left; f++)
		for (int d = 0; d < 4; d++)
		{
			tmpfigur2[f][d] = tmpFigur[f + left][d];
		}

	for (int c1 = 0; c1 < 4; c1++)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{
			if (tmpfigur2[c1][c2])
			{
				if (g_iStok[g_pPozFigure.x + c1][g_pPozFigure.y + c2] > 30)
					isPlace = TRUE;
			}
		}
	}

	int maxim = 0;
	for (int c1 = 0; c1 < 4; c1++)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{
			if (tmpfigur2[c1][c2])
			{
				maxim = max(maxim, c1);
			}
		}
	}
	if (g_pPozFigure.x + maxim >= HORIZONTAL_LINE)
	{
		for (int c2 = 0; c2 < 4; c2++)
		{
			for (int c1 = 0; c1 < 4; c1++)
			{
				if (tmpfigur2[c1][c2] == TRUE && first[c2] == 0)first[c2] = c1 + 1;
			}
		}
		for (int i = 1; i < maxim + 1; i++)
		{
			for (int c2 = 0; c2 < 4; c2++)
			{
				if (g_iStok[g_pPozFigure.x + first[c2] - 1 - i][g_pPozFigure.y + c2] > 30)
					isPlace = TRUE;
			}
		}
	}

	if (!isPlace)
	{
		
		ClearFigure(&g_bFigure[0][0]);
		for (int c1 = 0; c1 < 4; c1++)
		{
			for (int c2 = 0; c2 < 4; c2++)
			{
				g_bFigure[c1][c2] = tmpfigur2[c1][c2];
			}
		}
		OverRight();
	}

	
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_MENU_NEW_GAME:
	{
							 NewGame();
							 break;
	}
	case ID_MENU_EXIT:
	{
						 ExitProcess(TRUE);
						 break;
	}
	}
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	g_hwnd = hwnd;
	g_hMenu = CreateMenu();
	HMENU	hMenuPopapFile = CreatePopupMenu();
	AppendMenu(hMenuPopapFile, MF_STRING, ID_MENU_NEW_GAME, TEXT("New game"));
	AppendMenu(hMenuPopapFile, MF_STRING, ID_MENU_EXIT, TEXT("Exit"));
	AppendMenu(g_hMenu, MF_POPUP, (UINT)hMenuPopapFile, TEXT("File"));
	SetMenu(hwnd, g_hMenu);

	//hBitmap = (HBITMAP)LoadImage(g_hinstance, MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hBitmap = (HBITMAP)LoadImage(g_hinstance, L"draven.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	return TRUE;
}


void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo)
{
	lpMinMaxInfo->ptMaxTrackSize.x = SIZE_WINDOW_X;
	lpMinMaxInfo->ptMaxTrackSize.y = SIZE_WINDOW_Y;
	lpMinMaxInfo->ptMinTrackSize.x = SIZE_WINDOW_X;
	lpMinMaxInfo->ptMinTrackSize.y = SIZE_WINDOW_Y;
}
//-----------------------------------------------------------------------------------------
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	DrawPicture(hwnd);
	BeginPaint(hwnd, &ps);
	DrawStok();
	DrawFigure();
	EndPaint(hwnd, &ps);
}

void OnTimer(HWND hwnd, UINT id)
{
	int isMan;
	if (IsImpact())
	{
		KillTimer(g_hwnd, 1);
		LineClear();
		NextFigure();
		PaintNewFigur();
		if (IsImpact())
		{
			KillTimer(g_hwnd, 1);
			MessageBox(NULL, TEXT("Game Over."), TEXT("Exit"), MB_OK);
		}
		else Repaint();
		return;
	}
	EraseOldFigur();
	if (g_pPozFigure.y<VERTICAL_LINE)g_pPozFigure.y++;
	PaintNewFigur();

	for (int i = 0; i< HORIZONTAL_LINE; i++)
	{
		if (g_iStok[i][24] == 30)
		{
			isMan = rand() % 100;
			if (isMan > 70 && man_counter < MAX_MEN)
				CreateThread(NULL, 0, CreateManThread, NULL, 0, NULL);
			break;
		}
	}
}

void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	switch (vk)
	{
	case VK_RIGHT:
		MoveRight();
		break;
	case VK_LEFT:
		MoveLeft();
		break;
	case VK_DOWN:
		MoveDown();
		break;
	case VK_UP:
		Rotate();
		PaintNewFigur();
		DrawStok();
		break;
	}
}

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
							DrawStok();
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

int WINAPI WinMain(HINSTANCE h, HINSTANCE hi, LPSTR l, int i)
{
	HINSTANCE hInst = GetModuleHandle(NULL);
	g_hinstance = hInst;
	TCHAR szClassName[] = TEXT("MainWindows");
	TCHAR szTitle[] = TEXT("Draveris Score:0");

	
	MSG Msg;
	WNDCLASS wc;
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
	
	g_hStok = CreateWindow(TEXT("WndStok"), szTitle, WS_VISIBLE | WS_BORDER | WS_CHILD,
		20, 20, 300, 700, g_hwnd, NULL, hInst, NULL);

	g_hFigure = CreateWindow(TEXT("WndFigure"), szTitle, WS_VISIBLE | WS_BORDER | WS_CHILD,
		SIZE_WINDOW_X - 200, 30, 100, 100, g_hwnd, NULL, hInst, NULL);

	ShowWindow(g_hwnd, SW_SHOWNORMAL);

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
