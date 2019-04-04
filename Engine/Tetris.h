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
	static constexpr int	tetroW		= 4u;
	static constexpr int	tetroH		= 4u;
	static constexpr int	bloW		= 7u;
	static constexpr int	bloH		= 7u;
	static constexpr int	fieldW		= 12u;
	static constexpr int	fieldH		= 18u;
	static constexpr int	blocksW		= 20u;
	static constexpr int	blocksH		= 20u;
	static constexpr int	levelW		= 37u;
	static constexpr int	levelH		= 7u;
	static constexpr int	scoreW		= 200u;
	static constexpr int	scoreH		= 50u;
	static constexpr int	digitW		= 50u;
	static constexpr int	digitH		= 50u;
	static constexpr int	pauseW		= 580u;
	static constexpr int	pauseH		= 100u;
	static constexpr int	gameOverW	= 400u;
	static constexpr int	gameOverH	= 190u;
	static constexpr int	rows		= 10u;
	static constexpr int	cols		= 10u;

	const int scrW = gfx.ScreenWidth;
	const int scrH = gfx.ScreenHeight;
	const int offsetWidth	= (scrW / 2) - ((fieldW / 2) * blocksW);
	const int offsetHeight	= (scrH / 2) - ((fieldH / 2) * blocksH);	

	Surface* tex_Black		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_DarkGrey.png"));
	Surface* tex_Orange		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Orange.png"));
	Surface* tex_Cyan		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Cyan.png"));
	Surface* tex_Green		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Green.png"));
	Surface* tex_Red		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Red.png"));
	Surface* tex_Blue		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Blue.png"));
	Surface* tex_Magenta	= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Magenta.png"));
	Surface* tex_Yellow		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Yellow.png"));
	Surface* tex_Grey		= new Surface(Surface::FromFile(L"Textures\\Blocks\\Block_Grey.png"));
	
	Surface* tex_digit_0	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 0.png"));
	Surface* tex_digit_1	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 1.png"));
	Surface* tex_digit_2	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 2.png"));
	Surface* tex_digit_3	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 3.png"));
	Surface* tex_digit_4	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 4.png"));
	Surface* tex_digit_5	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 5.png"));
	Surface* tex_digit_6	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 6.png"));
	Surface* tex_digit_7	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 7.png"));
	Surface* tex_digit_8	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 8.png"));
	Surface* tex_digit_9	= new Surface(Surface::FromFile(L"Textures\\Digits\\Digit - 9.png"));

	std::vector<std::wstring> file_Background;
	Surface* tex_Background;// = new Surface(Surface::FromFile(L"Textures\\Backgrounds\\Nature.jpg"));
	Surface* tex_Pause		= new Surface(Surface::FromFile(L"Textures\\Words\\word - Pause.png"));
	Surface* tex_GameOver	= new Surface(Surface::FromFile(L"Textures\\Words\\word - GameOver.png"));	

	std::array<Surface*, 10> block_Textures = {		tex_Black,
													tex_Orange,
													tex_Cyan,
													tex_Green,
													tex_Red,
													tex_Blue,
													tex_Magenta,
													tex_Yellow,
													tex_Red,
													tex_Grey };

	std::array<Surface*, 10> number_Textures = {	tex_digit_0,
													tex_digit_1,
													tex_digit_2,
													tex_digit_3,
													tex_digit_4,
													tex_digit_5,
													tex_digit_6,
													tex_digit_7,
													tex_digit_8,
													tex_digit_9 };
													

	
	std::array<std::array<Block, cols>, rows> blocks_Digits;

	std::string			tetromino[7];
	std::string			text_Level;

	std::vector<int>	lines;

	std::vector<Block>	blocks;
	std::vector<Block>	blocks_Next;

	Block block_Background;
	Block block_Level;
	Block block_Score;
	Block block_Pause;
	Block block_GameOver;
		 
	std::vector<char>			blockBuffer_Fixed;
	std::vector<char>			blockBuffer_Shown;
	std::vector<unsigned int>	blockBuffer_Score;

	unsigned int	speed				= 20;
	unsigned int	score				= 0;
	unsigned int	level				= 0;
	unsigned int	tetrominoCounter	= 0;
	unsigned int	speedCounter		= 0;
	
	int	tetrominoNext		= 0;
	int	tetrominoCurrent	= 0;
	int	currentRotation		= 0;
	int	currentX			= fieldW / 2 - 2;
	int	currentY			= 0;

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
	void	InitialiseScore();
	void	InitialisePause();
	void	InitialiseGameOver();

	void	SetFieldBuffer();
	void	SetFieldBlocks();
	void	SetNextTetromino();
	void	SetScore();

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetromino();
	void	DrawBlur();
	void	DrawScore();
	void	DrawTextPause();
	void	DrawTextGameOver();

private:
	int		Random(const int min, const int max);
	int		Rotate(int px, int py, int r);
	bool	DoesTetrisFit(int tetrisID, int rotation, int posX, int posY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	unsigned int	ConvertCharToInt(const char value);
	void	Benchmark(void* pFunction);

	const std::vector<Color> Blur(
		const int width,
		const int height,
		const std::vector<Color>& input);
	void BoxBlur(const Surface& input, std::vector<Color>& output);

	/*auto boxesForGauss(int sigma, int n);
	void gaussBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurH_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurT_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void gaussBlur_1(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
};

