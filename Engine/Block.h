#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "TexVertex.h"
#include "Rect.h"
#include <memory>

class Block
{
public:
	Block() = default;
	Block(const RectI& pos, Surface* pTex)
		:
		pos(std::move(pos)),
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
	void Draw(Graphics& gfx)
	{
		tl = { pos.left,pos.top,0.0f };
		tr = { pos.right,pos.top,0.0f };
		br = { pos.right,pos.bottom,0.0f };
		bl = { pos.left,pos.bottom,0.0f };
		TexVertex tv0 = TexVertex(tl, tc0);
		TexVertex tv1 = TexVertex(tr, tc1);
		TexVertex tv2 = TexVertex(br, tc2);
		TexVertex tv3 = TexVertex(bl, tc3);
		gfx.DrawTriangleTex(tv0, tv1, tv2, *pTex);
		gfx.DrawTriangleTex(tv0, tv2, tv3, *pTex);
	}
private:
	RectF pos;
	Vec3 tl;
	Vec3 tr;
	Vec3 br;
	Vec3 bl;
	const Vec2 tc0 = { 0.0f,0.0f };
	const Vec2 tc1 = { 1.0f,0.0f };
	const Vec2 tc2 = { 1.0f,1.0f };
	const Vec2 tc3 = { 0.0f,1.0f };
	//std::unique_ptr<Surface> pTex;
	Surface* pTex;
};
