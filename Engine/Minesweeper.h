#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "Graphics.h"
#include "Random.h"
#include "Rect.h"
#include "Mouse.h"
#include "Block.h"

class Minesweeper
{
private:
	static constexpr unsigned int SCREEN_W = Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H = Graphics::ScreenHeight;

	const unsigned int MINE_NUM;

	const unsigned int GRID_COLS;
	const unsigned int GRID_ROWS;
	const unsigned int GRID_SIZE;
	
	unsigned int BLOCK_W;	
	unsigned int BLOCK_H;

	std::vector<unsigned int> block_value;
	std::vector<Block> blocks;
	std::vector<std::shared_ptr<Surface>> block_value_textures;
	std::vector<std::shared_ptr<Surface>> block_user_textures;
public:
	Minesweeper(const unsigned int& mines, const unsigned int& columns, const unsigned int& rows);
	void Update(Mouse& mouse);
public:
	void Draw(Graphics& gfx);
};

