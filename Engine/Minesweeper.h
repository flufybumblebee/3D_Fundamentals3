#pragma once

#include <array>
#include <vector>
#include <cassert>

#include "Graphics.h"
#include "Random.h"
#include "Rect.h"
#include "Mouse.h"
#include "Block.h"

namespace GRID
{
	static constexpr size_t EMPTY	= 0;
	static constexpr size_t ONE		= 1;
	static constexpr size_t TWO		= 2;
	static constexpr size_t THREE	= 3;
	static constexpr size_t FOUR	= 4;
	static constexpr size_t FIVE	= 5;
	static constexpr size_t SIX		= 6;
	static constexpr size_t SEVEN	= 7;
	static constexpr size_t EIGHT	= 8;
	static constexpr size_t MINE	= 9;
	static constexpr size_t TILE	= 10;
	static constexpr size_t FLAG	= 11;
};

namespace DISPLAY
{
	static constexpr size_t EMPTY	= 0;
	static constexpr size_t ONE		= 1;
	static constexpr size_t TWO		= 2;
	static constexpr size_t THREE	= 3;
	static constexpr size_t FOUR	= 4;
	static constexpr size_t FIVE	= 5;
	static constexpr size_t SIX		= 6;
	static constexpr size_t SEVEN	= 7;
	static constexpr size_t EIGHT	= 8;
	static constexpr size_t NINE	= 9;
	static constexpr size_t TILE	= 10;
};

namespace BORDER
{
	static constexpr size_t HORIZONTAL			= 0;
	static constexpr size_t VERTICLE			= 1;
	static constexpr size_t CORNER_TOP_LEFT		= 2;
	static constexpr size_t CORNER_TOP_RIGHT	= 3;
	static constexpr size_t CORNER_BOTTOM_LEFT	= 4;
	static constexpr size_t CORNER_BOTTOM_RIGHT = 5;
	static constexpr size_t T_LEFT				= 6;
	static constexpr size_t T_RIGHT				= 7;
};

class Minesweeper
{
private:
	static constexpr unsigned int SCREEN_W		= Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H		= Graphics::ScreenHeight;
	static constexpr unsigned int BLOCK_SIZE	= 33u;
	static constexpr unsigned int OFFSET		= 10u;

	const unsigned int MINES;

	const unsigned int GRID_COLS;
	const unsigned int GRID_ROWS;
	const unsigned int GRID_SIZE;
	
	unsigned int GRID_W;
	unsigned int GRID_H;

	std::vector<std::shared_ptr<Surface>>	border_textures;
	std::vector<Block>						border_blocks;
	RectUI									border_position;

	std::array<std::array<Block, 3>, 10>	display_mines;
	std::array<std::array<Block, 3>, 10>	display_timer;
	std::vector<std::shared_ptr<Surface>>	display_textures;
	RectUI									display_position;
	std::vector<unsigned int>				mines_number;

	RectUI									grid_position;

	std::vector<unsigned int>				block_values;
	std::vector<std::shared_ptr<Surface>>	block_textures;
	std::vector<Block>						blocks;
	std::vector<bool>						block_revealed;
	std::vector<bool>						block_flag;

	unsigned int mines = 0u;
	unsigned int flags = 0u;

	bool mouse_pressed	= false;
	bool gameover		= false;

public:
	Minesweeper(const unsigned int& mines, const unsigned int& columns, const unsigned int& rows);
	
public:
	void Update(Mouse& mouse);
	void Draw(Graphics& gfx);

private:
	void InitialiseTextures();
	void InitialiseBorder();
	void InitialiseBlockValues();
	void InitialiseBlocks();
	void InitialiseDisplayMines();
	void InitialiseDisplayTimer();

	void SetBlockValue(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void RevealBlocks(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void RevealBlock(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void ExtractDigits(std::vector<unsigned int>& ints, const unsigned int& NUM);

	void DrawDisplay(Graphics& gfx);
	void DrawGrid(Graphics& gfx);
	void DrawBlocks(Graphics& gfx);
	void DrawBorder(Graphics& gfx);
	void DrawMouseOverBlocks(Graphics& gfx);
	void DrawDisplayMines(Graphics& gfx);
};

