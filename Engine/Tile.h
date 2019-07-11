#pragma once

#include "Block.h"
#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"

class Tile
{
private:
	unsigned int value	= 0u;
	bool is_revealed	= false;
	bool is_flag		= false;
	Block block;
public:
	Tile() = default;
	Tile(const RectUI& POSITION, std::shared_ptr<Surface> texture);
public:
	void SetValue(const unsigned int& VALUE);
	void SetIsRevealed(const bool& IS_REVEALED);
	void SetIsFlag(const bool& IS_FLAG);
	void SetPosition(const RectUI& POSITION);
	void SetTexture(std::shared_ptr<Surface> texture);
	void SetMouseOver(Mouse& mouse);

	unsigned int Value() const;
	bool IsRevealed() const;
	bool IsFlag() const;
	bool IsMouseOver() const;
	RectUI Position() const;
public:
	void Reset();
	void Draw(Graphics& gfx);
};

