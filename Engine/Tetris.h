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
#include "Button.h"

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
	static const unsigned int	tetroNUM	= 7u;
	static const unsigned int	countNUM	= 10u;

	/*------------------------------------------*/
	
	static const unsigned int	tetroCOLS	= 4u;
	static const unsigned int	tetroROWS	= 4u;
	static const unsigned int	fieldCOLS	= 12u;
	static const unsigned int	fieldROWS	= 18u;
	static const unsigned int	scoreCOLS	= 10u;
	static const unsigned int	scoreROWS	= 10u;

	/*------------------------------------------*/

	std::array<std::string, tetroNUM>					tetromino;
	std::array<std::array<Block, fieldCOLS>, fieldROWS>	blocks_Field;
	std::array<std::array<Block, tetroCOLS>, tetroROWS>	blocks_NextTetro;

	std::array<char, (fieldCOLS * fieldROWS)>			blockBuffer_Fixed{};
	std::array<char, (fieldCOLS * fieldROWS)>			blockBuffer_Shown{};
	std::array<std::array<Block, scoreCOLS>, scoreROWS>	blocks_Score;
	std::array<Block, countNUM>							blocks_Counter;

	/*------------------------------------------*/
	
	const unsigned int	scrW		= gfx.ScreenWidth;
	const unsigned int	scrH		= gfx.ScreenHeight;
	const unsigned int	keyW		= 50u;
	const unsigned int	keyH		= 50u;
	const unsigned int	buttonW		= 50u;
	const unsigned int	buttonH		= 50u;
	const unsigned int	blockW		= 25u;
	const unsigned int	blockH		= 25u;
	const unsigned int	digitW		= 50u;
	const unsigned int	digitH		= 50u;
	const unsigned int	pauseW		= 580u;
	const unsigned int	pauseH		= 100u;
	const unsigned int	gameOverW	= 580u;
	const unsigned int	gameOverH	= 290u;

	/*------------------------------------------*/

	const unsigned int	blurNUM = 7u;

	const int ROTATE_CW = 1;
	const int MOVE_LEFT = -1;
	const int MOVE_RIGHT = 1;
	const int MOVE_DOWN = 1;

	enum KEY
	{
		RESTART,
		ROTATE,
		PAUSE,
		LEFT,
		DOWN,
		RIGHT
	};

	enum BUTTON
	{
		QUIT,
		SETTINGS,
		SOUND
	};

	/*------------------------------------------*/
	
	std::vector<Surface>	texture_Background;
	std::vector<Surface>	texture_Blocks;
	std::vector<Surface>	texture_Digits;
	std::vector<Surface>	texture_Pause;
	std::vector<Surface>	texture_GameOver;

	/*------------------------------------------*/

	Block block_Background;
	Block block_Pause;
	Block block_GameOver;

	/*------------------------------------------*/

	std::vector<Surface>	key_texture;
	std::vector<Surface>	key_b_texture;
	std::vector<RectUI>		key_a_position;
	std::vector<RectUI>		key_b_position;

	std::vector<Button>		key;

	std::vector<bool>		mouseOverKey;
	std::vector<bool>		mousePressKey;

	bool	keyIsPressed_UP		= false;
	bool	keyIsPressed_DOWN	= false;
	bool	keyIsPressed_LEFT	= false;
	bool	keyIsPressed_RIGHT	= false;
	bool	keyIsPressed_SPACE	= false;
	bool	keyIsPressed_ESCAPE = false;

	/*------------------------------------------*/

	std::vector<Surface>	button_a_texture;
	std::vector<Surface>	button_b_texture;
	std::vector<RectUI>		button_a_position;
	std::vector<RectUI>		button_b_position;

	std::vector<Button2>	button;

	std::vector<bool>		mouseOverButton;
	std::vector<bool>		mousePressButton;

	bool	button_Volume_FULL	 = false;
	bool	button_Settings_SHOW = false;	

	/*------------------------------------------*/

	std::vector<Surface>	box_texture;
	Block					box_block;

	/*------------------------------------------*/

	std::vector<unsigned int>	blockBuffer_Score;
	std::vector<unsigned int>	lines;
	
	/*------------------------------------------*/

	unsigned int	score			= 0u;
	unsigned int	index			= 0u;

	unsigned int	currentX		= 0u;
	unsigned int	currentY		= 0u;
	unsigned int	currentRotation	= 0u;
	unsigned int	currentTetro	= 0u;
	unsigned int	nextTetro		= 0u;

	unsigned int	frameCounter	= 0u;
	unsigned int	speed			= 0u;
	unsigned int	level			= 0u;
	unsigned int	prevLevel		= 0u;

	unsigned int	tickCounter		= 0u;
	unsigned int	counterTetro	= 0u;
	unsigned int	counterSpeed	= 0u;

	bool	mouseIsPressed		= false;

	bool	gameIsPaused		= false;
	bool	gameIsOver			= false;

	/*-------------------------------------------*/

	float volume	= 1.0f;
	float frequency = 1.0f;


public:
	Tetris(Keyboard& kbd, Mouse& mouse, Graphics& gfx);
	~Tetris() = default;

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
	void	SetKeysWithMouse();
	void	SetButtonsWithMouse();
	void	InputKeyboard();
	void	Pause();
	void	Reset();
	void	Quit();
	void	Settings();
	void	Sound();

	void	SetButton(BUTTON btn);

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

