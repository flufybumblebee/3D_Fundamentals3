#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "TexVertex.h"
#include "Rect.h"
#include <memory>

/*

NOTES:

pos		= position & size of texture when drawn
TC		= texture coordinate
pTex	= pointer to a texture
rhs		= right hand side (of the assignment)

*/

class Block
{
private:
	RectF pos;
	Surface* pTex = nullptr;

public:
	Block() = default;
	Block(const RectF& position, Surface* pTex)
		:
		pos(position),
		pTex(pTex)
	{}
	Block(const Block& copy)
		:
		pos(copy.pos),
		pTex(copy.pTex)
	{}
	Block(Block&& block) noexcept
		:
		pos(block.pos),
		pTex(block.pTex)
	{
		block.pTex = nullptr;
	}
	Block& operator = (const Block& rhs)
	{
		pos = rhs.pos;
		pTex = rhs.pTex;

		return *this;
	}
	Block& operator = (Block&& rhs) noexcept
	{
		if (&rhs != this)
		{
			pos = rhs.pos;
			pTex = rhs.pTex;

			rhs.pTex = nullptr;
		}

		return *this;
	}
	~Block()
	{
		delete pTex;
		pTex = nullptr;
	}
public:
	void SetPosition(const RectF& new_position)
	{
		pos = new_position;
	}
	void SetTexture(Surface* new_texture)
	{
		pTex = new_texture;
	}
	void Draw(Graphics& gfx)
	{
		gfx.DrawTriangleTex(
			{ {pos.left,	pos.top,	0.0f},{ 0.0f,0.0f } },
			{ {pos.right,	pos.top,	0.0f},{ 1.0f,0.0f } },
			{ {pos.right,	pos.bottom,	0.0f},{ 1.0f,1.0f } },
			*pTex);
		gfx.DrawTriangleTex(
			{ {pos.left,	pos.top,	0.0f},{ 0.0f,0.0f } },
			{ {pos.right,	pos.bottom,	0.0f},{ 1.0f,1.0f } },
			{ {pos.left,	pos.bottom,	0.0f},{ 0.0f,1.0f } },
			*pTex);
	}
};