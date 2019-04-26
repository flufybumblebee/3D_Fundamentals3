#pragma once

#include <string>
#include <array>
#include <vector>
#include <chrono>

#include "Block.h"
#include "Keyboard.h"
#include <Windows.h>
#include "Sound.h"



class Tetris
{
private:
	Keyboard& kbd;
	Graphics& gfx;

private:
	Sound sound_move;
	Sound sound_lines_1;
	Sound sound_lines_2;
	Sound sound_lines_3;
	Sound sound_lines_4;
	Sound sound_new_tetro;
	Sound sound_gameover;

private:	
	static constexpr unsigned int	nTetro	= 7u;
	static constexpr unsigned int	tetroW	= 4u;
	static constexpr unsigned int	tetroH	= 4u;
	static constexpr unsigned int	fieldW	= 12u;
	static constexpr unsigned int	fieldH	= 18u;
	static constexpr unsigned int	scoreW	= 10u;
	static constexpr unsigned int	scoreH	= 10u;

	const unsigned int	blockW		= 25u;
	const unsigned int	blockH		= 25u;
	const unsigned int	digitW		= 50u;
	const unsigned int	digitH		= 50u;
	const unsigned int	pauseW		= 580u;
	const unsigned int	pauseH		= 100u;
	const unsigned int	gameOverW	= 580u;
	const unsigned int	gameOverH	= 290u;

	const unsigned int	blurLevel	= 7u;

	const unsigned int scrW = gfx.ScreenWidth;
	const unsigned int scrH = gfx.ScreenHeight;
	
	Block block_Background;
	Block block_Pause;
	Block block_GameOver;

	std::vector<Surface> texture_Background;
	std::vector<Surface> texture_Blocks;
	std::vector<Surface> texture_Digits;
	std::vector<Surface> texture_Pause;
	std::vector<Surface> texture_GameOver;
	 
	std::array<std::string, nTetro>					tetromino;
	std::array<std::array<Block, fieldW>, fieldH>	blocks_Field;
	std::array<std::array<Block, tetroW>, tetroH>	blocks_NextTetro;

	std::array<char, (fieldW * fieldH)>				blockBuffer_Fixed{};
	std::array<char, (fieldW * fieldH)>				blockBuffer_Shown{};
	std::array<std::array<Block, scoreW>, scoreH>	blocks_Score;
	std::array<Block, 10>							blocks_Counter;

	std::vector<unsigned int>						blockBuffer_Score;
	std::vector<int>								lines;

	unsigned int	speed			= 0u;
	unsigned int	score			= 0u;
	unsigned int	level			= 0u;
	unsigned int	counterTetro	= 0u;
	unsigned int	counterSpeed	= 0u;
	unsigned int	index			= 0u;
	unsigned int	prevLevel		= 0u;

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

	bool	gameIsPaused		= false;
	bool	gameIsOver			= false;
	
	/*-------------------------------------------*/

	unsigned int tickCounter = 0u;
	unsigned int frameCounter = 0u;

public:
	Tetris(Keyboard& kbd, Graphics& gfx);
	~Tetris();

public:
	void	Setup();
	void	Update();
	void	Draw();

private:
	void	InitialiseBackground();
	void	InitialiseBlockTextures();
	void	InitialiseBlockPositions();
	void	InitialiseTetrominos();
	void	InitialiseNextTetro();
	void	InitialiseDigits();
	void	InitialiseScore();
	void	InitialisePause();
	void	InitialiseGameOver();
	void	InitialiseCounter();

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
	void	PauseOrReset();

	void	DrawBackground();
	void	DrawFieldBlocks();
	void	DrawNextTetro();
	void	DrawBlur();
	void	DrawScore();
	void	DrawPause();
	void	DrawGameOver();
	void	DrawCounter();

private:
	int		Rotate(int px, int py, int r);
	bool	DoesTetroFit(int tetrisID, int rotation, int posX, int posY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	int		ConvertCharToInt(const char value);

	std::vector<Color> ConvertSurfaceToColorVector(Surface surface);
	std::vector<Color> Blur(const int w,const int h,const std::vector<Color>& input);

	template <typename T>
	T RandomInt(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<T> dist(min, max);

		return dist(rng);
	}

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

