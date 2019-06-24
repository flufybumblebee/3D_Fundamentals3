#pragma once

#include <array>
#include <vector>
#include <cassert>
#include <chrono>
#include <string>

#include "Graphics.h"
#include "Random.h"
#include "Rect.h"
#include "Mouse.h"
#include "Block.h"
#include "Sound.h"

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

/*
TO DO:

GAMEWIN & GAMELOSE			- DONE
FIX GAMELOSE IMAGE			- DONE
RESET BUTTON IMAGES			- DONE
LIMIT MINE COUNTER TO ZERO  - DONE
FIX TIMER GOING OVER 999    - DONE (RESETS TO ZERO)
ADD SOUND EFFECTS			- DONE (ADDED EXPLOSION & FANFARE SOUNDS)
ADD MORE SOUND EFFECTS
ADD GAMEWIN IMAGE			- DONE (ANIMATED FLAG)
LIMIT GAME TO WINDOW SIZE
SMALLER TILE NUMBERS		- STARTED
RIGHT CLICK ON REVEALED TILES TO CHECK IF CORRECT KINDA THING

MAYBE DO:

ANIMATED TILES
SCROLL/CAMERA (FOR LARGE LEVELS)
PRESET LEVELS
CUSTOM LEVELS
SCOREBOARD
SAVE/LOAD
KEYBOARD CONTROLS

*/

class Minesweeper
{
private:
	static constexpr unsigned int MIN_COLS		= 7u;
	static constexpr unsigned int MIN_ROWS		= 7u;
	static constexpr unsigned int SCREEN_W		= Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H		= Graphics::ScreenHeight;
	static constexpr unsigned int BLOCK_SIZE	= 33u;
	static constexpr unsigned int OFFSET		= 10u;
	static constexpr unsigned int DIGIT_COLS	= 3u;
	static constexpr unsigned int DIGIT_ROWS	= 10u;
	static constexpr unsigned int EXPLOSION_NUM = 26u;
	static constexpr unsigned int FLAG_NUM		= 241u;
	
	const unsigned int GRID_COLS;
	const unsigned int GRID_ROWS;
	const unsigned int GRID_SIZE;
	const unsigned int MINES;
	

	std::vector<std::shared_ptr<Surface>>						border_textures;
	std::vector<Block>											border_blocks;
	RectUI														border_position;

	RectUI														background_position;
	std::vector<std::shared_ptr<Surface>>						digit_textures;
	std::array<std::array<Block, DIGIT_COLS>, DIGIT_ROWS>		mines_counter;
	std::vector<unsigned int>									mines_number;
	unsigned int												mines = 0u;
	std::array<std::array<Block, DIGIT_COLS>, DIGIT_ROWS>		timer;
	std::vector<unsigned int>									timer_number;
	bool														timer_started = false;
	unsigned int												time = 0;
	std::chrono::high_resolution_clock::time_point				t1;

	std::vector<std::shared_ptr<Surface>>						reset_button_textures;
	RectUI														reset_button_position;
	std::array<Block, 2u>										reset_button_blocks;
	bool														reset_button_pressed = false;

	RectUI														grid_position;

	std::vector<unsigned int>									block_values;
	std::vector<std::shared_ptr<Surface>>						block_textures;
	std::vector<Block>											blocks;
	std::vector<bool>											block_revealed;
	std::vector<bool>											block_flag;

	RectUI														gameover_position;

	std::array<std::shared_ptr<Surface>, EXPLOSION_NUM>			explosion_textures;
	std::array<Block, EXPLOSION_NUM>							explosion_blocks;

	std::array<std::shared_ptr<Surface>, FLAG_NUM>				flag_textures;
	std::array<Block, FLAG_NUM>									flag_blocks;

	unsigned int flags = 0u;

	bool mouse_pressed	= false;
	bool gameover		= false;
	bool gamewon		= false;

	unsigned int index0 = 0;
	unsigned int index1 = 0;
	unsigned int frames = 0;

	std::vector<Sound> sounds;
	bool sound_played = false;

public:
	Minesweeper(const unsigned int& columns, const unsigned int& rows, const unsigned int& mines );
	
public:
	void Update(Mouse& mouse);
	void Draw(Graphics& gfx);

private:
	void Setup();

	void InitialiseDigitTextures();
	void InitialiseBorder();
	void SetBlockValues();
	void InitialiseGridBlocks();

	void InitialiseMinesCounter();
	void InitialiseResetButton();
	void InitialiseTimer();
	void InitialiseGameOver();

	void InitialiseSounds();

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

	void DrawDisplayBackground(Graphics& gfx);
	void DrawMinesCounter(Graphics& gfx);
	void DrawResetButton(Graphics& gfx);
	void DrawTimer(Graphics& gfx);
	void DrawGameOver(Graphics& gfx);
};

