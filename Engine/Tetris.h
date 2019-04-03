#pragma once

#include <string>
#include <array>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>
#include <cassert>

#include "Block.h"
#include "Keyboard.h"

#include "Benchmark.h"
#include "Mat.h"

class Tetris
{
private:
	Keyboard& kbd;
	Graphics& gfx;

private:	
	static constexpr unsigned int	fieldW		= 12u;
	static constexpr unsigned int	fieldH		= 18u;
	static constexpr unsigned int	tetroW		= 4u;
	static constexpr unsigned int	tetroH		= 4u;
	static constexpr unsigned int	bloW		= 7u;
	static constexpr unsigned int	bloH		= 7u;
	static constexpr unsigned int	blocksW		= 20u;
	static constexpr unsigned int	blocksH		= 20u;
	static constexpr unsigned int	levelW		= 37u;
	static constexpr unsigned int	levelH		= 7u;
	static constexpr unsigned int	scoreW		= 200u;
	static constexpr unsigned int	scoreH		= 50u;
	static constexpr unsigned int	digitW		= 50u;
	static constexpr unsigned int	digitH		= 50u;
	static constexpr unsigned int	pauseW		= 580u;
	static constexpr unsigned int	pauseH		= 100u;
	static constexpr unsigned int	gameOverW	= 29u;
	static constexpr unsigned int	gameOverH	= 13u;
	static constexpr unsigned int	rows		= 10u;
	static constexpr unsigned int	cols		= 10u;

	const unsigned int offsetWidth	= (gfx.ScreenWidth  / 2) - ((fieldW / 2) * blocksW);
	const unsigned int offsetHeight	= (gfx.ScreenHeight / 2) - ((fieldH / 2) * blocksH);
	const unsigned int scrW = gfx.ScreenWidth;
	const unsigned int scrH = gfx.ScreenHeight;
	
	Surface* tex_background = new Surface(Surface::FromFile(L"Textures\\Backgrounds\\Nature.jpg"));

	Surface* tex_Black		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_DarkGrey.png"));
	Surface* tex_Orange		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Orange.png"));
	Surface* tex_Cyan		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Cyan.png"));
	Surface* tex_Green		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Green.png"));
	Surface* tex_Red		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Red.png"));
	Surface* tex_Blue		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Blue.png"));
	Surface* tex_Magenta	= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Magenta.png"));
	Surface* tex_Yellow		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Yellow.png"));
	Surface* tex_Grey		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Grey.png"));

	//Surface* digits;

	Surface* digit_0 = new Surface(Surface::FromFile(L"Digits\\Digit - 0.png"));
	Surface* digit_1 = new Surface(Surface::FromFile(L"Digits\\Digit - 1.png"));
	Surface* digit_2 = new Surface(Surface::FromFile(L"Digits\\Digit - 2.png"));
	Surface* digit_3 = new Surface(Surface::FromFile(L"Digits\\Digit - 3.png"));
	Surface* digit_4 = new Surface(Surface::FromFile(L"Digits\\Digit - 4.png"));
	Surface* digit_5 = new Surface(Surface::FromFile(L"Digits\\Digit - 5.png"));
	Surface* digit_6 = new Surface(Surface::FromFile(L"Digits\\Digit - 6.png"));
	Surface* digit_7 = new Surface(Surface::FromFile(L"Digits\\Digit - 7.png"));
	Surface* digit_8 = new Surface(Surface::FromFile(L"Digits\\Digit - 8.png"));
	Surface* digit_9 = new Surface(Surface::FromFile(L"Digits\\Digit - 9.png"));

	/*Surface* digit_A = new Surface(Surface::FromFile(L"Digits\\Digit - A.png"));
	Surface* digit_E = new Surface(Surface::FromFile(L"Digits\\Digit - E.png"));
	Surface* digit_G = new Surface(Surface::FromFile(L"Digits\\Digit - G.png"));
	Surface* digit_M = new Surface(Surface::FromFile(L"Digits\\Digit - M.png"));
	Surface* digit_O = new Surface(Surface::FromFile(L"Digits\\Digit - O.png"));
	Surface* digit_P = new Surface(Surface::FromFile(L"Digits\\Digit - P.png"));
	Surface* digit_R = new Surface(Surface::FromFile(L"Digits\\Digit - R.png"));
	Surface* digit_S = new Surface(Surface::FromFile(L"Digits\\Digit - S.png"));
	Surface* digit_V = new Surface(Surface::FromFile(L"Digits\\Digit - V.png"));
	Surface* digit_U = new Surface(Surface::FromFile(L"Digits\\Digit - U.png"));*/

	const Surface* word_Pause = new Surface(Surface::FromFile(L"Words\\Word - Pause.png"));
	//const Surface* word_GameOver; // = Surface::FromFile(L"Words\\Words - Game Over.png");

	Block background;

	//std::array<Color, 10> block_Colors;

	const std::array<Surface*, 10>	block_Textures = {	tex_Black,
														tex_Orange,
														tex_Cyan,
														tex_Green,
														tex_Red,
														tex_Blue,
														tex_Magenta,
														tex_Yellow,
														tex_Red,
														tex_Grey };

	std::array<Surface*, 10>		digit_Numbers = {	digit_0,
														digit_1,
														digit_2,
														digit_3,
														digit_4,
														digit_5,
														digit_6,
														digit_7,
														digit_8,
														digit_9 };
													

	/*const std::array<Surface*, 10>	digit_Letters = {	digit_A,
														digit_E,
														digit_G,
														digit_M,
														digit_O,
														digit_P,
														digit_R,
														digit_S,
														digit_V,
														digit_U };*/

	//std::array<std::array<Block,digitW*digitH>,10>	blocks_Digits;
	//Block blocks_Digits[rows][cols];
	std::array<std::array<Block, cols>, rows> blocks_Digits;

	std::string			tetromino[7];
	std::string			text_Level;

	std::vector<int>	lines;

	std::vector<Block>	blocks;
	std::vector<Block>	blocks_Next;

	//std::array<Block, levelW*levelH>		blocks_Text_Level;
	//std::array<Block, scoreW*scoreH>		blocks_Text_Score;
	//std::array<Block, pauseW*pauseH>		blocks_Text_Pause;
	//std::array<Block, gameOverW*gameOverH>	blocks_Text_GameOver;
		 
	std::vector<char>			blockBuffer_Fixed;
	std::vector<char>			blockBuffer_Shown;
	std::vector<unsigned int>	blockBuffer_Digits;

	unsigned int	speed				= 20;
	unsigned int	score				= 0;
	unsigned int	level				= 0;
	unsigned int	tetrominoCounter	= 0;
	unsigned int	speedCounter		= 0;
	
	unsigned int	tetrominoNext		= 0;
	unsigned int	tetrominoCurrent	= 0;
	unsigned int	currentRotation		= 0;
	unsigned int	currentX			= fieldW / 2 - 2;
	unsigned int	currentY			= 0;

	bool	keyIsPressed		= false;
	bool	spaceIsPressed		= false;
	bool	forceDown			= false;
	bool	gameIsPaused		= false;
	bool	gameIsOver			= false;

	bool	isTesting			= true;
	
public:
	Tetris(Keyboard& kbd, Graphics& gfx);
	~Tetris();

public:
	void	Setup();
	void	Update();
	void	Draw();

private:
	void	InitialiseBackground();
	void	InitialiseTetrominos();
	void	InitialiseDigits();

	void	InitialiseTextScore();
	void	InitialiseTextDigits();
	void	InitialiseTextLevel();
	void	InitialiseTextPause();
	void	InitialiseTextGameOver();

	void	SetFieldBuffer();
	void	SetFieldBlocks();
	void	SetNextTetromino();
	void	SetScore();

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetromino();
	void	DrawTextScore();
	void	DrawLevel();
	void	DrawTextDigits();
	void	DrawTextPause();
	void	DrawTextGameOver();

	void	DrawDigit(unsigned int xOff, unsigned int yOff, const Surface& digit);
	void	DrawBlur();

private:
	int		Random(const int min, const int max);
	int		Rotate(int px, int py, int r);
	bool	DoesTetrisFit(int tetrisID, int rotation, int posX, int posY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	unsigned int	ConvertCharToInt(const char value);
	void	Benchmark(void* pFunction);

	const std::vector<Color> Blur(
		const unsigned int width,
		const unsigned int height,
		const std::vector<Color>& input);
	void BoxBlur(const Surface& input, std::vector<Color>& output);

	auto boxesForGauss(int sigma, int n);
	void gaussBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurH_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurT_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void gaussBlur_1(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
};

