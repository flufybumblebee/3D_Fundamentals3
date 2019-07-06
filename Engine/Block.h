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
	Block(const RectUI& position, std::shared_ptr<Surface> pTex);
	Block(const Block& copy);
	Block(Block&& block) noexcept;
	Block& operator = (const Block& rhs);
	Block& operator = (Block&& rhs) noexcept;
	~Block() = default;
public:
	void SetMouseOver(Mouse& mouse);
	bool GetMouseOver() const;
	RectUI GetPosition() const;
	void SetPosition(const RectUI& new_position);
	void SetTexture(std::shared_ptr<Surface> new_texture);
	void Draw(Graphics& gfx);
};