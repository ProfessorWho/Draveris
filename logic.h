#ifndef LOG_H
#define LOG_H
void Rotate();

void isPunch();

void GetFigure(int eFigure);

char IsImpact();

void PaintNewFigur();

void EraseOldFigur();

void LineClear();

void MoveRight();

void MoveLeft();

void MoveDown();

void OverRight();

void ClearFigure(char *iArray);

void CopyFigure(char *iSrcArray, char *iDstArray);

void NextFigure();
#endif // !LOG_H
