#pragma once

#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"
#include "Mouse.h"

/*  NOTES:

position	= position (& size) of texture (when drawn)
pTex		= pointer to a texture
rhs			= right hand side (of the assignment operator)  */

class Block
{
private:
	bool mouse_over = false;
	RectF position;
	std::shared_ptr<Surface> pTex = nullptr;

public:
	Block() = default;
	Block(const RectUI& POSITION, std::shared_ptr<Surface> texture);
	Block(const Block& COPY);
	Block(Block&& block) noexcept;
	Block& operator = (const Block& rhs);
	Block& operator = (Block&& rhs) noexcept;
public:
	bool	MouseOver() const;
	RectUI	Position() const;
	void SetMouseOver(Mouse& mouse); 
	void SetPosition(const RectUI& POSITION);
	void SetTexture(std::shared_ptr<Surface> texture);
	void Draw(Graphics& gfx);
};