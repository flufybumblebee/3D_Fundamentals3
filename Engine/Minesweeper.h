#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <chrono>

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
	static constexpr unsigned int DISPLAY_COLS	= 3u;
	static constexpr unsigned int DISPLAY_ROWS	= 10u;

	const unsigned int MINES;

	const unsigned int GRID_COLS;
	const unsigned int GRID_ROWS;
	const unsigned int GRID_SIZE;
	
	bool timer_started = false;
	std::chrono::high_resolution_clock::time_point t1;

	std::vector<std::shared_ptr<Surface>>	border_textures;
	std::vector<Block>						border_blocks;
	RectUI									border_position;

	std::array<std::array<Block, DISPLAY_COLS>, DISPLAY_ROWS>	mines_counter;
	std::vector<unsigned int>									mines_number;
	unsigned int												mines = 0u;
	std::array<std::array<Block, DISPLAY_COLS>, DISPLAY_ROWS>	timer;
	std::vector<unsigned int>									timer_number;
	unsigned int												time = 0;
	std::vector<std::shared_ptr<Surface>>						display_digit_textures;
	RectUI														display_background_position;
	RectUI														reset_button_position;
	std::array<Block, 2u>										reset_button_blocks;
	bool reset_is_pressed = false;

	RectUI									grid_position;

	std::vector<unsigned int>				block_values;
	std::vector<std::shared_ptr<Surface>>	block_textures;
	std::vector<Block>						blocks;
	std::vector<bool>						block_revealed;
	std::vector<bool>						block_flag;

	unsigned int flags = 0u;

	bool mouse_pressed	= false;
	bool gameover		= false;

public:
	Minesweeper(const unsigned int& mines, const unsigned int& columns, const unsigned int& rows);
	
public:
	void Update(Mouse& mouse);
	void Draw(Graphics& gfx);

private:
	void Setup();

	void InitialiseTextures();
	void InitialiseBorder();
	void SetBlockValues();
	void InitialiseGridBlocks();

	void InitialiseMinesCounter();
	void InitialiseResetButton();
	void InitialiseTimer();

	void SetMinesCounter();
	void SetTimer();
	void SetBlockValue(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void RevealBlocks(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void RevealBlock(const int& X, const int& Y, const int& COLS, const int& ROWS);
	void ExtractDigits(std::vector<unsigned int>& vec, const unsigned int& NUM);

	void DrawGrid(Graphics& gfx);
	void DrawGridBlocks(Graphics& gfx);
	void DrawMouseOverBlocks(Graphics& gfx);

	void DrawBorder(Graphics& gfx);

	void DrawDisplay(Graphics& gfx);
	void DrawMinesCounter(Graphics& gfx);
	void DrawResetButton(Graphics& gfx);
	void DrawTimer(Graphics& gfx);
};

