
#ifndef DEF_GUARD

#include "Man.h"
#include <stdlib.h>
#include "GL.H"
#include "GLU.H"
#include "glut.h"
#include "GLAUX.H"
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include "resource.h"
#include <stdio.h>
#include "draw.h"
#include "logic.h"
#include "control.h"
#include "control_m.h"

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLu32.lib")
#pragma comment (lib, "glut32.lib")
#pragma comment (lib, "GLAUX.lib")

#define DEF_GUARD
#define	ID_MENU_NEW_GAME	WM_USER+1
#define	ID_MENU_EXIT		WM_USER+2

//-----------------------------------------------------------------------------------------
#define	HORIZONTAL_LINE		10  //Horizontal lines counter
#define	VERTICAL_LINE		25	//Vertical lines counter

#define	FIGUR_COUNT			7	//Figure counter

#define	SIZE_WINDOW_X		600 
#define	SIZE_WINDOW_Y		800	
#define	SIZE_WINDOW_STOK_X	300 //Size of glass
#define	SIZE_WINDOW_STOK_Y	700	
#define	SPEED				500 //speed of falling
#define MAX_MEN				5   //Max value of men
#define SIZE_RECT			30 //size of glass rectangle
#define TRUE				1
#define FALSE				0

extern HINSTANCE	g_hinstance;
extern HWND			g_hwnd, g_hStok, g_hFigure; //windows
extern HMENU		g_hMenu;
extern char			g_bFigure[4][4], g_bNextFigur[4][4]; //Massives for figures
extern int			g_iStok[HORIZONTAL_LINE][VERTICAL_LINE]; //Massive for glass
extern POINT		g_pPozFigure;			//Position of figure at moment
extern int			g_iColor, g_iFigure, g_iNextFigure, g_iScore; //color, kind, kind of next, score counter
extern enum			enumFigure{ line = 0, cube, lright, lleft, tfigur, zleft, zright };//kindes of figures
extern int			man_counter;
extern Man*			g_man[5]; //Massive of men
extern int			Timer_id[5]; //massive for linking men and threads
extern HBITMAP		hBitmap;
extern HGLRC		hRC;
extern HDC			hDC;
extern GLuint		texture[1];

extern int background[27][12];

extern GLfloat LightAmbient[];
extern GLfloat LightDiffuse[];
extern GLfloat LightPosition[];

#endif
