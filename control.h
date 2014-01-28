#ifndef CNTRL_H
#define CNTRL_H

void OnKeyDown(HWND hwnd, UINT vk, char fDown, int cRepeat, UINT flags);

void OnTimer(HWND hwnd, UINT id);

void OnPaint(HWND hwnd);

void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);

char OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

void NewGame();

#endif // !CNTRL_H
