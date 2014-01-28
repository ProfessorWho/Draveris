#include "const.h"

void KillMan(Man* _man)
{
	free(_man);
	man_counter--;
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

void CALLBACK Timer2Proc(HWND hwnd, UINT msg, UINT_PTR idEvent, DWORD dwTime)
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
	return i - 1;
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
	return 0;
}
