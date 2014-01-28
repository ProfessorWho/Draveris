
#include "const.h"

void DrawFigure() //Draw next figure
{
	HDC dc = GetDC(g_hFigure);
	RECT	rectWndFigure;
	HBRUSH  hOldBrach;
	HBRUSH	hBrash = CreateSolidBrush(RGB(100, 100, 100));
	HBRUSH	hBlackBrash = GetStockBrush(BLACK_BRUSH);

	GetWindowRect(g_hFigure, &rectWndFigure);
	int x = (rectWndFigure.right - rectWndFigure.left) / 4;
	int y = (rectWndFigure.bottom - rectWndFigure.top) / 4;
	for (int i = 0; i<4; i++)
	for (int j = 0; j<4; j++)
	{
		if (g_bNextFigur[i][j])
		{
			hOldBrach = SelectBrush(dc, hBrash);
			Rectangle(dc, i*x, j*y, ((i + 1)*x) + x, (j*y) + y);
			hBrash = SelectBrush(dc, hOldBrach);
		}
		else
		{
			hOldBrach = SelectBrush(dc, hBlackBrash);
			Rectangle(dc, i*x, j*y, ((i + 1)*x) + x, (j*y) + y);
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

void drawBlockStationary(float _x, float _y, float _r, float _g, float _b)
{
	/* draw outside */
	cleanObject();
	glTranslatef(_x, _y, 20);
	glScalef(1, 1, 1);
	glColor3f(0.15f, 0.15f, 0.15f);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glutSolidCube(1.0);

	/* fill in the creamy centre */
	cleanObject();
	glTranslatef(_x, _y, 20);
	glScalef(1, 1, 1.2);
	glColor3f(_r, _g, _b);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glutSolidCube(.85);
}

int DrawGLScene(GLvoid)
{
	int i, j;
	GLUquadricObj *quadObj;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	cleanObject();

	for (i = 0; i < VERTICAL_LINE + 2; i++)
	{
		for (j = 0; j < HORIZONTAL_LINE + 2; j++)
		{
			if (background[i][j] != 0)
			{

				cleanObject();
				glTranslatef((j*-1) + 5.5, (i*-1) + 1, 20);
				glScalef(1, 1, 1);
				glColor4f(0.16, 0.16, 0.16, 1.0f);
				glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
				glEnable(GL_COLOR_MATERIAL);
				glutSolidCube(1);

			}
		}
	}

	for (i = 0; i < HORIZONTAL_LINE; i++)
	{
		for (j = 0; j < VERTICAL_LINE; j++)
		{
			if (g_iStok[i][j] > 30)
			{

				drawBlockStationary((i*-1) + 4.5, (j*-1), 1.0, 3.0, 0.2);
			}
		}
	}

	for (int i = 0; i < MAX_MEN; i++)
	{
		if (g_man[i] != NULL)
		{
			cleanObject();
			glPushMatrix();
			glTranslated(((float)g_man[i]->x / SIZE_RECT)*-1 + 4.5, -24.0, 20.0);
			glRotated(120, 1, 0, 0);
			glRotated(180, 0, 1, 0);
			quadObj = gluNewQuadric();
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			gluQuadricTexture(quadObj, GL_TRUE);
			gluQuadricDrawStyle(quadObj, GLU_FILL);
			glColor3d(0.3, 0.3, 0.3);
			glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
			glEnable(GL_COLOR_MATERIAL);
			gluSphere(quadObj, 0.45, 50, 50);
			glPopMatrix();
			gluDeleteQuadric(quadObj);
			auxSwapBuffers();
		}
	}

	return 0;
}

void cleanObject()
{
	glLoadIdentity();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);	
	glBindTexture(GL_TEXTURE_2D, NULL); 
	glTranslatef(0.0f, -1.0f, -2.0f);

	gluLookAt(
		0.0f, 1.5f, -17.0f,
		0.0f, -5.4f, 3.0f,
		0.0f, 1.0f, 0.0f);
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0)
	{
		height = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int InitGL(GLvoid)
{
	LoadGLTextures();
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	return 0;
}

GLvoid LoadGLTextures()
{
	AUX_RGBImageRec *texture1;
	texture1 = auxDIBImageLoad(L"svb.bmp");
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texture1->data);
}

void	Repaint()
{
	DrawGLScene();
	SwapBuffers(hDC);
	DrawFigure();
}
