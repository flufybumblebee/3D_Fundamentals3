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
	static constexpr unsigned int	pauseW		= 35u;
	static constexpr unsigned int	pauseH		= 7u;
	static constexpr unsigned int	gameOverW	= 29u;
	static constexpr unsigned int	gameOverH	= 13u;
	static constexpr unsigned int	rows		= 10u;
	static constexpr unsigned int	cols		= 10u;

	const unsigned int offsetWidth	= (gfx.ScreenWidth  / 2) - ((fieldW / 2) * blocksW);
	const unsigned int offsetHeight	= (gfx.ScreenHeight / 2) - ((fieldH / 2) * blocksH);
	const unsigned int scrW = gfx.ScreenWidth;
	const unsigned int scrH = gfx.ScreenHeight;
	
	Surface	tex_background	= Surface::FromFile(L"Textures\\Backgrounds\\Nature.jpg");

	Surface tex_tileBlack	= Surface::FromFile(L"Textures\\Blocks\\Block_DarkGrey.png");
	Surface tex_tileOrange	= Surface::FromFile(L"Textures\\Blocks\\Block_Orange.png");
	Surface tex_tileCyan	= Surface::FromFile(L"Textures\\Blocks\\Block_Cyan.png");
	Surface tex_tileGreen	= Surface::FromFile(L"Textures\\Blocks\\Block_Green.png");
	Surface tex_tileRed		= Surface::FromFile(L"Textures\\Blocks\\Block_Red.png");
	Surface tex_tileBlue	= Surface::FromFile(L"Textures\\Blocks\\Block_Blue.png");
	Surface tex_tileMagenta = Surface::FromFile(L"Textures\\Blocks\\Block_Magenta.png");
	Surface tex_tileYellow	= Surface::FromFile(L"Textures\\Blocks\\Block_Yellow.png");
	Surface tex_tileGrey	= Surface::FromFile(L"Textures\\Blocks\\Block_Grey.png");

	Surface tex_tileZeroG	= Surface::FromFile(L"Digits\\Digit - 0.png");
	Surface tex_tileOneG	= Surface::FromFile(L"Digits\\Digit - 1.png");
	Surface tex_tileTwoG	= Surface::FromFile(L"Digits\\Digit - 2.png");
	Surface tex_tileThreeG	= Surface::FromFile(L"Digits\\Digit - 3.png");
	Surface tex_tileFourG	= Surface::FromFile(L"Digits\\Digit - 4.png");
	Surface tex_tileFiveG	= Surface::FromFile(L"Digits\\Digit - 5.png");
	Surface tex_tileSixG	= Surface::FromFile(L"Digits\\Digit - 6.png");
	Surface tex_tileSevenG	= Surface::FromFile(L"Digits\\Digit - 7.png");
	Surface tex_tileEightG	= Surface::FromFile(L"Digits\\Digit - 8.png");
	Surface tex_tileNineG	= Surface::FromFile(L"Digits\\Digit - 9.png");

	Surface digit_A			= Surface::FromFile(L"Digits\\Digit - A.png");
	Surface digit_B			= Surface::FromFile(L"Digits\\Digit - B.png");
	Surface digit_C			= Surface::FromFile(L"Digits\\Digit - C.png");
	Surface digit_D			= Surface::FromFile(L"Digits\\Digit - D.png");

	Surface words_Score		= Surface::FromFile(L"Words\\Words - Score.png");
	Surface words_Pause		= Surface::FromFile(L"Words\\Words - Pause.png");
	Surface words_GameOver	= Surface::FromFile(L"Words\\Words - Game Over.png");

	Block background;

	std::array<Color, 10>		block_Colors;

	const std::array<Surface*, 10>	block_Textures = {	&tex_tileBlack,
														&tex_tileOrange,
														&tex_tileCyan,
														&tex_tileGreen,
														&tex_tileRed,
														&tex_tileBlue,
														&tex_tileMagenta,
														&tex_tileYellow,
														&tex_tileRed,
														&tex_tileGrey };

	const std::array<Surface*, 10>	digit_Textures = {	&tex_tileZeroG,
														&tex_tileOneG,
														&tex_tileTwoG,
														&tex_tileThreeG,
														&tex_tileFourG,
														&tex_tileFiveG,
														&tex_tileSixG,
														&tex_tileSevenG,
														&tex_tileEightG,
														&tex_tileNineG };

	//std::array<std::array<Block,digitW*digitH>,10>	blocks_Digits;
	//Block blocks_Digits[rows][cols];
	std::array<std::array<Block, cols>, rows> blocks_Digits;

	std::string			tetromino[7];
	//std::string			text_Digits[10];
	std::string			text_Level;
	//std::string			text_Score;
	std::string			text_Pause;
	//std::string			text_GameOver;

	std::vector<int>	lines;

	std::vector<Block>	blocks;
	std::vector<Block>	blocks_Next;

	std::array<Block, levelW*levelH>		blocks_Text_Level;
	std::array<Block, scoreW*scoreH>		blocks_Text_Score;
	std::array<Block, pauseW*pauseH>		blocks_Text_Pause;
	std::array<Block, gameOverW*gameOverH>	blocks_Text_GameOver;
		 
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

