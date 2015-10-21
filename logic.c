#include "const.h"

void Rotate()	//upheaval of figure
{
	char	tmpFigur[4][4], tmpfigur2[4][4], isPlace;
	int left = 0;
	int first[4] = { 0, 0, 0, 0 };
	int maxim = 0;
	int i, j, c;
	isPlace = FALSE;
	EraseOldFigur();
	switch (g_iFigure)
	{
		case cube:return;
	}

	for (i = 0; i<4; i++)
		for (j = 0; j < 4; j++)
			{
				tmpFigur[i][j] = FALSE;
				tmpfigur2[i][j] = FALSE;
			}

	for (j = 3, c = 0; j >= 0; j--, c++)
		for (i = 0; i< 4; i++)
			tmpFigur[c][i] = g_bFigure[i][j];

	for (i = 3; i >= 0; i--)
	{
		for (j = 0; j < 4; j++)
		{
			if (tmpFigur[i][j])
			{
				left = i; break;
			}
		}
	}

	//Move left figure, if there is empty place

	for (i = 0; i < 4 - left; i++)
	for (j = 0; j < 4; j++)
	{
		tmpfigur2[i][j] = tmpFigur[i + left][j];
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (tmpfigur2[i][j])
			{
				if (g_iStok[g_pPozFigure.x + i][g_pPozFigure.y + j] > 30)
					isPlace = TRUE;
			}
		}
	}

	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (tmpfigur2[i][j])
			{
				maxim = max(maxim, i);
			}
		}
	}
	if (g_pPozFigure.x + maxim >= HORIZONTAL_LINE)
	{
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < 4; i++)
			{
				if (tmpfigur2[i][j] == TRUE && first[j] == 0)first[j] = i + 1;
			}
		}
		for (i = 1; i < maxim + 1; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (g_iStok[g_pPozFigure.x + first[j] - 1 - i][g_pPozFigure.y + j] > 30)
					isPlace = TRUE;
			}
		}
	}

	if (!isPlace)
	{

		ClearFigure(&g_bFigure[0][0]);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				g_bFigure[i][j] = tmpfigur2[i][j];
			}
		}
		OverRight();
	}


}

void OverRight() //Move left figure, if it went off the screen
{
	int tms = 0;
	int maxim = 0;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			if (g_bFigure[j][i] == TRUE)tms = j;
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

void MoveDown()
{
	if (IsImpact())return;
	EraseOldFigur();
	if (g_pPozFigure.y<VERTICAL_LINE)g_pPozFigure.y++;
	PaintNewFigur();
	DrawGLScene();
	SwapBuffers(hDC);
}

void MoveLeft()
{
	char tms = FALSE;
	int first[4] = { 0, 0, 0, 0 };
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			if (g_bFigure[i][j] == TRUE && first[j] == 0)first[j] = i + 1;
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
	DrawGLScene();
	SwapBuffers(hDC);
}

void MoveRight()
{
	char tms = FALSE;
	int maxim[4] = { 0, 0, 0, 0 };
	int max_of_maxim = 0;
	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			if (g_bFigure[i][j] == TRUE)maxim[j] = i + 1;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		max_of_maxim = max(max_of_maxim, maxim[i]);
	}
	EraseOldFigur();
	for (int i = 0; i < 4; i++)
	{
		if (g_iStok[maxim[i] + g_pPozFigure.x][g_pPozFigure.y + i] > 30)
			tms = TRUE;
	}
	if (g_pPozFigure.x >= HORIZONTAL_LINE - max_of_maxim)
		tms = TRUE;
	if (!tms) g_pPozFigure.x++;
	PaintNewFigur();
	DrawGLScene();
	SwapBuffers(hDC);
}

void LineClear() //Check for full line
{
	char	bLineFill = FALSE;
	WCHAR	szScore[100];
	int i, j;
	do
	{
		for (i = 0; i<VERTICAL_LINE; i++)
		{
			for (j = 0; j<HORIZONTAL_LINE; j++)
			{
				if (g_iStok[j][i]>30)
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
			for (; i>0; i--)
			{
				for (j = 0; j<HORIZONTAL_LINE; j++)
				{
					g_iStok[j][i] = g_iStok[j][i - 1];
				}
			}
			g_iScore += 100;
			wsprintf(szScore, TEXT("Draveris Score:%d"), g_iScore);
			SetWindowText(g_hwnd, szScore);
		}
	} while (bLineFill);
}

void EraseOldFigur()
{
	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
			if (g_bFigure[i][j])
			{
				g_iStok[i + g_pPozFigure.x][j + g_pPozFigure.y] = 30;
			}
}

void PaintNewFigur()
{
	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
			if (g_bFigure[i][j])
			{
				g_iStok[i + g_pPozFigure.x][j + g_pPozFigure.y] = g_iColor;
			}
}

char IsImpact() //Check for bottom or other figure
{
	if (g_pPozFigure.y == 15)
	{
		int f = 1;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (g_bFigure[i][j])
			{
				if (j + g_pPozFigure.y + 1 == VERTICAL_LINE)// Дно
				{
					isPunch();
					return TRUE;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (g_bFigure[i][j] && !g_bFigure[i][j + 1])
			{
				if (g_iStok[i + g_pPozFigure.x][j + g_pPozFigure.y + 1] > 30) //Линия
					return TRUE;
			}
		}
	}

	return FALSE;
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
		for (int j = 0; j<2; j++)
			g_bNextFigur[c][j] = TRUE;
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

void isPunch() //check for hit
{
	WCHAR	szScore[100];
	int maxim = 0;
	int start, end, c;
	char isHit = 0;
	c = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (g_bFigure[i][j])
				maxim = max(maxim, j);
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
		switch (isHit)
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

void ClearFigure(char *iArray)
{
	for (int i = 0; i<4 * 4; i++)
		*(iArray + i) = FALSE;
}
//-----------------------------------------------------------------------------------------
void CopyFigure(char *iSrcArray, char *iDstArray)
{
	for (int i = 0; i<4 * 4; i++)
		*(iDstArray + i) = *(iSrcArray + i);
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
