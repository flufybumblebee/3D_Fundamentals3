#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"

/*  NOTES:

pos		= position (& size) of texture (when drawn)
pTex	= pointer to a texture
rhs		= right hand side (of the assignment operator)  */

//class Block
//{
//private:
//	RectF pos;
//	Surface* pTex = nullptr;
//
//public:
//	Block() = default;
//	Block(const RectF& position, Surface* pTex);
//	Block(const Block& copy);
//	Block(Block&& block) noexcept;
//	Block& operator = (const Block& rhs);
//	Block& operator = (Block&& rhs) noexcept;
//	~Block() = default;
//public:
//	void SetPosition(const RectF& new_position);
//	void SetTexture(Surface* new_texture);
//	void Draw(Graphics& gfx);
//};

class Block
{
private:
	RectF pos;
	std::shared_ptr<Surface> pTex = nullptr;

public:
	Block() = default;
	Block(const RectUI& position, std::shared_ptr<Surface> pTex);
	Block(const Block& copy);
	Block(Block&& block) noexcept;
	Block& operator = (const Block& rhs);
	Block& operator = (Block&& rhs) noexcept;
	~Block() = default;
public:
	RectUI GetPosition() const;
	void SetPosition(const RectUI& new_position);
	void SetTexture(std::shared_ptr<Surface> new_texture);
	void Draw(Graphics& gfx);
};