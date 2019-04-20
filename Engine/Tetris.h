#pragma once

#include <string>
#include <array>
#include <vector>
#include <chrono>

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
	static constexpr unsigned int	tetroW		= 4u;
	static constexpr unsigned int	tetroH		= 4u;
	static constexpr unsigned int	fieldW		= 12u;
	static constexpr unsigned int	fieldH		= 18u;
	static constexpr unsigned int	blockW		= 25u;
	static constexpr unsigned int	blockH		= 25u;
	static constexpr unsigned int	digitW		= 50u;
	static constexpr unsigned int	digitH		= 50u;
	static constexpr unsigned int	pauseW		= 580u;
	static constexpr unsigned int	pauseH		= 100u;
	static constexpr unsigned int	gameOverW	= 400u;
	static constexpr unsigned int	gameOverH	= 190u;
	static constexpr unsigned int	rows		= 10u;
	static constexpr unsigned int	cols		= 10u;
	static constexpr unsigned int	blurLevel	= 7u;

	const int scrW = gfx.ScreenWidth;
	const int scrH = gfx.ScreenHeight;

	std::vector<Surface> texture_Background;
	std::vector<Surface> texture_Blocks;
	std::vector<Surface> texture_Digits;
	std::vector<Surface> texture_Pause;
	std::vector<Surface> texture_GameOver;

	std::array<std::array<Block, fieldW>, fieldH>	blocks;
	std::array<std::array<Block, cols>, rows>		blocks_Score;
	std::array<Block, 10>							blocks_Counter;

	std::string			tetromino[7];

	std::vector<int>	lines;
	std::vector<Block>	blocks_Next;
	
	Block block_Background;
	Block block_Pause;
	Block block_GameOver;
		 
	std::vector<char>			blockBuffer_Fixed;
	std::vector<char>			blockBuffer_Shown;
	std::vector<unsigned int>	blockBuffer_Score;

	unsigned int	speed				= 0;
	unsigned int	score				= 0;
	unsigned int	level				= 0;
	unsigned int	tetrominoCounter	= 0;
	unsigned int	speedCounter		= 0;
	unsigned int	tickCounter			= 0;
	size_t			sizeBG				= 0;
	size_t			randomBG			= 0;
	int				prevLevel			= 0;

	//std::ofstream file;

	float	frameTime	= 0;
	float	counter0	= 0;
	float	counter1	= 0;
	float	counter2	= 0;
	std::chrono::high_resolution_clock::time_point t0;
	static constexpr float tickTime = 100000.0f;

	int		nextTetro		= 0;
	int		currentTetro	= 0;
	int		currentRotation	= 0;
	int		currentX		= 0;
	int		currentY		= 0;

	bool	isFirstFrame		= true;
	bool	tick				= false;
	bool	keyIsPressed_UP		= false;
	bool	keyIsPressed_DOWN	= false;
	bool	keyIsPressed_LEFT	= false;
	bool	keyIsPressed_RIGHT	= false;
	bool	keyIsPressed_SPACE	= false;

	bool	gameIsPaused		= false;
	bool	gameIsOver			= false;
	
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
	void	InitialiseBlocks();
	void	InitialiseTetrominos();
	void	InitialiseDigits();
	void	InitialiseScore();
	void	InitialisePause();
	void	InitialiseGameOver();
	void	InitialiseCounter();

	void	SetBackground();
	void	ClearScore();
	void	ResetFieldBuffers();
	void	SetBlockTextures();
	void	SetNextTetromino();
	void	SetScore();
	void	SetLevel();
	void	SetCounter();
	void	CheckForLines();
	void	AddTetroToFixedBuffer();
	void	AddTetroToShownBuffer();
	void	AddFixedToShownBuffer();
	void	PauseOrReset();

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetromino();
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
	std::vector<Color> Blur(
		const int width,
		const int height,
		const std::vector<Color>& input);

	template <typename T>
	T RandomInt(T min, T max)
	{
		std::mt19937 rng;
		rng.seed(std::random_device()());
		std::uniform_int_distribution<T> dist(min, max);

		return dist(rng);
	}

	/*void	Benchmark(void* pFunction);
	void BoxBlur(const Surface& input, std::vector<Color>& output);
	auto boxesForGauss(int sigma, int n);
	void gaussBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlur_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurH_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void boxBlurT_4(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	void gaussBlur_1(std::vector<unsigned char> scl, std::vector<unsigned char> tcl, int w, int h, int r);
	std::vector<Color> FastBlur(const std::vector<Color>& input, const int w, const int h, const int radius);
	Surface FastBlur(const Surface& input, int radius);

	void superFastBlur(unsigned char* pix, int w, int h, int radius);*/
};

