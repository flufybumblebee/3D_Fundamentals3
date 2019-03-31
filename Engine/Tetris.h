#pragma once

#include <string>
#include <array>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <fstream>
#include <cmath>

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
	typedef unsigned int uint;
	
	static constexpr uint	fieldW		= 12u;
	static constexpr uint	fieldH		= 18u;
	static constexpr uint	tetroW		= 4u;
	static constexpr uint	tetroH		= 4u;
	static constexpr uint	bloW		= 7u;
	static constexpr uint	bloH		= 7u;
	static constexpr uint	blocksW		= 20u;
	static constexpr uint	blocksH		= 20u;
	static constexpr uint	levelW		= 37u;
	static constexpr uint	levelH		= 7u;
	static constexpr uint	scoreW		= 37u;
	static constexpr uint	scoreH		= 7u;
	static constexpr uint	digitW		= 6u;
	static constexpr uint	digitH		= 7u;
	static constexpr uint	pauseW		= 35u;
	static constexpr uint	pauseH		= 7u;
	static constexpr uint	gameOverW	= 29u;
	static constexpr uint	gameOverH	= 13u;

	const uint offsetWidth	= (gfx.ScreenWidth  / 2) - ((fieldW / 2) * blocksW);
	const uint offsetHeight	= (gfx.ScreenHeight / 2) - ((fieldH / 2) * blocksH);
	
	Surface	tex_background	= Surface::FromFile(L"Textures\\Backgrounds\\Street.bmp");

	Surface tex_tileBlack	= Surface::FromFile(L"Textures\\Blocks\\TileBlack.bmp");
	Surface tex_tileOrange	= Surface::FromFile(L"Textures\\Blocks\\TileOrange.bmp");
	Surface tex_tileCyan	= Surface::FromFile(L"Textures\\Blocks\\TileCyan.bmp");
	Surface tex_tileGreen	= Surface::FromFile(L"Textures\\Blocks\\TileGreen.bmp");
	Surface tex_tileRed		= Surface::FromFile(L"Textures\\Blocks\\TileRed.bmp");
	Surface tex_tileBlue	= Surface::FromFile(L"Textures\\Blocks\\TileBlue.bmp");
	Surface tex_tileMagenta = Surface::FromFile(L"Textures\\Blocks\\TileMagenta.bmp");
	Surface tex_tileYellow	= Surface::FromFile(L"Textures\\Blocks\\TileYellow.bmp");
	Surface tex_tileGrey	= Surface::FromFile(L"Textures\\Blocks\\TileGrey.bmp");

	Surface tex_tileZeroG	= Surface::FromFile(L"Textures\\Digits\\tileZeroG.bmp");
	Surface tex_tileOneG	= Surface::FromFile(L"Textures\\Digits\\tileOneG.bmp");
	Surface tex_tileTwoG	= Surface::FromFile(L"Textures\\Digits\\tileTwoG.bmp");
	Surface tex_tileThreeG	= Surface::FromFile(L"Textures\\Digits\\tileThreeG.bmp");
	Surface tex_tileFourG	= Surface::FromFile(L"Textures\\Digits\\tileFourG.bmp");
	Surface tex_tileFiveG	= Surface::FromFile(L"Textures\\Digits\\tileFiveG.bmp");
	Surface tex_tileSixG	= Surface::FromFile(L"Textures\\Digits\\tileSixG.bmp");
	Surface tex_tileSevenG	= Surface::FromFile(L"Textures\\Digits\\tileSevenG.bmp");
	Surface tex_tileEightG	= Surface::FromFile(L"Textures\\Digits\\tileEightG.bmp");
	Surface tex_tileNineG	= Surface::FromFile(L"Textures\\Digits\\tileNineG.bmp");

	Block background;

	std::array<Color, 10>		block_Colors;

	const std::array<Surface*, 10>	block_Textures = {
														&tex_tileBlack,
														&tex_tileOrange,
														&tex_tileCyan,
														&tex_tileGreen,
														&tex_tileRed,
														&tex_tileBlue,
														&tex_tileMagenta,
														&tex_tileYellow,
														&tex_tileRed,
														&tex_tileGrey };

	const std::array<Surface*, 10>	digit_Textures = {
														&tex_tileZeroG,
														&tex_tileOneG,
														&tex_tileTwoG,
														&tex_tileThreeG,
														&tex_tileFourG,
														&tex_tileFiveG,
														&tex_tileSixG,
														&tex_tileSevenG,
														&tex_tileEightG,
														&tex_tileNineG };

	

	static constexpr uint rows = 10u;
	static constexpr uint cols = 10u;

	//std::array<std::array<Block,digitW*digitH>,10>	blocks_Digits;
	//Block blocks_Digits[rows][cols];

	std::array<std::array<Block, cols>, rows> blocks_Digits;

	std::string			tetromino[7];
	std::string			text_Digits[10];
	std::string			text_Level;
	std::string			text_Score;
	std::string			text_Pause;
	std::string			text_GameOver;

	std::vector<int>	lines;

	std::vector<Block>	blocks;
	std::vector<Block>	blocks_Next;
	//std::array<Block,>	blocks_Text_Number;
	std::array<Block, levelW*levelH>		blocks_Text_Level;
	std::array<Block, scoreW*scoreH>		blocks_Text_Score;
	std::array<Block, pauseW*pauseH>		blocks_Text_Pause;
	std::array<Block, gameOverW*gameOverH>	blocks_Text_GameOver;
		 
	std::vector<char>	blockBuffer_Fixed;
	std::vector<char>	blockBuffer_Shown;
	std::vector<uint>	blockBuffer_Digits;

	uint	speed				= 20;
	uint	score				= 0;
	uint	level				= 0;
	uint	tetrominoCounter	= 0;
	uint	speedCounter		= 0;

	uint	tetrominoNext		= 0;
	uint	tetrominoCurrent	= 0;
	uint	currentRotation		= 0;
	uint	currentX			= fieldW / 2 - 2;
	uint	currentY			= 0;

	bool	keyIsPressed		= false;
	bool	spaceIsPressed		= false;
	bool	forceDown			= false;
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
	void	UpdateDigits(std::array<Block,digitW*digitH>& blocks_Digit, const std::string& text_Digit);
	void	ResetLevel();
	void	ResetScore();
	void	ResetField();

	void	DrawBackground();
	void	DrawField();
	void	DrawNextTetromino();
	void	DrawTextScore();
	void	DrawLevel();
	void	DrawTextDigits();
	void	DrawTextPause();
	void	DrawTextGameOver();

	void	DrawBlur();

private:
	int		Random(const int min, const int max);
	int		Rotate(int px, int py, int r);
	bool	DoesTetrisFit(int tetrisID, int rotation, int posX, int posY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	uint	ConvertCharToInt(const char value);
	void	Benchmark(void* pFunction);

	void Blur(const Surface& input, std::vector<Color>& output);
};

