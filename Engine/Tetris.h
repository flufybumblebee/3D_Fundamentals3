#pragma once

#include <string>
#include <array>
#include <vector>
#include <chrono>
#include <Windows.h>

#include "Block.h"
#include "Mouse.h   "
#include "Keyboard.h"
#include "Sound.h"
#include "Button.h"

class Tetris
{
private:
	Keyboard& kbd;
	Mouse& mouse;
	Graphics& gfx;	

private:
	static constexpr unsigned int	TETRO_NUM	= 7u;
	static constexpr unsigned int	COUNT_NUM	= 10u;

	/*------------------------------------------*/
	
	static constexpr unsigned int	TETRO_COLS	= 4u;
	static constexpr unsigned int	TETRO_ROWS	= 4u;
	static constexpr unsigned int	FIELD_COLS	= 12u;
	static constexpr unsigned int	FIELD_ROWS	= 18u;
	static constexpr unsigned int	SCORE_COLS	= 10u;
	static constexpr unsigned int	SCORE_ROWS	= 10u;
	static constexpr unsigned int	LEVEL_COLS	= 2u;
	static constexpr unsigned int	LEVEL_ROWS	= 10u;

	/*------------------------------------------*/

	std::array<std::string, TETRO_NUM>						tetromino;
	std::array<std::array<Block, FIELD_COLS>, FIELD_ROWS>	field_blocks;
	std::array<std::array<Block, TETRO_COLS>, TETRO_ROWS>	next_tetro_blocks;

	std::array<char, (FIELD_COLS * FIELD_ROWS)>				blockBuffer_Fixed{};
	std::array<char, (FIELD_COLS * FIELD_ROWS)>				blockBuffer_Shown{};
	std::array<std::array<Block, SCORE_COLS>, SCORE_ROWS>	score_blocks;
	std::array<std::array<Block, LEVEL_COLS>, LEVEL_ROWS>	level_blocks;
	std::array<Block, COUNT_NUM>							counter_blocks;

	/*------------------------------------------*/
	
	const unsigned int	SCREEN_W	= gfx.ScreenWidth;
	const unsigned int	SCREEN_H	= gfx.ScreenHeight;
	const unsigned int	BLOCK_W		= SCREEN_H / 20;
	const unsigned int	BLOCK_H		= SCREEN_H / 20;
	const unsigned int	KEY_W		= BLOCK_W * 2;
	const unsigned int	KEY_H		= BLOCK_H * 2;
	const unsigned int	BUTTON_W	= KEY_W;
	const unsigned int	BUTTON_H	= KEY_H;
	const unsigned int	DIGIT_W		= KEY_W;
	const unsigned int	DIGIT_H		= KEY_H;
	const unsigned int	FIELD_W		= BLOCK_W * FIELD_COLS;
	const unsigned int	FIELD_H		= BLOCK_H * FIELD_ROWS;

	const unsigned int	BLUR_NUM	= 7u;
	
	/*------------------------------------------*/
	
	std::vector<Surface>	block_textures;
	std::vector<Surface>	digit_textures;

	/*------------------------------------------*/

	std::vector<Surface>	background_textures;
	RectUI					background_position;
	Block					background_block;

	/*------------------------------------------*/

	RectUI					field_position;
	Block					field_block;

	/*------------------------------------------*/

	std::vector<Surface>	field_grid_textures;
	Block					field_grid_block;

	/*------------------------------------------*/

	std::vector<Surface>	pause_textures;
	RectUI					pause_position;
	Block					pause_block;
	bool					gameIsPaused		= false;

	/*------------------------------------------*/

	std::vector<Surface>	gameover_textures;
	RectUI					gameover_position;
	Block					gameover_block;
	bool					gameIsOver			= false;

	/*------------------------------------------*/

	std::vector<unsigned char> virtual_keys;

	std::vector<Surface>	key_textures;
	std::vector<RectUI>		key_position_a;
	std::vector<RectUI>		key_position_b;
	std::vector<Block>		key_a;
	std::vector<Block>		key_b;
	std::vector<bool>		key_mouseover;
	std::vector<bool>		key_mousepress;

	bool	keyIsPressed_UP		= false;
	bool	keyIsPressed_DOWN	= false;
	bool	keyIsPressed_LEFT	= false;
	bool	keyIsPressed_RIGHT	= false;
	bool	keyIsPressed_SPACE	= false;
	bool	keyIsPressed_ESCAPE = false;

	/*------------------------------------------*/
	
	std::vector<Surface>	button_texture_a;
	std::vector<Surface>	button_texture_b;

	std::vector<RectUI>		button_position;

	std::vector<Block>		button_a;
	std::vector<Block>		button_b;

	std::vector<bool>		button_mouseover;
	std::vector<bool>		button_mousepress;

	bool	button_settings_SHOW = false;	

	/*------------------------------------------*/

	std::vector<Surface>	button2_texture_a;
	std::vector<Surface>	button2_texture_b;
	std::vector<Surface>	button2_texture_c;
	std::vector<Surface>	button2_texture_d;

	std::vector<RectUI>		button2_position;

	std::vector<Block>		button2_a;
	std::vector<Block>		button2_b;
	std::vector<Block>		button2_c;
	std::vector<Block>		button2_d;

	std::vector<bool>		button2_mouseover;
	std::vector<bool>		button2_mousepress;

	bool	button2_volume_FULL	 = true;

	/*------------------------------------------*/

	std::vector<Sound> sounds;

	float volume	= 1.0f;
	float frequency = 1.0f;

	/*------------------------------------------*/

	std::vector<Surface>	box_textures;
	Block					box_block;

	/*------------------------------------------*/

	std::vector<Surface>		level_textures;
	Block						level_block;
	std::vector<unsigned int>	level_buffer;
	unsigned int				level				= 0u;
	unsigned int				prevLevel			= 0u;
	bool						level_goal			= false;

	/*------------------------------------------*/

	std::vector<Surface>		score_textures;
	Block						score_block;
	std::vector<unsigned int>	score_buffer;

	/*------------------------------------------*/

	std::vector<unsigned int>	lines;
	unsigned int				line_count = 0;

	/*------------------------------------------*/

	std::vector<Surface>		tetris_textures;
	RectUI						tetris_position;
	Block						tetris_block;

	/*------------------------------------------*/

	size_t	current_background	= 0u;

	unsigned int	currentX			= 0u;
	unsigned int	currentY			= 0u;
	unsigned int	currentRotation		= 0u;
	unsigned int	currentTetro		= 0u;
	unsigned int	nextTetro			= 0u;

	unsigned int	frameCounter		= 0u;
	unsigned int	score				= 0u;
	unsigned int	speed				= 0u;
	unsigned int	tickCounter			= 0u;
	unsigned int	counterTetro		= 0u;
	unsigned int	counterSpeed		= 0u;
	
	/*------------------------------------------*/

	bool	mouseIsPressed	= false;
	bool	mouse_press_LEFT = false;
	
public:
	Tetris(Keyboard& kbd, Mouse& mouse, Graphics& gfx);
	~Tetris() = default;

public:
	void	Update();
	void	Draw();

private:
	void	Initialise();
	void	Setup();

	/*------------------------------------------*/

	void	InitialiseBackground();
	void	InitialiseBlocks();
	void	InitialiseDigits();
	void	InitialiseTetrominos();
	void	InitialiseField();
	void	InitialiseFieldGrid();
	void	InitialiseNextTetro();
	void	InitialisePause();
	void	InitialiseGameOver();

	void	InitialiseLevel();
	void	InitialiseScore();
	void	InitialiseCounter();

	void	InitialiseKeys();
	void	InitialiseButtons();
	void	InitialiseButtons2();

	void	InitialiseSounds();

	void	InitialiseSettingsBox();

	void	InitialiseTetris();

	/*------------------------------------------*/

	void	Input();
	void	SetKeys();
	void	SetButtons();
	void	SetButtons2();
	void	Pause();
	void	Restart();
	void	Quit();
	void	Settings();
	void	Volume();

	/*------------------------------------------*/

	void	SetBackground();
	void	ResetField();
	void	SetFieldBlocks();
	void	SetNextTetro();
	void	CheckForLines();
	void	DeleteLines();
	void	SetFixedWithTetro();
	void	SetShownWithTetro();
	void	SetShownWithFixed();
	void	SetScore();
	void	SetLevel();
	void	SetCounter();

	/*------------------------------------------*/

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetro();
	void	DrawBlur();
	void	DrawPause();
	void	DrawGameOver();

	void	DrawScore();
	void	DrawLevel();
	void	DrawCounter();

	void	DrawKeys();
	void	DrawButtons();
	void	DrawButtons2();

	void	DrawFieldGrid();

	void	DrawBox();

	void	DrawTetris();

private:
	int		Rotate(int x, int y, int r);
	bool	DoesTetroFit(int offsetRotation,int offsetX,int offsetY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	int		ConvertCharToInt(const char value);

	std::vector<Color>	ConvertSurfaceToColorVector(Surface&& surface);
	std::vector<Color>	Blur(const int& WIDTH, const int& HEIGHT,const std::vector<Color>& input);

	/* FUNCTIONS I COULDN'T GET TO WORK */
	/*void	Benchmark(void* pFunction);*/
	/*void	BoxBlur(const Surface& input, std::vector<Color>& output);*/
	/*auto	boxesForGauss(int sigma, int n);*/
	/*void	gaussBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
	/*void	boxBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
	/*void	boxBlurH_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
	/*void	boxBlurT_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
	/*void	gaussBlur_1(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
	/*std::vector<Color> FastBlur(const std::vector<Color>& input, const int w, const int h, const int radius);*/
	/*Surface FastBlur(const Surface& input, int radius);*/
	/*void superFastBlur(unsigned char* pix, int w, int h, int radius);*/
};

