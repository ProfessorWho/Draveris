#include "const.h"

void NewGame()
{
	WCHAR	szScore[100];
	srand(GetTickCount() % 100);
	g_iScore = 0;
	ClearFigure(&g_bNextFigur[0][0]);
	g_iNextFigure = rand() % FIGUR_COUNT;
	GetFigure(g_iNextFigure);

	for (int i = 0; i<HORIZONTAL_LINE; i++)
	for (int j = 0; j<VERTICAL_LINE; j++)
		g_iStok[i][j] = 30;
	for (int i = 0; i < MAX_MEN; i++)
	{
		KillMan(g_man[i]);
		g_man[i] = NULL;
	}
	g_iScore = 0;
	wsprintf(szScore, TEXT("Draveris Score:%d"), g_iScore);
	SetWindowText(g_hwnd, szScore);

	NextFigure();
	Repaint();
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

char OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	g_hwnd = hwnd;
	g_hMenu = CreateMenu();
	HMENU	hMenuPopapFile = CreatePopupMenu();
	AppendMenu(hMenuPopapFile, MF_STRING, ID_MENU_NEW_GAME, TEXT("New game"));
	AppendMenu(hMenuPopapFile, MF_STRING, ID_MENU_EXIT, TEXT("Exit"));
	AppendMenu(g_hMenu, MF_POPUP, (UINT)hMenuPopapFile, TEXT("File"));
	SetMenu(hwnd, g_hMenu);
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
	DrawGLScene();
	SwapBuffers(hDC);
	DrawPicture(hwnd);
	DrawFigure();
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
			srand(time(NULL));
			isMan = rand() % 100;
			if (isMan > 70 && man_counter < MAX_MEN)
				CreateThread(NULL, 0, CreateManThread, NULL, 0, NULL);
			break;
		}
	}
}

void OnKeyDown(HWND hwnd, UINT vk, char fDown, int cRepeat, UINT flags)
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
		DrawGLScene();
		SwapBuffers(hDC);
		break;
	}
}
