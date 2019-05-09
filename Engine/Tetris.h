#pragma once

#include <string>
#include <array>
#include <vector>
#include <chrono>
#include <Windows.h>

#include "Block.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Sound.h"

class Tetris
{
private:
	Keyboard& kbd;
	Mouse& mouse;
	Graphics& gfx;

private:
	Sound sound_move;
	Sound sound_line1;
	Sound sound_line2;
	Sound sound_line3;
	Sound sound_line4;
	Sound sound_new_tetro;
	Sound sound_gameover;

private:
	static constexpr unsigned int	nTetro		= 7u;
	static constexpr unsigned int	nCount		= 10u;
	static constexpr unsigned int	nKeys		= 6u;
	static constexpr unsigned int	nButtons	= 3u;

	static constexpr unsigned int	tetroW	= 4u;
	static constexpr unsigned int	tetroH	= 4u;
	static constexpr unsigned int	fieldW	= 12u;
	static constexpr unsigned int	fieldH	= 18u;
	static constexpr unsigned int	scoreW	= 10u;
	static constexpr unsigned int	scoreH	= 10u;

	static constexpr unsigned int	keyW	= 50u;
	static constexpr unsigned int	keyH	= 50u;
	static constexpr unsigned int	buttonW = 50u;
	static constexpr unsigned int	buttonH = 50u;

	const unsigned int	blockW = 25u;
	const unsigned int	blockH = 25u;
	const unsigned int	digitW = 50u;
	const unsigned int	digitH = 50u;
	const unsigned int	pauseW = 580u;
	const unsigned int	pauseH = 100u;
	const unsigned int	gameOverW = 580u;
	const unsigned int	gameOverH = 290u;

	const unsigned int	nBlur = 7u;

	const unsigned int scrW = gfx.ScreenWidth;
	const unsigned int scrH = gfx.ScreenHeight;

	Block block_Background;
	Block block_Pause;
	Block block_GameOver;

	Block block_Box;

	std::vector<Surface>		texture_Box;
	std::vector<Surface>		texture_Mute;

	std::vector<Surface>		texture_Button;
	std::vector<Surface>		texture_Key;

	std::vector<Surface>		texture_Background;
	std::vector<Surface>		texture_Blocks;
	std::vector<Surface>		texture_Digits;
	std::vector<Surface>		texture_Pause;
	std::vector<Surface>		texture_GameOver;

	std::vector<unsigned int>	blockBuffer_Score;
	std::vector<unsigned int>	lines;

	std::vector<RectUI>			rect_Button;
	std::vector<RectUI>			rect_KeyReleased;
	std::vector<RectUI>			rect_KeyPressed;
	
	std::vector<bool>			mouseOverKey;
	std::vector<bool>			mousePressKey;
	std::vector<bool>			mouseOverButton;
	std::vector<bool>			mousePressButton;

	std::array<std::string, nTetro>					tetromino;
	std::array<std::array<Block, fieldW>, fieldH>	blocks_Field;
	std::array<std::array<Block, tetroW>, tetroH>	blocks_NextTetro;

	std::array<char, (fieldW* fieldH)>				blockBuffer_Fixed{};
	std::array<char, (fieldW* fieldH)>				blockBuffer_Shown{};
	std::array<std::array<Block, scoreW>, scoreH>	blocks_Score;
	std::array<Block, nCount>						blocks_Counter; 

	std::array<Block, nKeys>						block_KeyReleased;
	std::array<Block, nKeys>						block_KeyPressed;
	std::array<Block, nButtons>						block_Button;

	unsigned int	score			= 0u;
	unsigned int	index			= 0u;

	unsigned int	currentX		= 0u;
	unsigned int	currentY		= 0u;
	unsigned int	currentRotation	= 0u;
	unsigned int	currentTetro	= 0u;
	unsigned int	nextTetro		= 0u;

	bool	keyIsPressed_UP		= false;
	bool	keyIsPressed_DOWN	= false;
	bool	keyIsPressed_LEFT	= false;
	bool	keyIsPressed_RIGHT	= false;
	bool	keyIsPressed_SPACE	= false;
	bool	keyIsPressed_ESCAPE = false;
	bool	mouseIsPressed		= false;

	bool	button_Volume_FULL	 = false;
	bool	button_Settings_SHOW = false;

	bool	gameIsPaused		= false;
	bool	gameIsOver			= false;

	enum
	{
		ESCAPE,
		UP,
		SPACE,
		LEFT,
		DOWN,
		RIGHT
	} KEY;

	enum
	{
		QUIT,
		SETTINGS,
		VOLUME
	} BUTTON;

	float volume = 1.0f;
	float frequency = 1.0f;

	const int ROTATE_CW		= 1;
	const int MOVE_LEFT		= -1;
	const int MOVE_RIGHT	= 1;
	const int MOVE_DOWN		= 1;

	/*-------------------------------------------*/

	unsigned int	frameCounter	= 0u;
	unsigned int	speed			= 0u;
	unsigned int	level			= 0u;
	unsigned int	prevLevel		= 0u;

	unsigned int	tickCounter		= 0u;
	unsigned int	counterTetro	= 0u;
	unsigned int	counterSpeed	= 0u;

public:
	Tetris(Keyboard& kbd, Mouse& mouse, Graphics& gfx);

public:
	void	Update();
	void	Draw();

private:
	void	Initialise();
	void	Setup();

	void	InitialiseTextures();
	void	InitialiseBackground();
	void	InitialiseBlockPositions();
	void	InitialiseTetrominos();
	void	InitialiseNextTetro();
	void	InitialiseScore();
	void	InitialisePause();
	void	InitialiseGameOver();
	void	InitialiseCounter();
	void	InitialiseKeys();
	void	InitialiseButtons();
	void	InitialiseSettingsBox();

	void	Input();
	void	InputKeyboard();
	void	InputMouseKeys();
	void	InputMouseButtons();
	void	Pause();
	void	Reset();
	void	Quit();
	void	Settings();
	void	Sound();

	void	SetBackground();
	void	ResetScore();
	void	ResetField();
	void	SetScore();
	void	SetFieldBlocks();
	void	SetNextTetro();
	void	SetLevel();
	void	SetCounter();
	void	CheckForLines();
	void	DeleteLines();
	void	SetFixedWithTetro();
	void	SetShownWithTetro();
	void	SetShownWithFixed();

	void	DrawBackground();
	void	DrawFieldBlocks();
	void	DrawNextTetro();
	void	DrawBlur();
	void	DrawScore();
	void	DrawPause();
	void	DrawGameOver();
	void	DrawCounter();
	void	DrawKeys();
	void	DrawButtons();

	void	DrawBox();

private:
	int		Rotate(int x, int y, int r);
	bool	DoesTetroFit(int offsetRotation,int offsetX,int offsetY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	int		ConvertCharToInt(const char value);

	std::vector<Color>	ConvertSurfaceToColorVector(Surface surface);
	std::vector<Color>	Blur(const int w,const int h,const std::vector<Color>& input);

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

