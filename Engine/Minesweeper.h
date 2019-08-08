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

namespace SOUNDS
{
	static constexpr size_t FANFARE		= 0;
	static constexpr size_t EXPLOSION	= 1;
	static constexpr size_t CLICK_0		= 2;
	static constexpr size_t CLICK_1		= 3;
	static constexpr size_t CLICK_2		= 4;
	static constexpr size_t CLICK_3		= 5;
}

namespace BUTTON
{
	namespace TEXTURE
	{
		static constexpr size_t BACKGROUND		= 0;
		static constexpr size_t DIVIDER			= 1;

		static constexpr size_t COLS			= 2;
		static constexpr size_t ROWS			= 3;
		static constexpr size_t MINES			= 4;
		static constexpr size_t LEVEL_EASY		= 5;
		static constexpr size_t LEVEL_MEDIUM	= 6;
		static constexpr size_t LEVEL_HARD		= 7;
		static constexpr size_t LEVEL_CUSTOM	= 8;

		static constexpr size_t HELP			= 9;
		static constexpr size_t RESTART_SMILE	= 10;
		static constexpr size_t RESTART_WIN		= 11;
		static constexpr size_t RESTART_LOSE	= 12;
		static constexpr size_t RESTART_GASP	= 13;
		static constexpr size_t SETTINGS		= 14;

		static constexpr size_t VOLUME			= 15;
		static constexpr size_t VOLUME_MUTE		= 16;
		static constexpr size_t TIMER			= 17;
	}

	namespace RECT
	{
		static constexpr size_t COLS = 0;
		static constexpr size_t ROWS = 1;
		static constexpr size_t MINES = 2;
		static constexpr size_t LEVEL = 3;

		static constexpr size_t HELP_RELEASE = 4;
		static constexpr size_t HELP_PRESSED = 5;
		static constexpr size_t RESTART_RELEASE = 6;
		static constexpr size_t RESTART_PRESSED = 7;
		static constexpr size_t SETTINGS_RELEASE = 8;
		static constexpr size_t SETTINGS_PRESSED = 9;

		static constexpr size_t VOLUME = 10;
		static constexpr size_t TIMER = 11;
	}

	static constexpr size_t COLS		= 0;
	static constexpr size_t ROWS		= 1;
	static constexpr size_t MINES		= 2;

	static constexpr size_t LEVEL		= 3;

	static constexpr size_t HELP		= 4;
	static constexpr size_t RESTART		= 5;
	static constexpr size_t SETTINGS	= 6;

	static constexpr size_t TIMER		= 7;
	static constexpr size_t VOLUME		= 8;
}

namespace LEVEL
{
	namespace EASY
	{
		static constexpr unsigned int COLS = 9u;
		static constexpr unsigned int ROWS = 9u;
		static constexpr unsigned int MINES = 10u;
	}

	namespace MEDIUM
	{
		static constexpr unsigned int COLS = 16u;
		static constexpr unsigned int ROWS = 16u;
		static constexpr unsigned int MINES = 40u;
	}

	namespace HARD
	{
		static constexpr unsigned int COLS = 30u;
		static constexpr unsigned int ROWS = 16u;
		static constexpr unsigned int MINES = 99u;
	}
}

/*
TO DO:
	TEXTURE - HELP SCREEN (started)
	Convert SetGrid() to grid based function

MAYBE DO:

SETTINGS:
	CUSTOM LEVEL SETTINGS
	VOLUME ADJUST	(MUSIC & FX)
	VOLUME MUTE		(MUSIC & FX)
	TEXT SIZE ADJUST
	TILE BACKGROUND TEXTURE ADJUST
	TILE MOUSEOVER TEXTURE ADJUST

GRID:
	CAMERA SCROLL
	ANIMATED TILES

SCOREBOARD
SAVE/LOAD
KEYBOARD CONTROLS
ADD MUSIC

mouse/tile based dynamic shadows with numbers bombs and flags

*/

class Minesweeper
{
private:
	static constexpr unsigned int SCREEN_W			= Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H			= Graphics::ScreenHeight;
	static constexpr unsigned int OFFSET			= 10u;
	static constexpr unsigned int DIGIT_COLS		= 3u;
	static constexpr unsigned int DIGIT_ROWS		= 10u;
	static constexpr unsigned int DIGIT_W			= OFFSET * 4u;
	static constexpr unsigned int DIGIT_H			= OFFSET * 4u;
	static constexpr unsigned int EXPLOSION_FRAMES	= 26u;
	static constexpr unsigned int FLAG_FRAMES		= 241u;
	static constexpr unsigned int BUTTONS_NUM		= 3u;

	const RectUI SCREEN_RECT{ 0u,SCREEN_H - 1u,0u,SCREEN_W - 1u };

	/*------------------------------------------------------------------------------------*/

	std::vector<Sound>	sounds;
	std::vector<Sound>	win_sounds;
	bool				gameover_sound_played	= false;
	bool				checked_sound_played	= false;
	bool				volume_mute				= false;
	float				sound_fx_volume			= 1.0f;
	float				music_volume			= 1.0f;

	/*------------------------------------------------------------------------------------*/
	
	std::vector<std::shared_ptr<Surface>>						grid_textures;
	std::vector<std::shared_ptr<Surface>>						tile_textures;
	std::unique_ptr<Grid>										grid = nullptr;
		
	/*------------------------------------------------------------------------------------*/

	std::array<std::shared_ptr<Surface>, DIGIT_ROWS>			digit_textures{ nullptr };

	/*------------------------------------------------------------------------------------*/

	std::array<RectUI, DIGIT_COLS>								mine_counter_rects;
	std::array<std::array<Block, DIGIT_COLS>, DIGIT_ROWS>		mine_counter;
	std::vector<unsigned int>									mine_counter_value;
	unsigned int												mines = 0u;
	RectUI														mine_counter_rect;
	
	/*------------------------------------------------------------------------------------*/

	std::array<RectUI, DIGIT_COLS>								timer_rects;
	std::array<std::array<Block, DIGIT_COLS>, DIGIT_ROWS>		timer_blocks;
	std::vector<unsigned int>									timer_value;
	bool														timer_started = false;
	unsigned int												time = 0;
	std::chrono::high_resolution_clock::time_point				t1;
	RectUI														timer_rect;
	
	/*------------------------------------------------------------------------------------*/
	
	std::vector<std::shared_ptr<Surface>>						button_textures;

	RectUI														button_background_rect;
	Block														button_background_block;

	std::vector<RectUI>											button_rects;
	std::vector<Block>											button_blocks;

	std::vector<RectUI>											button_divider_rects;
	std::vector<Block>											button_divider_blocks;

	std::vector<bool>											button_pressed;

	unsigned int level = 0u;
	unsigned int custom_cols = 3u;
	unsigned int custom_rows = 3u;
	unsigned int custom_mines = 2u;
	
	/*------------------------------------------------------------------------------------*/

	std::vector<std::shared_ptr<Surface>>						help_textures;
	Block														help_block;
	bool														is_help = false;

	std::vector<std::shared_ptr<Surface>>						help_button_textures;
	Block														help_button_block;

	/*------------------------------------------------------------------------------------*/

	std::vector<std::shared_ptr<Surface>>						settings_textures;
	std::array<Block, 6>										settings_text_blocks;
	std::array<Block, 5>										settings_blocks;
	bool														is_settings = true;
	
	/*------------------------------------------------------------------------------------*/

	RectUI														gameover_rect;

	std::array<std::shared_ptr<Surface>, EXPLOSION_FRAMES>		explosion_textures{ nullptr };
	std::array<Block, EXPLOSION_FRAMES>							explosion_blocks;

	std::array<std::shared_ptr<Surface>, FLAG_FRAMES>			flag_textures{ nullptr };
	std::array<Block, FLAG_FRAMES>								flag_blocks;
	unsigned int												flags = 0u;

	/*------------------------------------------------------------------------------------*/
	
	bool mouse_pressed	= false;
	bool gameover		= false;
	bool gamewon		= false;

	/*------------------------------------------------------------------------------------*/

	unsigned int index0 = 0;
	unsigned int index1 = 0;
	unsigned int frames = 0;

	
private:
	void InitialiseSounds();
	void InitialiseTextures();

	void InitialiseHelpTextures();
	void InitialiseGridTextures();
	void InitialiseTileTextures();
	void InitialiseDigitTextures();
	void InitialiseButtonTextures();
	void InitialiseGameOverTextures();

	void InitialiseDisplay();

	void InitialiseButtonBackground();
	void InitialiseMinesCounter();
	void InitialiseTimer();
	void InitialiseButtons();
	void InitialiseButtonDividers();

	void InitialiseGameOver();
	void InitialiseHelp();
	
	void SetSound();
	void SetMinesCounter();
	void SetTimer();
	void SetHelp(Mouse& mouse);
	void SetButtons(Mouse& mouse);
	void SetGrid(Mouse& mouse);
	void SetGameOver();
	
	void DrawButtonDisplay(Graphics& gfx);

	void DrawButtonBackground(Graphics& gfx);
	void DrawMinesCounter(Graphics& gfx);
	void DrawTimer(Graphics& gfx);
	void DrawButtons(Graphics& gfx);
	void DrawButtonDividers(Graphics& gfx);

	void DrawGameOver(Graphics& gfx);

	void DrawHelp(Graphics& gfx);
	
public:
	Minesweeper();

private:
	void Setup();
	void Restart();

public:
	void Update(Mouse& mouse);
	void Draw(Graphics& gfx);
};

