#ifndef CNTRLM_H
#define CNTRLM_H
DWORD WINAPI CreateManThread(LPVOID lpParam);

void ThreadMan();

int CreateMan();

void ManControl(UINT_PTR idEvent);

void KillMan(Man* _man);
#endif
