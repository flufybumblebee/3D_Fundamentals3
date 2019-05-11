#pragma once

#include "Surface.h"
#include "TexVertex.h"
#include "Rect.h"
#include "Graphics.h"

class Cell
{
private:
	TexVertex top_left;
	TexVertex top_right;
	TexVertex bottom_right;
	TexVertex bottom_left;

	Surface* pSurf = nullptr;

public:
	Cell(RectF pos, Surface* pSurf = nullptr)
		:
		top_left({ pos.left,	pos.top,	0.0f }, { 0.0f,0.0f }),
		top_right({ pos.right,pos.top,	0.0f }, { 1.0f,0.0f }),
		bottom_right({ pos.right,pos.bottom,	0.0f }, { 1.0f,1.0f }),
		bottom_left({ pos.left,	pos.bottom,	0.0f }, { 0.0f,1.0f }),
		pSurf(pSurf)
	{}
	void SetTexture(Surface* surf)
	{
		pSurf = surf;
	}
	void Draw(Graphics& gfx)
	{
		if (pSurf != nullptr)
		{
			gfx.DrawTriangleTex(top_left, top_right, bottom_right, *pSurf);
			gfx.DrawTriangleTex(top_left, bottom_right, bottom_left, *pSurf);
		}
	}
};
