#pragma once

#include <string>
#include <array>
#include <vector>
#include <chrono>
#include <Windows.h>
#include <map>

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
	
	std::map<char, int>										values;

	std::array<std::string, TETRO_NUM>						tetromino;
	std::array<char, (FIELD_COLS * FIELD_ROWS)>				fixed_buffer{};
	std::array<char, (FIELD_COLS * FIELD_ROWS)>				shown_buffer{};

	std::array<std::array<Block, FIELD_COLS>, FIELD_ROWS>	field_blocks;
	std::array<std::array<Block, TETRO_COLS>, TETRO_ROWS>	next_tetro_blocks;
	std::array<std::array<Block, SCORE_COLS>, SCORE_ROWS>	score_blocks;
	std::array<std::array<Block, LEVEL_COLS>, LEVEL_ROWS>	level_blocks;

	std::array<Block, COUNT_NUM>							counter_blocks;

	/*------------------------------------------*/
	
	const unsigned int	SCREEN_W	= gfx.ScreenWidth;
	const unsigned int	SCREEN_H	= gfx.ScreenHeight;
	const unsigned int	BLOCK_W		= SCREEN_H / 20u;
	const unsigned int	BLOCK_H		= SCREEN_H / 20u;
	const unsigned int	FIELD_W		= BLOCK_W * FIELD_COLS;
	const unsigned int	FIELD_H		= BLOCK_H * FIELD_ROWS;

	const unsigned int	KEY_W		= BLOCK_W * 2u;
	const unsigned int	KEY_H		= BLOCK_H * 2u;
	const unsigned int	BUTTON_W	= KEY_W;
	const unsigned int	BUTTON_H	= KEY_H;
	const unsigned int	DIGIT_W		= KEY_W;
	const unsigned int	DIGIT_H		= KEY_H;

	const unsigned int	BLUR_NUM	= 7u;
	
	/*------------------------------------------*/

	std::vector<std::shared_ptr<Surface>> ptr_background_textures;
	std::vector<std::shared_ptr<Surface>> ptr_block_textures;
	std::vector<std::shared_ptr<Surface>> ptr_digit_textures;
	std::vector<std::shared_ptr<Surface>> ptr_key_textures;
	std::vector<std::shared_ptr<Surface>> ptr_button_quit_textures;
	std::vector<std::shared_ptr<Surface>> ptr_button_volume_textures;
	std::vector<std::shared_ptr<Surface>> ptr_paused_textures;
	std::vector<std::shared_ptr<Surface>> ptr_gameover_textures;
	std::vector<std::shared_ptr<Surface>> ptr_level_textures;
	std::vector<std::shared_ptr<Surface>> ptr_score_textures;
	std::vector<std::shared_ptr<Surface>> ptr_box_textures;
	std::vector<std::shared_ptr<Surface>> ptr_tetris_textures;
	std::vector<std::shared_ptr<Surface>> ptr_field_grid_textures;

	/*------------------------------------------*/
	
	RectUI	background_position;
	RectUI	field_position;
	RectUI	pause_position;
	RectUI	gameover_position;
	RectUI	tetris_position;

	Block	background_block;
	Block	field_block;
	Block	field_grid_block;
	Block	box_block;
	Block	paused_block;
	Block	gameover_block;
	Block	level_block;
	Block	score_block;
	Block	tetris_block;

	bool	game_is_paused		= false;
	bool	game_is_over		= false;
	bool	mouse_is_pressed	= false;

	/*------------------------------------------*/

	const bool	TESTING			= false;

	/*------------------------------------------*/

	// KEYS & BUTTONS
		
	std::vector<Button> keys;
	std::vector<bool>	key_is_pressed{false,false,false,false,false,false};
	
	Button				button_quit;
	Button				button_volume;
		
	bool				button_volume_full	 = true;
	 
	/*------------------------------------------*/

	// SOUNDS

	std::vector<Sound> sounds;

	float volume	= 1.0f;
	float frequency = 1.0f;
	
	/*------------------------------------------*/

	// LEVEL

	std::vector<unsigned int>	level_buffer;
	unsigned int				level		= 0u;
	unsigned int				level_prev	= 0u;
	bool						level_goal	= false;

	/*------------------------------------------*/
	
	std::vector<unsigned int>	score_buffer;

	/*------------------------------------------*/

	std::vector<unsigned int>	lines;
	unsigned int				line_count	= 0u;

	/*------------------------------------------*/
	
	size_t	current_background	= 0u;

	unsigned int	current_x			= 0u;
	unsigned int	current_y			= 0u;
	unsigned int	current_rotation	= 0u;
	unsigned int	current_tetro		= 0u;
	unsigned int	next_tetro			= 0u;

	unsigned int	score				= 0u;
	unsigned int	speed				= 0u;
	unsigned int	counter_frames		= 0u;
	unsigned int	counter_ticks		= 0u;
	unsigned int	counter_tetro		= 0u;
	unsigned int	counter_speed		= 0u;
	
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
	void	InitialiseBox();
	void	InitialisePaused();
	void	InitialiseGameOver();
	void	InitialiseLevel();
	void	InitialiseScore();
	void	InitialiseCounter();
	void	InitialiseTetris();
	void	InitialiseKeys();
	void	InitialiseButtons();
	void	InitialiseSounds();

	/*------------------------------------------*/

	void	Input();
	void	Pause();
	void	Restart();
	void	Quit();
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
	void	DrawFieldGrid();
	void	DrawBox();
	void	DrawTetris();

private:
	int		Rotate(int x, int y, int r);
	bool	DoesTetroFit(int offsetRotation,int offsetX,int offsetY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int& NUM);

	std::vector<Color>	ConvertSurfaceToColorVector(const Surface& surface);
	std::vector<Color>	Blur(const int& WIDTH, const int& HEIGHT,const std::vector<Color>& INPUT);

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

