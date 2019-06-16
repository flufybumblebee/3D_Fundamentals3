#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "Graphics.h"
#include "Random.h"
#include "Rect.h"
#include "Mouse.h"
#include "Block.h"

enum TEXTURE
{
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	MINE,
	TILE,
	FLAG
};

class Minesweeper
{
private:
	static constexpr unsigned int SCREEN_W = Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H = Graphics::ScreenHeight;

	const unsigned int MINES;

	const unsigned int GRID_COLS;
	const unsigned int GRID_ROWS;
	const unsigned int GRID_SIZE;
	
	unsigned int GRID_W;
	unsigned int GRID_H;
	unsigned int BLOCK_W;	
	unsigned int BLOCK_H;

	std::vector<unsigned int>				block_values;
	std::vector<std::shared_ptr<Surface>>	block_textures;
	std::vector<Block>						blocks;
	std::vector<bool>						block_revealed;
	std::vector<bool>						block_flag;

	RectUI									grid_position;
	RectUI									display_position;

	std::array<std::array<Block, 3>, 10>	display_mines;
	std::array<std::array<Block, 5>, 10>	display_time;

	bool mouse_pressed	= false;
	bool gameover		= false;

public:
	Minesweeper(const unsigned int& mines, const unsigned int& columns, const unsigned int& rows);
	
public:
	void Update(Mouse& mouse);
	void Draw(Graphics& gfx);

private:
	inline void SetBlockValue(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void RevealBlocks(const int& X, const int& Y, const int& COLS, const int& ROWS);
	inline void RevealBlock(const int& X, const int& Y, const int& COLS, const int& ROWS);
};

