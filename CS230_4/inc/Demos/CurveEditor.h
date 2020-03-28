#ifndef CS230_CURVE_EDITOR_H_
#define CS230_CURVE_EDITOR_H_
// ----------------------------------------------------------------------------
#include "..\Engine\Animation\Path2D.h"

extern u32 gPickedPoint;
void DrawPath(const Path2D & path);
void DrawPointInfo(const Path2D & path, u32 kf_idx);
void DrawAllPointInfo(const Path2D & path);
void SavePathToFile(const Path2D & path);
void LoadPathFromFile(Path2D & path);
void EditPath(Path2D & path);
// ----------------------------------------------------------------------------
#endif