#include "Tetris.h"

#include <thread>
#include <random>
#include <fstream>
#include <cmath>
#include <cassert>
#include <utility>

typedef unsigned int uint;
typedef unsigned char uchar;

typedef std::chrono::steady_clock Time;
typedef std::chrono::microseconds ms;

Tetris::Tetris(Keyboard& kbd, Graphics& gfx)
	:
	kbd(kbd),
	gfx(gfx)
{
	InitialiseBackground();
	InitialiseBlocks();
	InitialiseTetrominos();
	InitialiseDigits();
	InitialiseCounter();
	InitialiseScore();
	InitialisePause();
	InitialiseGameOver();

	//file.open("Benchmark.txt");
}
Tetris::~Tetris()
{
	/*if (file.is_open())
	{
		file.close();
	}*/
}

/*-------------------------------------------*/

void Tetris::Setup()
{	
	gameIsOver			= false;
	tetrominoNext		= Random(0, 6);
	tetrominoCurrent	= Random(0, 6);
	currentX			= fieldW / 2 - 2;
	currentY			= 0;
	currentRotation		= 0;
	speed				= 20;
	level				= 0;
	tetrominoCounter	= 0;
	speedCounter		= 0;

	ClearScore();	
	SetBackground();
	SetFieldBuffer();
	SetFieldBlocks();
	SetNextTetromino();
}
void Tetris::Update()
{
	if (isFirstRun)
	{
		t0 = Time::now();
		isFirstRun = false;
	}

	auto t1 = Time::now();
	auto duration = std::chrono::duration<float,std::micro>(t1 - t0);
	t0 = Time::now();

	//file << "duration:    " << duration.count() << " microseconds" << std::endl;
	//file << "frame timer: " << frameTime << " microseconds" << std::endl;

	if (frameTime < tickTime)
	{
		frameTime += duration.count();
		tick = false;
	}
	else
	{
		counter0 += tickTime;
		counter1 += tickTime;
		counter2 += tickTime;
		SetCounter();
		frameTime = frameTime - tickTime;
		tick = true;
	}

	if (!keyIsPressed_SPACE)
	{
		if (kbd.KeyIsPressed(VK_SPACE) && !gameIsPaused)
		{
			if (gameIsOver)
			{
				Setup();
			} 
			else
			{
				gameIsPaused = true;
			}
			keyIsPressed_SPACE = true;
		}
		else if (kbd.KeyIsPressed(VK_SPACE) && gameIsPaused)
		{
			gameIsPaused = false;
			keyIsPressed_SPACE = true;
		}
	}
	else
	{
		if (!kbd.KeyIsPressed(VK_SPACE))
		{
			keyIsPressed_SPACE = false;
		}
	}
		
	if (!gameIsPaused)
	{
		if (!gameIsOver)
		{	
			if (tick)
			{
				currentX -= (kbd.KeyIsPressed(VK_LEFT) && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX - 1, currentY + 0)) ? 1 : 0;
				currentX += (kbd.KeyIsPressed(VK_RIGHT) && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX + 1, currentY + 0)) ? 1 : 0;
				currentY += (kbd.KeyIsPressed(VK_DOWN) && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX + 0, currentY + 1)) ? 1 : 0;
				/*if (kbd.KeyIsPressed(VK_LEFT))
				{
					currentX -= (!keyIsPressed_LEFT && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX - 1, currentY + 0)) ? 1 : 0;
					keyIsPressed_LEFT = true;
				}
				else
				{
					keyIsPressed_LEFT = false;
				}

				if (kbd.KeyIsPressed(VK_RIGHT))
				{
					currentX += (!keyIsPressed_RIGHT && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX + 1, currentY + 0)) ? 1 : 0;
					keyIsPressed_RIGHT = true;
				}
				else
				{
					keyIsPressed_RIGHT = false;
				}
				
				if (kbd.KeyIsPressed(VK_DOWN))
				{
					currentY += (!keyIsPressed_DOWN && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX + 0, currentY + 1)) ? 1 : 0;
					keyIsPressed_DOWN = true;
				}
				else
				{
					keyIsPressed_DOWN = false;
				}*/
				
				if (kbd.KeyIsPressed(VK_UP))
				{
					currentRotation += (!keyIsPressed_UP && DoesTetrisFit(tetrominoCurrent, currentRotation + 1, currentX, currentY)) ? 1 : 0;
					keyIsPressed_UP = true;
				}
				else
				{
					keyIsPressed_UP = false;
				}

				counter0 = 0;
			}

			if (tick)
			{
				if (DoesTetrisFit(tetrominoCurrent, currentRotation, currentX, currentY + 1))
				{
					currentY++; // force tetris down
				}
				else
				{
					// add tetris to fixed field buffer
					for (int y = 0; y < tetroH; y++)
					{
						for (int x = 0; x < tetroW; x++)
						{
							if (tetromino[tetrominoCurrent][Rotate(x, y, currentRotation)] == 'X')
							{
								uint i = (currentY + y) * fieldW + (currentX + x);
								blockBuffer_Fixed[i] = tetrominoCurrent + 1;
							}
						}
					}

					// check for lines
					for (int y = 0; y < 4; y++)
					{
						if (currentY + y < fieldH - 1)
						{
							bool isLine = true;
							for (int x = 1; x < fieldW - 1; x++)
							{
								uint i = (currentY + y) * fieldW + x;
								isLine &= (blockBuffer_Fixed[i]) != 0;
							}

							if (isLine)
							{
								// remove line
								for (int x = 1; x < fieldW - 1; x++)
								{
									uint i = (currentY + y) * fieldW + x;
									blockBuffer_Fixed[i] = 8;
								}
								lines.push_back(currentY + y);
							}
						}
					}

					// calculate score
					SetScore();

					// randomly assign new tetris

					currentX = fieldW / 2 - 2;
					currentY = 0;
					currentRotation = 0;

					tetrominoCurrent = tetrominoNext;
					tetrominoNext = Random(0, 6);

					SetNextTetromino();
					// if tetris doesn't fit...
					gameIsOver = !DoesTetrisFit(tetrominoCurrent, currentRotation, currentX, currentY);
				}
				
				counter1 = 0.0f;
			}

			// add all fixed blocks to displayBuffer from fixedBuffer
			for (int y = 0; y < fieldH; y++)
			{
				for (int x = 0; x < fieldW; x++)
				{
					uint i = y * fieldW + x;
					blockBuffer_Shown[i] = " ABCDEFG=#"[blockBuffer_Fixed[i]];
				}
			}

			// add current tetris piece to display buffer
			for (int y = 0; y < tetroH; y++)
			{
				for (int x = 0; x < tetroW; x++)
				{
					// set to the ascii table value
					if (tetromino[tetrominoCurrent][Rotate(x, y, currentRotation)] == 'X')
					{
						uint i = (currentY + y) * fieldW + (currentX + x);
						blockBuffer_Shown[i] = " ABCDEFG=#"[tetrominoCurrent + 1];
					}
				}
			}

			// Animate Line Completion
			if (!lines.empty())
			{
				//std::this_thread::sleep_for(std::chrono::milliseconds(800)); // delay

				for (auto& v : lines)
				{
					for (int x = 1; x < fieldW - 1; x++)
					{
						for (int y = v; y > 0; y--)
						{
							uint i = y * fieldW + x;
							uint j = (y - 1) * fieldW + x;
							blockBuffer_Fixed[i] = blockBuffer_Fixed[j];
						}
						blockBuffer_Fixed[x] = 0;
					}
				}

				lines.clear();
			}

			SetFieldBlocks();
		}		
	}
}
void Tetris::Draw()
{
	DrawBackground();
	DrawField();
	DrawNextTetromino();
	DrawBlur();
	DrawTextPause();
	DrawTextGameOver();
	DrawScore();
	DrawCounter();
}

/*-------------------------------------------*/

void Tetris::InitialiseBackground()
{
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksBlue.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksGreen.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksRainbow.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature0.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature2.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature5.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Street0.bmp"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Space1.jpg"));

	sizeBG = texture_Background.size();

	RectI rect = { 0,static_cast<int>(scrH),0,static_cast<int>(scrW) };

	size_t min = static_cast<size_t>(0);
	size_t max = static_cast<size_t>(texture_Background.size() - 1);
	randomBG = static_cast<size_t>(Random(min, max));
	block_Background = Block(rect, &texture_Background[randomBG]);
}
void Tetris::InitialiseBlocks()
{
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_DarkGrey.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Orange.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Cyan.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Green.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Red.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Blue.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Magenta.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Yellow.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Red.png"));
	texture_Blocks.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Grey.png"));
}
void Tetris::InitialiseTetrominos()
{
	tetromino[0].append("..X.");
	tetromino[0].append("..X.");
	tetromino[0].append("..X.");
	tetromino[0].append("..X.");

	tetromino[1].append("..X.");
	tetromino[1].append(".XX.");
	tetromino[1].append(".X..");
	tetromino[1].append("....");

	tetromino[2].append(".X..");
	tetromino[2].append(".XX.");
	tetromino[2].append("..X.");
	tetromino[2].append("....");

	tetromino[3].append("....");
	tetromino[3].append(".XX.");
	tetromino[3].append(".XX.");
	tetromino[3].append("....");

	tetromino[4].append("..X.");
	tetromino[4].append(".XX.");
	tetromino[4].append("..X.");
	tetromino[4].append("....");

	tetromino[5].append(".XX.");
	tetromino[5].append("..X.");
	tetromino[5].append("..X.");
	tetromino[5].append("....");

	tetromino[6].append(".XX.");
	tetromino[6].append(".X..");
	tetromino[6].append(".X..");
	tetromino[6].append("....");
}
void Tetris::InitialiseDigits()
{	
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 0.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 1.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 2.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 3.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 4.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 5.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 6.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 7.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 8.png"));
	texture_Digits.push_back(Surface::FromFile(L"Textures\\Digits\\Digit - 9.png"));
}
void Tetris::InitialiseScore()
{	
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			RectI position = RectI(
				(digitH / 5),
				(digitH / 5) + digitH,
				(scrW - 1) - (digitW * (x)) - (digitW / 5 * (x + 1)) - digitW,
				(scrW - 1) - (digitW * (x)) - (digitW / 5 * (x + 1)));

			blocks_Score[y][x] = Block(position, &texture_Digits[y]);
		}
	}	
}
void Tetris::InitialisePause()
{
	texture_Pause.push_back(Surface::FromFile(L"Textures\\Words\\word - Pause.png"));

	RectI rect = RectI(
		(scrH / 2) - (pauseH / 2),
		(scrH / 2) + (pauseH / 2),
		(scrW / 2) - (pauseW / 2),
		(scrW / 2) + (pauseW / 2));

	size_t size = texture_Pause.size();
	size_t i = (size > 1) ? Random(static_cast<size_t>(0), size - 1) : 0;
	block_Pause = Block(rect, &texture_Pause[i]);
}
void Tetris::InitialiseGameOver()
{	
	texture_GameOver.push_back(Surface::FromFile(L"Textures\\Words\\word - GameOver.png"));

	RectI rect = RectI(
		(scrH / 2) - (gameOverH / 2),
		(scrH / 2) + (gameOverH / 2),
		(scrW / 2) - (gameOverW / 2),
		(scrW / 2) + (gameOverW / 2));

	size_t size = texture_GameOver.size();
	size_t i = (size > 1) ? Random(static_cast<size_t>(0), size - 1) : 0;
	block_GameOver = Block(rect, &texture_GameOver[i]);
}
void Tetris::InitialiseCounter()
{
	for (int i = 0; i < 10; i++)
	{
			RectI position = RectI( 
				(scrH / 2) - (digitH / 2),
				(scrH / 2) + (digitH / 2),
				(scrW / 2) - ((fieldW/2) * blockW) - (digitW * 3),
				(scrW / 2) - ((fieldW / 2) * blockW) - (digitW * 2));

			blocks_Counter[i] = Block(position, &texture_Digits[i]);	
	}
}

/*-------------------------------------------*/

void Tetris::SetBackground()
{
	randomBG = (randomBG >= texture_Background.size()) ? 0 : randomBG++;

	block_Background.SetTexture( &texture_Background[randomBG] );
}
void Tetris::ClearScore()
{
	blockBuffer_Score.clear();
	score = 0;
	ExtractDigits(blockBuffer_Score, score);
}
void Tetris::SetFieldBuffer()
{
	blockBuffer_Fixed.clear();
	blockBuffer_Shown.clear();

	// set playing field border
	for (int y = 0; y < fieldH; y++)	
	{
		for (int x = 0; x < fieldW; x++)
		{
			blockBuffer_Fixed.push_back((x == 0 || x == fieldW - 1 || y == fieldH - 1) ? 9 : 0);

			//if (x == 0 || x == fieldW - 1) field[y*fieldW + x] = 9;
			//else if(y == fieldH - 1) field[y*fieldW + x] = 9;
			//else field[y*fieldW + x] = 0;
		}
	}

	// set shown blocks buffer with fixed block buffer
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)		
		{
			uint i = y * fieldW + x;
			blockBuffer_Shown.push_back(" ABCDEFG=#"[blockBuffer_Fixed[i]]);
		}
	}
}
void Tetris::SetFieldBlocks()
{
	blocks.clear();

	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			RectI pos = RectI(
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH),
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH) + blockH,
				(scrW / 2) - ((fieldW / 2) * blockW) + (x * blockW),
				(scrW / 2) - ((fieldW / 2) * blockW) + (x * blockW) + blockW);
			int i = y * fieldW + x;
			int j = ConvertCharToInt(blockBuffer_Shown[i]);

			blocks.push_back(Block(pos, &texture_Blocks[j]));
		}
	}
}
void Tetris::SetNextTetromino()
{
	blocks_Next.clear();

	for (int y = 0; y < tetroH; y++)
	{
		for (int x = 0; x < tetroW; x++)
		{			
			RectI rect = RectI(
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH),
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH) + blockH,
				(scrW / 2) - ((fieldW / 2) * blockW) + (blockW * fieldW) + (x * blockW),
				(scrW / 2) - ((fieldW / 2) * blockW) + (blockW * fieldW) + (x * blockW) + blockW);
			int i = y * tetroW + x;
			int j = (tetromino[tetrominoNext][i] != '.') ? tetrominoNext + 1 : 0;

			blocks_Next.push_back(Block(rect,&texture_Blocks[j]));
		}
	}
}
void Tetris::SetScore()
{
	score += 25;
	if (!lines.empty()) score += (1 << lines.size()) * 100;
	ExtractDigits(blockBuffer_Score, score);	
}
void Tetris::SetLevel()
{
	prevLevel = level;
	level++;
}
void Tetris::SetCounter()
{
	if (tickCounter < 9)
	{
		tickCounter++;
	}
	else
	{
		tickCounter = 0;
	}
}

/*-------------------------------------------*/

void Tetris::DrawBackground()
{
	block_Background.Draw(gfx);
}
void Tetris::DrawField()
{
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			uint i = y * fieldW + x;
			if (blockBuffer_Shown[i] == ' ')
			{
				continue;
			}
			else
			{
				blocks[i].Draw(gfx);
			}
		}
	}
}
void Tetris::DrawNextTetromino()
{
	for (int y = 0; y < tetroH; y++)
	{
		for (uint x = 0; x < tetroW; x++)
		{
			uint i = y * tetroW + x;
			if (tetromino[tetrominoNext][i] == '.')
			{
				continue;
			}
			else
			{
				blocks_Next[i].Draw(gfx);
			}
		}
	}
}
void Tetris::DrawScore()
{
	for (size_t i = 0; i < blockBuffer_Score.size(); i++)
	{
		blocks_Score[blockBuffer_Score[i]][i].Draw(gfx);
	}
}
void Tetris::DrawTextPause()
{
	if (gameIsPaused)
	{		
		block_Pause.Draw(gfx);
	}
}
void Tetris::DrawTextGameOver()
{
	if (gameIsOver)
	{		
		block_GameOver.Draw(gfx);
	}
}
void Tetris::DrawCounter()
{
	if (!gameIsOver)
	{
		if (!gameIsPaused)
		{
			blocks_Counter[tickCounter].Draw(gfx);
		}
	}
}
void Tetris::DrawBlur()
{
	if (gameIsPaused || gameIsOver) 
	{
		std::vector<Color> input;
		input = ConvertSurfaceToColorVector(gfx.CopySysBuffer());

		int w = scrW;
		int h = scrH;
		size_t wh = static_cast<size_t>(w) * h;

		std::vector<Color> output(wh);
		
		for (int i = 0; i < blurLevel; i++)
		{
			input = Blur(w, h, input);
		}
		output = input;

		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				size_t i = static_cast<size_t>(y) * w + x;
				gfx.PutPixel(x, y, output[i]);
			}
		}			
	}
}

/*-------------------------------------------*/

int		Tetris::Rotate(int px, int py, int r)
{
	int pi = 0;

	switch (r % 4)
	{
	case 0: //   0 degrees 
		pi = 0 + (py * 4) + px;			//  0  1  2  3
		break;							//  4  5  6  7
										//  8  9 10 11
										// 12 13 14 15

	case 1: //  90 degrees
		pi = 12 + py - (px * 4);		// 12  8  4  0
		break;							// 13  9  5  1
										// 14 10  6  2
										// 15 11  7  3

	case 2: // 120 degrees
		pi = 15 - (py * 4) - px;		// 15 14 13 12
		break;							// 11 10  9  8
										//  7  6  5  4
										//  3  2  1  0

	case 3: // 270 degrees
		pi = 3 - py + (px * 4);			//  3  7 11 15
		break;							//  2  6 10 14
										//  1  5  9 13
										//  0  4  8 12
	}

	return pi;
}
bool	Tetris::DoesTetrisFit(int tetrisID, int rotation, int posX, int posY)
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			// get index into tetris
			int pi = Rotate(px, py, rotation);

			// get index into field
			int fi = (posY + py) * fieldW + (posX + px);

			if (posX + px >= 0 && posX + px < fieldW &&
				posY + py >= 0 && posY + py < fieldH)
			{
				if (tetromino[tetrisID][pi] != L'.' && blockBuffer_Fixed[fi] != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}
void	Tetris::ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num)
{
	ints.clear();
	unsigned int result = num;

	if (result > 0)
	{
		while (result > 0)
		{
			ints.push_back(result % 10);
			result /= 10;
		}
	}
	else
	{
		ints.push_back(0);
	}
}
Color	Tetris::ConvertCharToColor(const char value)
{
	/*Color color;
	for (int i = 0; i < 10; i++)
	{
		if (value == " ABCDEFG=#"[i])
		{
			color = block_Colors[i];
			return color;
		}
		else
		{
			continue;
		}
	}*/

	return Colors::Black;
}
int		Tetris::ConvertCharToInt(const char value)
{
	for (int i = 0; i < 10; i++)
	{
		if (value == " ABCDEFG=#"[i])
		{
			return i;
		}
		else
		{
			continue;
		}
	}

	return 0;
}

/*-------------------------------------------*/

std::vector<Color> Tetris::ConvertSurfaceToColorVector(Surface surface)
{
	int w = surface.GetWidth();
	int h = surface.GetHeight();

	std::vector<Color> output;

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			output.push_back(surface.GetPixel(x, y));
		}
	}

	return output;
}
std::vector<Color> Tetris::Blur(const int w,const int h,const std::vector<Color>& input)
{
	const size_t sizeA = size_t(w) * h;
	const size_t sizeB = input.size();
	assert( sizeA == sizeB );

	Mat3 m;
	
	const float val = 1.0f / 9.0f;

	m.elements[0][0] = val;	// 0
	m.elements[0][1] = val;	// 1
	m.elements[0][2] = val;	// 2
	m.elements[1][0] = val;	// 3
	m.elements[1][1] = val;	// 4 center pixel
	m.elements[1][2] = val;	// 5
	m.elements[2][0] = val;	// 6
	m.elements[2][1] = val;	// 7
	m.elements[2][2] = val;	// 8

	//m.elements[0][0] = 0.1;	// 0
	//m.elements[0][1] = 0.4;	// 1
	//m.elements[0][2] = 0.1;	// 2
	//m.elements[1][0] = 0.4;	// 3
	//m.elements[1][1] = -2.0;	// 4 center pixel
	//m.elements[1][2] = 0.4;	// 5
	//m.elements[2][0] = 0.1;	// 6
	//m.elements[2][1] = 0.4;	// 7
	//m.elements[2][2] = 0.1;	// 8

	/*
	0 1 2 - 00 01 02
	3 4 5 - 10 11 12
	6 7 8 - 20 21 22
	*/

	std::vector<Color> output(sizeA, Colors::Black);
	
	typedef unsigned char uchar;
	
    for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			Vec3 total = { 0.0f,0.0f,0.0f };

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int cx = x + col;
					int cy = y + row;

					if (cx > 0 && cx < w && cy > 0 && cy < h)
					{
						uint i = cy * w + cx;

						Vec3 pixel = Vec3( input[i] );

						total.x += (pixel.x *= m.elements[row + 1][col + 1]);
						total.y += (pixel.y *= m.elements[row + 1][col + 1]);
						total.z += (pixel.z *= m.elements[row + 1][col + 1]);
					}
				}
			}

			uint i = y * w + x;

			output[i].SetR(static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, total.x))));
			output[i].SetG(static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, total.y))));
			output[i].SetB(static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, total.z))));
		}
	}

	assert(input.size() == output.size());

	return output;
}

//void	Tetris::Benchmark(void* pFunction)
//{
//	std::ofstream file;
//	file.open("Benchmark.txt"); // creates file and names it and opens for writing to
//
//	const unsigned int number = 100;
//
//	for (int i = 0; i < number; i++)
//	{
//		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 
//
//		&pFunction;
//
//		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//
//		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
//
//		file << i << " " << duration.count() << " microseconds" << std::endl;
//	}
//}
//void	Tetris::BoxBlur(const Surface & input, std::vector<Color>& output)
//{
//	output.clear();
//
//	const uint height = input.GetHeight();
//	const uint width = input.GetWidth();
//
//	for (uint y = 0; y < height; y++)
//	{
//		for (uint x = 0; x < width; x++)
//		{
//			uchar rTotal = 0;
//			uchar gTotal = 0;
//			uchar bTotal = 0;
//
//			for (int row = -1; row <= 1; row++)
//			{
//				for (int col = -1; col <= 1; col++)
//				{
//					int currentX = x + col;
//					int currentY = y + row;
//
//					if (currentX > 0 && currentX < (int)width && currentY > 0 && currentY < (int)height)
//					{
//						Color c = input.GetPixel(currentX, currentY);
//
//						uchar r = c.GetR();
//						uchar g = c.GetG();
//						uchar b = c.GetB();
//
//						rTotal += r;
//						gTotal += g;
//						bTotal += b;
//					}
//				}
//			}
//
//			uint div = 9;
//
//			uchar rFinal = rTotal / div;
//			uchar gFinal = gTotal / div;
//			uchar bFinal = bTotal / div;
//
//			output.push_back( Color(rFinal, gFinal, bFinal) );
//			
//		}
//	}
//	assert(output.size() == (input.GetWidth() * input.GetHeight()));
//}
//auto Tetris::boxesForGauss(int sigma, int n)  // standard deviation, number of boxes
//{
//	float wIdeal = std::sqrt((12 * sigma*sigma / (float)n) + 1);  // Ideal averaging filter width 
//	int wl = (int)std::floor(wIdeal);  if (wl % 2 == 0) wl--;
//	int wu = wl + 2;
//
//	float mIdeal = (12 * sigma*sigma - (float)n * wl*wl - 4 * (float)n*wl - 3 * (float)n) / (-4 * wl - 4);
//	int m = (int)std::round(mIdeal);
//	// float sigmaActual = std::sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );
//
//	std::vector<int> sizes;  
//	
//	for (int i = 0; i < n; i++) 
//		sizes.push_back(i < m ? wl : wu);
//
//	return sizes;
//}
//
//void Tetris::gaussBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	auto bxs = boxesForGauss(r, 3);
//
//	boxBlur_4(scl, tcl, w, h, (bxs[0] - 1) / 2);
//	boxBlur_4(tcl, scl, w, h, (bxs[1] - 1) / 2);
//	boxBlur_4(scl, tcl, w, h, (bxs[2] - 1) / 2);
//}
//void Tetris::boxBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	for (auto i = 0; i < scl.size(); i++) tcl[i] = scl[i];
//
//	boxBlurH_4(tcl, scl, w, h, r);
//	boxBlurT_4(scl, tcl, w, h, r);
//}
//void Tetris::boxBlurH_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	auto iarr = 1 / (r + r + 1);
//
//	for (auto i = 0; i < h; i++) 
//	{
//		auto ti = i * w;
//		auto li = ti;
//		auto ri = ti + r;
//		auto fv = scl[ti];
//		auto lv = scl[ti + w - 1];
//		auto val = (r + 1)*fv;
//
//		for (auto j = 0; j < r; j++)
//		{
//			val += scl[ti + j];
//		}
//
//		for (auto j = 0; j <= r; j++) 
//		{ 
//			val += scl[ri++] - fv;
//			tcl[ti++] = (int)std::round(val*iarr);
//		}
//
//		for (auto j = r + 1; j < w - r; j++)
//		{ 
//			val += scl[ri++] - scl[li++];
//			tcl[ti++] = (int)std::round(val*iarr);
//		}
//
//		for (auto j = w - r; j < w; j++) 
//		{ 
//			val += lv - scl[li++];
//			tcl[ti++] = (int)std::round(val*iarr);
//		}
//	}
//}
//void Tetris::boxBlurT_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	auto iarr = 1 / (r + r + 1);
//
//	for (auto i = 0; i < w; i++) 
//	{
//		auto ti = i, li = ti, ri = ti + r * w;
//		auto fv = scl[ti];
//		auto lv = scl[ti + w * (h - 1)];
//		auto val = (r + 1)*fv;
//
//		for (auto j = 0; j < r; j++)
//		{
//			val += scl[ti + j * w];
//		}
//
//		for (auto j = 0; j <= r; j++)
//		{ 
//			val += scl[ri] - fv;
//			tcl[ti] = (int)std::round(val*iarr);
//			ri += w; ti += w;
//		}
//
//		for (auto j = r + 1; j < h - r; j++)
//		{ 
//			val += scl[ri] - scl[li];
//			tcl[ti] = (int)std::round(val*iarr);
//			li += w; ri += w; ti += w; 
//		}
//
//		for (auto j = h - r; j < h; j++)
//		{ 
//			val += lv - scl[li];  
//			tcl[ti] = (int)std::round(val*iarr);
//			li += w; ti += w;
//		}
//	}
//}
//
//void Tetris::gaussBlur_1(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	// scl = source channel, tcl = target channel, width, height, radius
//
//	int rs = (int)std::ceil(r * 2.57);     // significant radius
//
//	for (int i = 0; i < h; i++)
//	{
//		for (int j = 0; j < w; j++)
//		{
//			int val = 0, wsum = 0;
//
//			for (int iy = i - rs; iy < i + rs + 1; iy++)
//			{
//				for (int ix = j - rs; ix < j + rs + 1; ix++)
//				{
//					int x = std::min<int>(w - 1, std::max<int>(0, ix));
//					int y = std::min<int>(h - 1, std::max<int>(0, iy));
//					int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
//					int wght = (int)(std::exp(-dsq / (2 * r*r)) / (PI * 2 * r*r));
//					val += scl[y*w + x] * wght;  wsum += wght;
//				}
//			}
//			tcl[i*w + j] = (uchar)std::round(val / wsum);
//		}
//	}
//}
//std::vector<Color> Tetris::FastBlur(const std::vector<Color>& input, const int w, const int h, const int radius)
//{
//	assert(radius >= 1);
//
//	std::vector<Color> output(w, h);
//
//	const unsigned int wm = w - 1;
//	const unsigned int hm = h - 1;
//	const unsigned int wh = w * h;
//
//	const unsigned int div = radius + radius + 1;
//
//	std::vector<unsigned int> r(wh);
//	std::vector<unsigned int> g(wh);
//	std::vector<unsigned int> b(wh);
//	std::vector<unsigned int> vmin(wh);
//	std::vector<unsigned int> vmax(wh);
//	std::vector<unsigned int> pix(wh);
//
//	for (size_t y = 0; y < h; y++)
//	{
//		for (size_t x = 0; x < w; x++)
//		{
//			size_t i = y * w + x;
//			pix[i] = input[i].dword;
//		}
//	}
//	
//	size_t size = 256;
//
//	std::vector<unsigned int> dv(size * div);
//
//	for (int i = 0; i < size * div; i++)
//	{
//		dv[i] = (i / div);
//	}
//
//	size_t yw = 0;
//	size_t yi = 0;
//
//	for (int y = 0; y < h; y++)
//	{
//		unsigned int rsum = 0;
//		unsigned int gsum = 0;
//		unsigned int bsum = 0;
//
//
//		for (int i = -radius; i <= radius; i++)
//		{
//			const int p = pix[yi + std::min<int>(wm, std::max<int>(i, 0))];
//
//			rsum += (p & 0xff0000) >> 16;
//			gsum += (p & 0x00ff00) >> 8;
//			bsum += p & 0x0000ff;
//		}
//
//		for (int x = 0; x < w; x++)
//		{
//			r[yi] = dv[rsum];
//			g[yi] = dv[gsum];
//			b[yi] = dv[bsum];
//
//			if (y == 0)
//			{
//				vmin[x] = std::min<int>(x + radius + 1, wm);
//				vmax[x] = std::max<int>(x - radius, 0);
//			}
//
//			const int p1 = pix[yw + vmin[x]];
//			const int p2 = pix[yw + vmax[x]];
//
//			rsum += ((p1 & 0xff0000) - (p2 & 0xff0000)) >> 16;
//			gsum += ((p1 & 0x00ff00) - (p2 & 0x00ff00)) >> 8;
//			bsum += (p1 & 0x0000ff) - (p2 & 0x0000ff);
//
//			yi++;
//		}
//		yw += w;
//	}
//
//	for (int x = 0; x < w; x++)
//	{
//		int rsum = 0;
//		int gsum = 0;
//		int bsum = 0;
//
//		int yp = -radius * w;
//
//		for (int i = -radius; i <= radius; i++)
//		{
//			yi = std::max<int>(0, yp) + x;
//			rsum += r[yi];
//			gsum += g[yi];
//			bsum += b[yi];
//			yp += w;
//		}
//
//		yi = x;
//
//		for (int y = 0; y < h; y++)
//		{
//			pix[yi] = 0xff000000 | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];
//
//			if (x == 0)
//			{
//				vmin[y] = std::min<int>(y + radius + 1, hm) * w;
//				vmax[y] = std::max<int>(y - radius, 0) * w;
//			}
//
//			const int p1 = x + vmin[y];
//			const int p2 = x + vmax[y];
//
//			rsum += r[p1] - r[p2];
//			gsum += g[p1] - g[p2];
//			bsum += b[p1] - b[p2];
//
//			yi += w;
//		}
//	}
//	
//	for (unsigned int i = 0; i < wh; i++)
//	{
//		output[i].dword = pix[i];
//	}
//
//	return std::move(output);
//}
//
//Surface Tetris::FastBlur(const Surface& input, int radius)
//{
//	assert(radius >= 1);
//
//	int w = input.GetWidth();
//	int h = input.GetHeight();
//
//	assert(w >= 1 && h >= 1);
//
//	Surface output(w,h);
//
//	const int wm = w - 1;
//	const int hm = h - 1;
//	const int wh = w * h;
//
//	const int div = radius + radius + 1;
//
//	std::vector<unsigned int> r(wh);
//	std::vector<unsigned int> g(wh);
//	std::vector<unsigned int> b(wh);
//	std::vector<unsigned int> vmin(wh);
//	std::vector<unsigned int> vmax(wh);
//	std::vector<unsigned int> pix(wh);
//
//	for (int y = 0; y < h; y++)
//	{
//		for (int x = 0; x < w; x++)
//		{
//			int i = y * w + x;
//			pix[i] = input.GetBufferPtrConst()[i].dword;
//		}
//	}
//
//	size_t size = 256;
//
//	std::vector<int> dv(size * div);
//
//	for (int i = 0; i < size * div; i++)
//	{
//		dv[i] = (i / div);
//	}
//
//	size_t yw = 0;
//
//	for (int y = 0; y < h; y++)
//	{
//		unsigned int rsum = 0;
//		unsigned int gsum = 0;
//		unsigned int bsum = 0;
//
//		size_t yi = 0;
//
//		for (int i = -radius; i <= radius; i++)
//		{
//			const int p = pix[yi + std::min<int>(wm, std::max<int>(i, 0))];
//
//			rsum += (p & 0xff0000) >> 16;
//			gsum += (p & 0x00ff00) >> 8;
//			bsum += p & 0x0000ff;
//		}
//
//		for (int x = 0; x < w; x++)
//		{
//			r[yi] = dv[rsum];
//			g[yi] = dv[gsum];
//			b[yi] = dv[bsum];
//
//			if (y == 0)
//			{
//				vmin[x] = std::min<int>(x + radius + 1, wm);
//				vmax[x] = std::max<int>(x - radius, 0);
//			}
//
//			const int p1 = pix[yw + vmin[x]];
//			const int p2 = pix[yw + vmax[x]];
//
//			rsum += ((p1 & 0xff0000) - (p2 & 0xff0000)) >> 16;
//			gsum += ((p1 & 0x00ff00) - (p2 & 0x00ff00)) >> 8;
//			bsum += (p1 & 0x0000ff) - (p2 & 0x0000ff);
//
//			yi++;
//		}
//		yw += w;
//	}
//
//	for (int x = 0; x < w; x++)
//	{
//		int rsum = 0;
//		int gsum = 0;
//		int bsum = 0;
//
//		int yi = 0;
//		int yp = -radius * w;
//
//		for (int i = -radius; i <= radius; i++)
//		{
//			yi = std::max<int>(0, yp) + x;
//			rsum += r[yi];
//			gsum += g[yi];
//			bsum += b[yi];
//			yp += w;
//		}
//
//		yi = x;
//
//		for (int y = 0; y < h; y++)
//		{
//			pix[yi] = 0xff000000 | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];
//
//			if (x == 0)
//			{
//				vmin[y] = std::min<int>(y + radius + 1, hm) * w;
//				vmax[y] = std::max<int>(y - radius, 0) * w;
//			}
//
//			const int p1 = x + vmin[y];
//			const int p2 = x + vmax[y];
//
//			rsum += r[p1] - r[p2];
//			gsum += g[p1] - g[p2];
//			bsum += b[p1] - b[p2];
//
//			yi += w;
//		}
//	}
//
//	for (int i = 0; i < wh; i++)
//	{
//		output.GetBufferPtr()[i].dword = pix[i];
//	}
//
//	return std::move(output);
//}
//
//void Tetris::superFastBlur(unsigned char* pix, int w, int h, int radius) {
//
//	if (radius < 1) return;
//
//	int wm = w - 1;
//	int hm = h - 1;
//	int wh = w * h;
//	int div = radius + radius + 1;
//
//	unsigned char* r = new unsigned char[wh];
//	unsigned char* g = new unsigned char[wh];
//	unsigned char* b = new unsigned char[wh];
//
//	int rsum, gsum, bsum, x, y, i, p, p1, p2, yp, yi, yw;
//
//	int* vMIN = new int[std::max<int>(w, h)];
//	int* vMAX = new int[std::max<int>(w, h)];
//
//	unsigned char* dv = new unsigned char[256 * div];
//
//	for (i = 0; i < 256 * div; i++) dv[i] = (i / div);
//
//	yw = yi = 0;
//
//	for (y = 0; y < h; y++) {
//		rsum = gsum = bsum = 0;
//		for (i = -radius; i <= radius; i++) {
//			p = (yi + std::min<int>(wm, std::max<int>(i, 0))) * 3;
//			rsum += pix[p];
//			gsum += pix[p + 1];
//			bsum += pix[p + 2];
//		}
//		for (x = 0; x < w; x++) {
//
//			r[yi] = dv[rsum];
//			g[yi] = dv[gsum];
//			b[yi] = dv[bsum];
//
//			if (y == 0) {
//				vMIN[x] = std::min<int>(x + radius + 1, wm);
//				vMAX[x] = std::max<int>(x - radius, 0);
//			}
//			p1 = (yw + vMIN[x]) * 3;
//			p2 = (yw + vMAX[x]) * 3;
//
//			rsum += pix[p1] - pix[p2];
//			gsum += pix[p1 + 1] - pix[p2 + 1];
//			bsum += pix[p1 + 2] - pix[p2 + 2];
//
//			yi++;
//		}
//		yw += w;
//	}
//
//	for (x = 0; x < w; x++) 
//	{
//		rsum = gsum = bsum = 0;
//		yp = -radius * w;
//		for (i = -radius; i <= radius; i++) 
//		{
//			yi = std::max<int>(0, yp) + x;
//			rsum += r[yi];
//			gsum += g[yi];
//			bsum += b[yi];
//			yp += w;
//		}
//		yi = x;
//		for (y = 0; y < h; y++) 
//		{
//			pix[yi * 3] = dv[rsum];
//			pix[yi * 3 + 1] = dv[gsum];
//			pix[yi * 3 + 2] = dv[bsum];
//			if (x == 0) {
//				vMIN[y] = std::min<int>(y + radius + 1, hm) * w;
//				vMAX[y] = std::max<int>(y - radius, 0) * w;
//			}
//			p1 = x + vMIN[y];
//			p2 = x + vMAX[y];
//
//			rsum += r[p1] - r[p2];
//			gsum += g[p1] - g[p2];
//			bsum += b[p1] - b[p2];
//
//			yi += w;
//		}
//	}
//
//	delete[] r;
//	delete[] g;
//	delete[] b;
//
//	delete[] vMIN;
//	delete[] vMAX;
//	delete[] dv;
//}