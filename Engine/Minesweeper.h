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
#include "Grid.h"

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
namespace SOUNDS
{
	static constexpr size_t FANFARE = 0;
	static constexpr size_t EXPLOSION = 1;
	static constexpr size_t CLICK_0 = 2;
	static constexpr size_t CLICK_1 = 3;
	static constexpr size_t CLICK_2 = 4;
}
namespace BUTTONS
{
	static constexpr size_t HELP = 0;
	static constexpr size_t SETTINGS = 2;
	static constexpr size_t RESET = 4;
}

/*

TO DO:

PRESET LEVELS - DONE
CUSTOM LEVELS
RIGHT CLICK ON REVEALED TILES TO CHECK IF CORRECT KINDA THING
LIMIT GAME TO WINDOW SIZE - DONE
ADD CAMERA SCROLL
FIND AND FIX BUGS

MAYBE DO:

ANIMATED TILES
SCOREBOARD
SAVE/LOAD
KEYBOARD CONTROLS

*/

class Minesweeper
{
private:
	static constexpr unsigned int SCREEN_W			= Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H			= Graphics::ScreenHeight;
	static constexpr unsigned int OFFSET			= 10u;
	static constexpr unsigned int DIGIT_COLS		= 3u;
	static constexpr unsigned int DIGIT_ROWS		= 10u;
	static constexpr unsigned int EXPLOSION_FRAMES	= 26u;
	static constexpr unsigned int FLAG_FRAMES		= 241u;
	static constexpr unsigned int BUTTONS_NUM		= 3u;

	static constexpr unsigned int COLS_BEGINNER			= 9u;
	static constexpr unsigned int ROWS_BEGINNER			= 9u;
	static constexpr unsigned int MINES_BEGINNER		= 10u;

	static constexpr unsigned int COLS_INTERMEDIATE		= 16u;
	static constexpr unsigned int ROWS_INTERMEDIATE		= 16u;
	static constexpr unsigned int MINES_INTERMEDIATE	= 40u;

	static constexpr unsigned int COLS_ADVANCED			= 30u;
	static constexpr unsigned int ROWS_ADVANCED			= 16u;
	static constexpr unsigned int MINES_ADVANCED		= 99u;

	std::unique_ptr<Grid>										grid;
		
	std::vector<std::shared_ptr<Surface>>						border_textures;
	std::vector<Block>											border_blocks;
	RectUI														border_position;
	
	std::vector<std::shared_ptr<Surface>>						digit_textures;

	std::array<std::array<Block, DIGIT_COLS>, DIGIT_ROWS>		mines_counter;
	std::vector<unsigned int>									mines_number;
	unsigned int												mines = 0u;

	std::array<std::array<Block, DIGIT_COLS>, DIGIT_ROWS>		timer;
	std::vector<unsigned int>									timer_number;
	bool														timer_started = false;
	unsigned int												time = 0;
	std::chrono::high_resolution_clock::time_point				t1;

	std::vector<std::shared_ptr<Surface>>						button_textures;
	std::array<RectUI, BUTTONS_NUM * 2u>						button_positions;
	std::array<Block, BUTTONS_NUM * 2u>							button_blocks;
	std::array<bool, BUTTONS_NUM>								button_pressed{ false };

	std::vector<std::shared_ptr<Surface>>						help_textures;
	Block														help_block;
	bool														is_help = false;

	std::vector<std::shared_ptr<Surface>>						settings_textures;
	std::vector<Block>											settings_blocks;
	std::vector<Block>											settings_text_blocks;
	bool														is_settings = false;
	bool														is_beginner = false;
	bool														is_intermediate = false;
	bool														is_advanced = false;
	bool														is_custom = false;
	Block settings_confirmation;

	RectUI														gameover_position;

	std::array<std::shared_ptr<Surface>, EXPLOSION_FRAMES>		explosion_textures;
	std::array<Block, EXPLOSION_FRAMES>							explosion_blocks;

	std::array<std::shared_ptr<Surface>, FLAG_FRAMES>			flag_textures;
	std::array<Block, FLAG_FRAMES>								flag_blocks;
	unsigned int												flags = 0u;
	
	const RectUI SCREEN_RECT{ 0u,SCREEN_H - 1u,0u,SCREEN_W - 1u };

	bool mouse_pressed	= false;
	bool gameover		= false;
	bool gamewon		= false;

	unsigned int index0 = 0;
	unsigned int index1 = 0;
	unsigned int frames = 0;

	std::vector<Sound> sounds;
	bool sound_played = false;

public:
	Minesweeper();
	
public:
	void Update(Mouse& mouse);
	void Draw(Graphics& gfx);

private:
	void Initialise();
	void Setup();
	void Reset();

	void InitialiseTextures();

	void InitialiseSettingsTextures();
	void InitialiseBorderTextures();
	void InitialiseDigitTextures();
	void InitialiseButtonTextures();
	void InitialiseGameOverTextures();

	void InitialiseBorder();
	void InitialiseMinesCounter();
	void InitialiseButtons();
	void InitialiseTimer();
	void InitialiseGameOver();
	void InitialiseSounds();
	void InitialiseHelp();
	void InitialiseSettings();

	void SetSettings(Mouse& mouse);
	void SetButtons(Mouse& mouse);
	void SetGrid(Mouse& mouse);
	void SetGameOver();
	void SetMinesCounter();
	void SetTimer();
	
	void ExtractDigits(std::vector<unsigned int>& vec, const unsigned int& NUM);
		
	void DrawBorder(Graphics& gfx);
	void DrawDisplayBackground(Graphics& gfx);
	void DrawMinesCounter(Graphics& gfx);
	void DrawButtons(Graphics& gfx);
	void DrawTimer(Graphics& gfx);
	void DrawGameOver(Graphics& gfx);

	void DrawHelp(Graphics& gfx);
	void DrawSettings(Graphics& gfx);
};

