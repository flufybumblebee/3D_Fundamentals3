#pragma once

#include <string>
#include <array>
#include <vector>

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
	static constexpr int	blockW		= 25u;
	static constexpr int	blockH		= 25u;
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
	const int offsetWidth	= (scrW / 2) - ((fieldW / 2) * blockW);
	const int offsetHeight	= (scrH / 2) - ((fieldH / 2) * blockH);	

	std::vector<Surface> tex_Background;
	std::vector<Surface> tex_Blocks;
	std::vector<Surface> tex_Digits;
	std::vector<Surface> tex_Pause;
	std::vector<Surface> tex_GameOver;

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
	size_t			sizeBG				= 0;
	size_t			randomBG			= 0;
	int				prevLevel			= 0;

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
	void	InitialiseBlocks();
	void	InitialiseTetrominos();
	void	InitialiseScore();
	void	InitialisePause();
	void	InitialiseGameOver();

	void	ClearScore();
	void	SetFieldBuffer();
	void	SetFieldBlocks();
	void	SetNextTetromino();
	void	SetScore();
	void	SetLevel();

	void	SetBackground();

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetromino();
	void	DrawBlur();
	void	DrawScore();
	void	DrawTextPause();
	void	DrawTextGameOver();

private:
	int		Rotate(int px, int py, int r);
	bool	DoesTetrisFit(int tetrisID, int rotation, int posX, int posY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	int		ConvertCharToInt(const char value);

	void	Benchmark(void* pFunction);
	std::vector<Color> ConvertSurfaceToColorVector(Surface surface);
	std::vector<Color> Blur(
		const int width,
		const int height,
		const std::vector<Color>& input);
	void BoxBlur(const Surface& input, std::vector<Color>& output);

	template <typename T>
	T Random(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<T> dist(min, max);

		return dist(rng);
	}

	/*auto boxesForGauss(int sigma, int n);
	void gaussBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurH_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurT_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void gaussBlur_1(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);*/
};

