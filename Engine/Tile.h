#pragma once

#include "Block.h"
#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"
#include <assert.h>

class Tile
{
private:
	static constexpr size_t EMPTY			= 0;
	static constexpr size_t ONE				= 1;
	static constexpr size_t TWO				= 2;
	static constexpr size_t THREE			= 3;
	static constexpr size_t FOUR			= 4;
	static constexpr size_t FIVE			= 5;
	static constexpr size_t SIX				= 6;
	static constexpr size_t SEVEN			= 7;
	static constexpr size_t EIGHT			= 8;
	static constexpr size_t MINE			= 9;
	static constexpr size_t EXPLODED		= 10;
	static constexpr size_t FLAG			= 11;
	static constexpr size_t FLAG_WRONG		= 12;
	static constexpr size_t FLAG_CORRECT	= 13;
	static constexpr size_t MOUSEOVER		= 14;

	unsigned int value	= 0u;
	bool is_revealed	= false;
	bool is_flag		= false;
	bool is_mine		= false;
	bool is_checked		= false;
	bool is_flag_wrong	= false;
	bool is_exploded	= false;
	bool mouseover		= false;

	RectUI mouseover_rect;

	std::vector<Block> blocks;
	std::vector<std::shared_ptr<Surface>> textures;

public:
	Tile() = default;
	Tile(std::vector<Block> BLOCKS);

public:
	void SetValue(const unsigned int& VALUE);
	void SetRevealed(const bool& IS_REVEALED);
	void SetFlag(const bool& IS_FLAG);
	void SetChecked(const bool& IS_CHECKED);
	void SetExploded(const bool& IS_EXPLODED);
	void SetMouseOver(Mouse& mouse);

	void SetMouseoverRect(const RectUI& MOUSE_RECT);
	void SetRect(const size_t& INDEX, const RectUI& RECT);
	void SetTexture(const size_t& INDEX, std::shared_ptr<Surface> texture);

	unsigned int Value() const;
	bool Revealed() const;
	bool Flag() const;
	bool Exploded() const;
	bool Checked() const;
	bool Mine() const;
	bool MouseOver() const;

	RectUI MouseoverRect() const;
	RectUI Rect(const size_t INDEX) const;
public:
	void Reset();
	void Draw(Graphics& gfx);
};
