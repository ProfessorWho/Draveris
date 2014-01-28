#ifndef DRAW_H
#define DRAW_H

int DrawGLScene(GLvoid);

void DrawFigure();

void DrawPicture(HWND hwnd);

void drawBlockStationary(float _x, float _y, float _r, float _g, float _b);

void cleanObject();

GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

int InitGL(GLvoid);

GLvoid LoadGLTextures();

void	Repaint();
#endif
