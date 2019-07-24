#pragma once

#include "Block.h"
#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"
#include <assert.h>

class Tile
{
private:
	unsigned int value	= 0u;
	bool is_revealed	= false;
	bool is_flag		= false;
	bool is_mine		= false;
	bool is_checked		= false;
	Block block;
public:
	Tile() = default;
	Tile(const RectUI& POSITION, std::shared_ptr<Surface> texture);
public:
	void SetValue(const unsigned int& VALUE);
	void SetIsRevealed(const bool& IS_REVEALED);
	void SetIsFlag(const bool& IS_FLAG);
	void SetIsChecked(const bool& IS_CHECKED);
	void SetPosition(const RectUI& POSITION);
	void SetTexture(std::shared_ptr<Surface> texture);
	void SetMouseOver(Mouse& mouse);

	unsigned int Value() const;
	bool Revealed() const;
	bool Flag() const;
	bool Checked() const;
	bool Mine() const;
	RectUI Position() const;
	bool MouseOver() const;
public:
	void Reset();
	void Draw(Graphics& gfx);
};
