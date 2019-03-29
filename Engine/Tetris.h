#pragma once

#include <string>
#include <array>
#include <vector>
#include <thread>
#include <random>
#include <chrono>

#include "Block.h"
#include "Keyboard.h"

typedef unsigned int uint;

class Tetris
{
private:
	Keyboard& kbd;
	Graphics& gfx;

private:
	static constexpr uint	fieldW		= 12u;
	static constexpr uint	fieldH		= 18u;
	static constexpr uint	tetroW		= 4u;
	static constexpr uint	tetroH		= 4u;
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

	const uint offsetWidth	= (gfx.ScreenWidth / 2) - ((fieldW / 2)*blocksW);
	const uint offsetHeight	= (gfx.ScreenHeight / 2) - ((fieldH / 2)*blocksH);

	Surface	background	= Surface::FromFile(L"Textures\\Blue.jpg");

	Surface tileBlack	= Surface::FromFile(L"Textures\\TileBlack.bmp");
	Surface tileOrange	= Surface::FromFile(L"Textures\\TileOrange.bmp");
	Surface tileCyan	= Surface::FromFile(L"Textures\\TileCyan.bmp");
	Surface tileGreen	= Surface::FromFile(L"Textures\\TileGreen.bmp");
	Surface tileRed		= Surface::FromFile(L"Textures\\TileRed.bmp");
	Surface tileBlue	= Surface::FromFile(L"Textures\\TileBlue.bmp");
	Surface tileMagenta = Surface::FromFile(L"Textures\\TileMagenta.bmp");
	Surface tileYellow	= Surface::FromFile(L"Textures\\TileYellow.bmp");
	Surface tileGrey	= Surface::FromFile(L"Textures\\TileGrey.bmp");

	std::array<Color, 10>		block_Colors;
	std::array<Surface*, 10>	block_Textures;
	std::array<std::array<Block,digitW*digitH>,10>	blocks_Digits;

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

	// testing
	uint	count				= 0;

public:
	Tetris(Keyboard& kbd, Graphics& gfx);
	~Tetris();

public:
	void	Setup();
	void	Update();
	void	Draw();

private:
	void	InitialiseTetrominos();
	void	InitialiseDigits();

	void	InitialiseTextScore();
	void	InitialiseTextLevel();
	void	InitialiseTextPause();
	void	InitialiseTextGameOver();

	void	SetFieldBuffer();
	void	SetFieldBlocks();

	void	SetNextTetromino();
	void	InitialiseTextDigits();
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

private:
	int		Random(const int min, const int max);
	int		Rotate(int px, int py, int r);
	bool	DoesTetrisFit(int tetrisID, int rotation, int posX, int posY);
	void	ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num);
	Color	ConvertCharToColor(const char value);
	uint	ConvertCharToInt(const char value);
};

