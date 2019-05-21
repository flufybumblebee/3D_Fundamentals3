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

	/*------------------------------------------*/

	std::array<std::string, TETRO_NUM>						tetromino;
	std::array<std::array<Block, FIELD_COLS>, FIELD_ROWS>	field_blocks;
	std::array<std::array<Block, TETRO_COLS>, TETRO_ROWS>	next_tetro_blocks;

	std::array<char, (FIELD_COLS * FIELD_ROWS)>				blockBuffer_Fixed{};
	std::array<char, (FIELD_COLS * FIELD_ROWS)>				blockBuffer_Shown{};
	std::array<std::array<Block, SCORE_COLS>, SCORE_ROWS>	score_blocks;
	std::array<Block, COUNT_NUM>							counter_blocks;

	/*------------------------------------------*/
	
	const unsigned int	SCREEN_W	= gfx.ScreenWidth;
	const unsigned int	SCREEN_H	= gfx.ScreenHeight;
	const unsigned int	BLOCK_W		= 25u;
	const unsigned int	BLOCK_H		= 25u;
	const unsigned int	KEY_W		= 50u;
	const unsigned int	KEY_H		= 50u;
	const unsigned int	BUTTON_W	= 50u;
	const unsigned int	BUTTON_H	= 50u;
	const unsigned int	DIGIT_W		= 50u;
	const unsigned int	DIGIT_H		= 50u;
	const unsigned int	PAUSE_W		= 580u;
	const unsigned int	PAUSE_H		= 100u;
	const unsigned int	GAMEOVER_W	= 580u;
	const unsigned int	GAMEOVER_H	= 290u;

	const unsigned int	BLUR_NUM	= 7u;

	const unsigned int	FIELD_W		= BLOCK_W * FIELD_COLS;
	const unsigned int	FIELD_H		= BLOCK_H * FIELD_ROWS;
	
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

	/*------------------------------------------*/

	std::vector<Surface>	gameover_textures;
	RectUI					gameover_position;
	Block					gameover_block;

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

	std::vector<unsigned int>	blockBuffer_Score;
	std::vector<unsigned int>	lines;
	
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
	unsigned int	level				= 0u;
	unsigned int	prevLevel			= 0u;
	unsigned int	tickCounter			= 0u;
	unsigned int	counterTetro		= 0u;
	unsigned int	counterSpeed		= 0u;

	/*------------------------------------------*/

	bool	mouseIsPressed	= false;

	bool	gameIsPaused	= false;
	bool	gameIsOver		= false;

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

	void	InitialiseScore();
	void	InitialiseCounter();

	void	InitialiseKeys();
	void	InitialiseButtons();
	void	InitialiseButtons2();

	void	InitialiseSounds();

	void	InitialiseSettingsBox();

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
	void	ResetScore();
	void	ResetField();

	/*------------------------------------------*/

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetro();
	void	DrawBlur();
	void	DrawPause();
	void	DrawGameOver();
	void	DrawScore();
	void	DrawCounter();
	void	DrawKeys();
	void	DrawButtons();
	void	DrawButtons2();

	void	DrawFieldGrid();

	void	DrawBox();

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

