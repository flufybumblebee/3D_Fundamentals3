#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "TexVertex.h"
#include "Rect.h"
#include <memory>

class Block
{
private:
	RectF pos;
	const Vec2 tc0 = { 0.0f,0.0f };
	const Vec2 tc1 = { 1.0f,0.0f };
	const Vec2 tc2 = { 1.0f,1.0f };
	const Vec2 tc3 = { 0.0f,1.0f };
	Surface* pTex = nullptr;

public:
	Block() = default;
	Block(const RectI& position, Surface* pTex)
		:
		pos(std::move(position)),
		pTex(std::move(pTex))
	{}
	Block(Block& copy)
		:
		pos(std::move(copy.pos)),
		pTex(std::move(copy.pTex))
	{}
	Block& operator = (Block& rhs)
	{
		pos = std::move(rhs.pos);
		pTex = std::move(rhs.pTex);

		return *this;
	}
	
public:
	void SetPosition(RectI new_position)
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
			{ {pos.left,	pos.top,	0.0f},tc0 },
			{ {pos.right,	pos.top,	0.0f},tc1 },
			{ {pos.right,	pos.bottom,	0.0f},tc2 },
			*pTex);
		gfx.DrawTriangleTex(
			{ {pos.left,	pos.top,	0.0f},tc0 },
			{ {pos.right,	pos.bottom,	0.0f},tc2 },
			{ {pos.left,	pos.bottom,	0.0f},tc3 },
			*pTex);
	}
};