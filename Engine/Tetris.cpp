#include "Tetris.h"

#include <thread>
#include <random>
#include <fstream>
#include <cmath>
#include <cassert>
#include <utility>
#include <algorithm>
#include <iostream>

#include "Random.h"

using namespace rnd;

using uint	= unsigned int;
using uchar = unsigned char;

using Time	= std::chrono::steady_clock;
using ms	= std::chrono::microseconds;

Tetris::Tetris(Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	:
	kbd(kbd),
	mouse(mouse),
	gfx(gfx),
	sound_move(		L"Sounds\\pop1.wav" ),
	sound_line1(	L"Sounds\\success0.wav"),
	sound_line2(	L"Sounds\\success1.wav"),
	sound_line3(	L"Sounds\\success2.wav"),
	sound_line4(	L"Sounds\\success3.wav"),
	sound_new_tetro(L"Sounds\\pop0.wav"),
	sound_gameover(	L"Sounds\\fail0.wav")
{
	Initialise();
	Setup();
}
Tetris::~Tetris(){}

/*-------------------------------------------*/

void Tetris::Initialise()
{
	InitialiseTextures();
	InitialiseBackground();
	InitialiseBlockPositions();
	InitialiseTetrominos();
	InitialiseNextTetro();
	InitialiseCounter();
	InitialiseScore();
	InitialisePause();
	InitialiseGameOver();
	InitialiseKeys();
	InitialiseButtons();
}
void Tetris::Setup()
{	
	gameIsPaused	= false;
	gameIsOver		= false;
	nextTetro		= RandomInt(0u, 6u);
	currentTetro	= RandomInt(0u, 6u);
	currentX		= fieldW / 2u - 2u;
	currentY		= 0u;
	currentRotation	= 0u;
	tickCounter		= 0u;
	frameCounter	= 0u;
	speed			= 120u;
	//level			= 0u;
	//counterTetro	= 0u;
	//counterSpeed	= 0u;

	SetBackground();
	ResetScore();	
	ResetField();
	SetFieldBlocks();
	SetNextTetro();
	SetCounter();
}
void Tetris::Input()
{
	InputMouseKeys();
	InputMouseButtons();
	InputKeyboard();
	Pause();
	Reset();
	Quit();
	Sound();
}

/*-------------------------------------------*/

void Tetris::Update()
{	
	Input();

	if (!gameIsOver && !gameIsPaused)
	{
		if (frameCounter == speed)
		{
			if (DoesTetroFit(0,0,MOVE_DOWN))
			{
				if(!keyIsPressed_DOWN) currentY++; // force tetris down
			}
			else
			{
				SetFixedWithTetro();
				CheckForLines();
				SetScore();
				SetNextTetro();
				
				gameIsOver = !DoesTetroFit(0, 0, 0);
				if(gameIsOver) sound_gameover.Play(frequency, volume);
			}

			frameCounter = 0;
		}

		SetShownWithFixed();
		SetShownWithTetro();
		DeleteLines();
		SetFieldBlocks();

		frameCounter++;
	}		
}
void Tetris::Draw()
{
	DrawBackground();
	DrawFieldBlocks();
	DrawNextTetro();
	DrawBlur();
	DrawPause();
	DrawGameOver();
	DrawScore();
	DrawCounter();
	DrawKeys();
	DrawButtons();
}

/*-------------------------------------------*/

void Tetris::InitialiseTextures()
{
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Blocks3DRainbow.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksBlue.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksGreen.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature0.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature2.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature5.jpg"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Street0.bmp"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Space1.jpg"));

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
	
	texture_Pause.push_back(Surface::FromFile(L"Textures\\Words\\Pause.png"));
	texture_GameOver.push_back(Surface::FromFile(L"Textures\\Words\\GameOver.png"));

	texture_Key.push_back(Surface::FromFile(L"Textures\\key.png"));
	texture_Key.push_back(Surface::FromFile(L"Textures\\Keys\\key_up.png"));
	texture_Key.push_back(Surface::FromFile(L"Textures\\Keys\\key_pause.png"));
	texture_Key.push_back(Surface::FromFile(L"Textures\\Keys\\key_left.png"));
	texture_Key.push_back(Surface::FromFile(L"Textures\\Keys\\key_down.png"));
	texture_Key.push_back(Surface::FromFile(L"Textures\\Keys\\key_right.png"));

	texture_Button.push_back(Surface::FromFile(L"Textures\\close4.png"));
	texture_Button.push_back(Surface::FromFile(L"Textures\\cog1.png"));
	texture_Button.push_back(Surface::FromFile(L"Textures\\Volume_Full.png"));
	texture_Mute.push_back(Surface::FromFile(L"Textures\\Volume_Mute.png"));
}
void Tetris::InitialiseBackground()
{
	RectI rect = { 0,static_cast<int>(scrH-1),0,static_cast<int>(scrW-1) };	
	uint min = 0u;
	uint max = static_cast<uint>(texture_Background.size()) - 1u;
	index = RandomInt(min, max);
	block_Background = Block(rect, &texture_Background[index]);
}
void Tetris::InitialiseBlockPositions()
{
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			RectI pos = RectI(
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH),
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH) + blockH,
				(scrW / 2) - ((fieldW / 2) * blockW) + (x * blockW),
				(scrW / 2) - ((fieldW / 2) * blockW) + (x * blockW) + blockW);
			
			blocks_Field[y][x] = Block(pos, &texture_Blocks[0]);
		}
	}
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
void Tetris::InitialiseNextTetro()
{
	for (uint y = 0u; y < tetroH; y++)
	{
		for (uint x = 0u; x < tetroW; x++)
		{
			RectI rect = RectI(
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH),
				(scrH / 2) - ((fieldH / 2) * blockH) + (y * blockH) + blockH,
				(scrW / 2) - ((fieldW / 2) * blockW) + (blockW * fieldW) + (x * blockW),
				(scrW / 2) - ((fieldW / 2) * blockW) + (blockW * fieldW) + (x * blockW) + blockW);

			blocks_NextTetro[y][x] = Block(rect, &texture_Blocks[0]);
		}
	}
}
void Tetris::InitialiseScore()
{	
	for (int y = 0; y < scoreH; y++)
	{
		for (int x = 0; x < scoreW; x++)
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
	RectI rect = RectI(
		(scrH / 2) - (pauseH / 2),
		(scrH / 2) + (pauseH / 2),
		(scrW / 2) - (pauseW / 2),
		(scrW / 2) + (pauseW / 2));

	size_t size = texture_Pause.size();
	size_t i = (size > 1) ? RandomInt(static_cast<size_t>(0), size - 1) : 0;
	block_Pause = Block(rect, &texture_Pause[i]);
}
void Tetris::InitialiseGameOver()
{
	RectI rect = RectI(
		(scrH / 2) - (gameOverH / 2),
		(scrH / 2) + (gameOverH / 2),
		(scrW / 2) - (gameOverW / 2),
		(scrW / 2) + (gameOverW / 2));

	size_t size = texture_GameOver.size();
	size_t i = (size > 1) ? RandomInt(static_cast<size_t>(0), size - 1) : 0;
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
void Tetris::InitialiseKeys()
{
	const uint top = (scrH / 2u) + ((fieldH / 2u) * blockH) - (keyH * 2u);
	const uint left = (scrW / 2u) + ((fieldW / 2u) * blockW) + keyW;

	{
		uint t0 = top + (keyH * 0u);
		uint t1 = top + (keyH * 1u);
		
		uint b0 = top + (keyH * 1u) - 1u;
		uint b1 = top + (keyH * 2u) - 1u;
		
		uint l0 = left + (keyW * 0u);
		uint l1 = left + (keyW * 1u);
		uint l2 = left + (keyW * 2u);
		
		uint r0 = left + (keyW * 1u) - 1u;
		uint r1 = left + (keyW * 2u) - 1u;
		uint r2 = left + (keyW * 3u) - 1u;

		RectUI top_left = { t0,b0,l0,r0 };
		RectUI top_middle = { t0,b0,l1,r1 };
		RectUI top_right = { t0,b0,l2,r2 };

		RectUI bottom_left = { t1,b1,l0,r0 };
		RectUI bottom_middle = { t1,b1,l1,r1 };
		RectUI bottom_right = { t1,b1,l2,r2 };

		rect_KeyReleased.push_back(top_left);
		rect_KeyReleased.push_back(top_middle);
		rect_KeyReleased.push_back(top_right);

		rect_KeyReleased.push_back(bottom_left);
		rect_KeyReleased.push_back(bottom_middle);
		rect_KeyReleased.push_back(bottom_right);
	}

	/*-----------------------------------------------------------*/

	{
		const uint offset = 5u;

		uint t0 = top + (keyH * 0u) + offset;
		uint t1 = top + (keyH * 1u) + offset;

		uint b0 = top + (keyH * 1u) - offset - 1u;
		uint b1 = top + (keyH * 2u) - offset - 1u;

		uint l0 = left + (keyW * 0u) + offset;
		uint l1 = left + (keyW * 1u) + offset;
		uint l2 = left + (keyW * 2u) + offset;

		uint r0 = left + (keyW * 1u) - offset - 1u;
		uint r1 = left + (keyW * 2u) - offset - 1u;
		uint r2 = left + (keyW * 3u) - offset - 1u;

		RectUI top_left = { t0,b0,l0,r0 };
		RectUI top_middle = { t0,b0,l1,r1 };
		RectUI top_right = { t0,b0,l2,r2 };
			
		RectUI bottom_left = { t1,b1,l0,r0 };
		RectUI bottom_middle = { t1,b1,l1,r1 };
		RectUI bottom_right = { t1,b1,l2,r2 };

		rect_KeyPressed.push_back(top_left);
		rect_KeyPressed.push_back(top_middle);
		rect_KeyPressed.push_back(top_right);

		rect_KeyPressed.push_back(bottom_left);
		rect_KeyPressed.push_back(bottom_middle);
		rect_KeyPressed.push_back(bottom_right);
	}

	assert(rect_KeyPressed.size() == rect_KeyReleased.size());
	assert(rect_KeyPressed.size() == nKeys);

	for (int i = 0; i < nKeys; i++)
	{
		block_KeyReleased[i]	= Block(rect_KeyReleased[i], &texture_Key[i]);
		block_KeyPressed[i]		= Block(rect_KeyPressed[i], &texture_Key[i]);
	}

	/*-------------------------------------------*/

	mouseOverKey.assign(nKeys, false);
	mousePressKey.assign(nKeys, false);
	
	assert(mouseOverKey.size() == mousePressKey.size());
	assert(mouseOverKey.size() == nKeys);
}
void Tetris::InitialiseButtons()
{
	mouseOverButton.assign(nButtons, false);
	mousePressButton.assign(nButtons, false);

	{
		const uint t = buttonH * 0u;
		const uint b = buttonH * 1u;
		const uint l = buttonW * 0u;
		const uint r = buttonW * 1u;

		const RectUI rect = { t,b,l,r };

		rect_Button.push_back(rect);
	}

	{
		const uint t = (scrH - 1u) - (buttonH * 1u);
		const uint b = (scrH - 1u) - (buttonH * 0u);
		const uint l = buttonW * 0u;
		const uint r = buttonW * 1u;

		const RectUI rect = { t,b,l,r };

		rect_Button.push_back(rect);
	}

	{
		const uint t = (scrH - 1u) - (buttonH * 2u);
		const uint b = (scrH - 1u) - (buttonH * 1u);
		const uint l = buttonW * 0u;
		const uint r = buttonW * 1u;

		const RectUI rect = { t,b,l,r };

		rect_Button.push_back(rect);		
	}

	for (int i = 0; i < nButtons; i++)
	{
		block_Button[i] = Block(rect_Button[i], &texture_Button[i]);
	}	
}

/*-------------------------------------------*/

void Tetris::InputMouseKeys()
{
	if (mouse.IsInWindow())
	{
		const uint mouseX = mouse.GetPosX();
		const uint mouseY = mouse.GetPosY();
		const bool leftIsPressed = mouse.LeftIsPressed();

		/*-------------------------------------------*/

		for (uint i = 0u; i < nKeys; i++)
		{	
			const bool fitsLeft		= mouseX >=	rect_KeyReleased[i].left;
			const bool fitsRight	= mouseX <	rect_KeyReleased[i].right;
			const bool fitsTop		= mouseY >= rect_KeyReleased[i].top;
			const bool fitsBottom	= mouseY <	rect_KeyReleased[i].bottom;

			const bool isOver		= fitsLeft && fitsRight && fitsTop && fitsBottom;

			if (isOver)
			{
				mouseOverKey[i] = true;
			}
			else
			{
				mouseOverKey[i] = false;
			}
		}

		/*-------------------------------------------*/

		for (uint i = 0u; i < nKeys; i++)
		{
			if (mouseOverKey[i] && leftIsPressed)
			{
				mousePressKey[i] = true;
			}
			else
			{
				mousePressKey[i] = false;
			}
		}
	}		
}
void Tetris::InputMouseButtons()
{
	if (mouse.IsInWindow())
	{
		const uint mouseX = mouse.GetPosX();
		const uint mouseY = mouse.GetPosY();
		const bool leftIsPressed = mouse.LeftIsPressed();

		/*-------------------------------------------*/

		for (uint i = 0u; i < nButtons; i++)
		{
			const bool fitsLeft		= mouseX >=	rect_Button[i].left;
			const bool fitsRight	= mouseX <	rect_Button[i].right;
			const bool fitsTop		= mouseY >=	rect_Button[i].top;
			const bool fitsBottom	= mouseY <	rect_Button[i].bottom;

			const bool isOver		= fitsLeft && fitsRight && fitsTop && fitsBottom;

			if (isOver)
			{
				mouseOverButton[i] = true;
			}
			else
			{
				mouseOverButton[i] = false;
			}
		}

		/*-------------------------------------------*/

		for (uint i = 0u; i < nButtons; i++)
		{
			if (mouseOverButton[i] && leftIsPressed)
			{
				mousePressButton[i] = true;
			}
			else
			{
				mousePressButton[i] = false;
			}
		}
	}
}
void Tetris::InputKeyboard()
{
	if (!gameIsPaused && !gameIsOver)
	{
		const bool tick = frameCounter > 0u && frameCounter % 10u == 0u;

		if (!keyIsPressed_LEFT)
		{
			keyIsPressed_LEFT = kbd.KeyIsPressed(VK_LEFT) || mousePressKey[LEFT];
		}

		if (!keyIsPressed_RIGHT)
		{
			keyIsPressed_RIGHT = kbd.KeyIsPressed(VK_RIGHT) || mousePressKey[RIGHT];
		}

		if (!keyIsPressed_DOWN)
		{
			keyIsPressed_DOWN = kbd.KeyIsPressed(VK_DOWN) || mousePressKey[DOWN];
		}

		const bool tetroFits_LEFT = DoesTetroFit(0, MOVE_LEFT, 0);
		const bool tetroFits_RIGHT = DoesTetroFit(0, MOVE_RIGHT, 0);
		const bool tetroFits_DOWN = DoesTetroFit(0, 0, MOVE_DOWN);

		if (tick)
		{
			if (keyIsPressed_LEFT)
			{
				if (tetroFits_LEFT) currentX--;
				keyIsPressed_LEFT = false;
			}

			if (keyIsPressed_RIGHT)
			{
				if (tetroFits_RIGHT)	currentX++;
				keyIsPressed_RIGHT = false;
			}

			if (keyIsPressed_DOWN)
			{
				if (tetroFits_DOWN) currentY++;
				keyIsPressed_DOWN = false;
			}

			SetCounter();
		}

		const bool tetroFits_RotatedClockWise = DoesTetroFit(ROTATE_CW, 0, 0);

		if (!keyIsPressed_UP)
		{
			if (kbd.KeyIsPressed(VK_UP) || mousePressKey[UP])
			{
				if (tetroFits_RotatedClockWise)
				{
					currentRotation++;
					sound_move.Play(frequency, volume);
				}

				keyIsPressed_UP = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_UP) && !mousePressKey[UP])
			{
				keyIsPressed_UP = false;
			}
		}
	}
}
void Tetris::Pause()
{
	if (!gameIsOver)
	{
		if (!keyIsPressed_SPACE)
		{
			if (kbd.KeyIsPressed(VK_SPACE) || mousePressKey[SPACE])
			{
				if (!gameIsPaused)
				{
					gameIsPaused = true;
				}
				else
				{
					gameIsPaused = false;
				}

				keyIsPressed_SPACE = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_SPACE) && !mousePressKey[SPACE])
			{
				keyIsPressed_SPACE = false;
			}
		}
	}
}
void Tetris::Reset()
{
	if (gameIsPaused || gameIsOver)
	{
		if (mousePressKey[ESCAPE])
		{
			Setup();
		}
	}
}
void Tetris::Quit()
{
	if (mousePressButton[QUIT])
	{
		PostQuitMessage(0);
	}
}
void Tetris::Settings()
{

}

void Tetris::Sound()
{
	const bool leftIsPressed = mouse.LeftIsPressed();

	if (!mouseIsPressed)
	{
		if (mousePressButton[VOLUME])
		{
			if (volumeIsFULL)
			{
				block_Button[VOLUME].SetTexture(&texture_Mute[0]);
				volumeIsFULL = false;
				volume = 0.0f;
			}
			else
			{
				block_Button[VOLUME].SetTexture(&texture_Button[VOLUME]);
				volumeIsFULL = true;
				volume = 1.0f;
			}

			mouseIsPressed = true;
		}
	}
	else
	{
		if (!leftIsPressed)
		{
			mouseIsPressed = false;
		}
	}
}

/*-------------------------------------------*/

void Tetris::SetBackground()
{
	if (index < texture_Background.size() - 1u)
	{
		index++;
	}
	else
	{
		index = 0u;
	}
	block_Background.SetTexture( &texture_Background[index] );
}
void Tetris::ResetScore()
{
	blockBuffer_Score.clear();
	score = 0;
	ExtractDigits(blockBuffer_Score, score);
}
void Tetris::ResetField()
{
	for (int y = 0; y < fieldH; y++)	
	{
		for (int x = 0; x < fieldW; x++)
		{
			int i = y * fieldW + x;
			blockBuffer_Fixed[i] = (x == 0 || x == fieldW - 1 || y == fieldH - 1) ? 9 : 0;

			//if (x == 0 || x == fieldW - 1) field[y*fieldW + x] = 9;
			//else if(y == fieldH - 1) field[y*fieldW + x] = 9;
			//else field[y*fieldW + x] = 0;
		}
	}
}
void Tetris::SetFieldBlocks()
{
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			int i = y * fieldW + x;
			int j = ConvertCharToInt(blockBuffer_Shown[i]);
			blocks_Field[y][x].SetTexture(&texture_Blocks[j]);
		}
	}
}
void Tetris::SetNextTetro()
{
	currentX = fieldW / 2 - 2;
	currentY = 0;
	currentRotation = 0;

	currentTetro = nextTetro;
	nextTetro = RandomInt(0, 6);

	for (uint y = 0; y < tetroH; y++)
	{
		for (uint x = 0; x < tetroW; x++)
		{	
			int i = y * tetroW + x;
			int j = (tetromino[nextTetro][i] != '.') ? nextTetro + 1 : 0;

			blocks_NextTetro[y][x].SetTexture(&texture_Blocks[j]);
		}
	}

	sound_new_tetro.Play(frequency,volume);
}
void Tetris::SetScore()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
{
	score += 25;
	if (!lines.empty()) score += (1 << lines.size()) * 100;
	ExtractDigits(blockBuffer_Score, score);

	if (score % 1000 == 0) speed -= (speed / 2);
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
void Tetris::CheckForLines()
{
	for (uint y = 0; y < tetroH; y++)
	{
		if (currentY + y < fieldH - 1)
		{
			bool isLine = true;
			for (uint x = 1; x < fieldW - 1; x++)
			{
				uint i = (currentY + y) * fieldW + x;
				isLine &= (blockBuffer_Fixed[i] != 0);
			}

			if (isLine)
			{
				// set line to '=' (red)
				for (uint x = 1; x < fieldW - 1; x++)
				{
					uint i = (currentY + y) * fieldW + x;
					blockBuffer_Fixed[i] = 8;
				}
				lines.push_back(currentY + y);
			}
		}
	}
}
void Tetris::DeleteLines()
{
	if (!lines.empty())
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(800)); // delay

		/*for (auto& v : lines)
		{
			for (int x = 1; x < fieldW - 1; x++)
			{
				for (int y = v; y > 0; y--)
				{
					int i = y * fieldW + x;
					int j = (y - 1) * fieldW + x;
					blockBuffer_Fixed[i] = blockBuffer_Fixed[j];
				}
				blockBuffer_Fixed[x] = 0;
			}
		}*/

		const uint nLines = static_cast<uint>(lines.size());		
		
		for (uint x = 1; x < fieldW - 1; x++)
		{
			for (uint i = 0; i < nLines; i++)
			{
				for (uint y = lines[i]; y > 0; y--)
				{					
					int j = y * fieldW + x;
					int k = (y - 1) * fieldW + x;
					blockBuffer_Fixed[j] = blockBuffer_Fixed[k];
				}					
				blockBuffer_Fixed[x] = 0;
			}
		}

		lines.clear();

		switch (nLines)
		{
		case 1:
			sound_line1.Play(frequency, volume);
			break;
		case 2:
			sound_line2.Play(frequency, volume);
			break;
		case 3:
			sound_line3.Play(frequency, volume);
			break;
		case 4:
			sound_line4.Play(frequency, volume);
			break;
		default:
			break;
		}
	}
}
void Tetris::SetFixedWithTetro()
{
	for (uint y = 0; y < tetroH; y++)
	{
		for (uint x = 0; x < tetroW; x++)
		{
			if (tetromino[currentTetro][Rotate(x, y, currentRotation)] == 'X')
			{
				uint i = (currentY + y) * fieldW + (currentX + x);
				blockBuffer_Fixed[i] = currentTetro + 1;
			}
		}
	}
}
void Tetris::SetShownWithTetro()
{
	// add current tetris piece to display buffer
	for (uint y = 0; y < tetroH; y++)
	{
		for (uint x = 0; x < tetroW; x++)
		{
			// set to the ascii table value
			if (tetromino[currentTetro][Rotate(x, y, currentRotation)] == 'X')
			{		
				uint i = (currentY + y) * fieldW + (currentX + x);
				blockBuffer_Shown[i] = " ABCDEFG=#"[currentTetro + 1];
			}
		}
	}
}
void Tetris::SetShownWithFixed()
{
	// add all fixed blocks to displayBuffer from fixedBuffer
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			uint i = y * fieldW + x;
			blockBuffer_Shown[i] = " ABCDEFG=#"[blockBuffer_Fixed[i]];
		}
	}
}

/*-------------------------------------------*/

void Tetris::DrawBackground()
{
	block_Background.Draw(gfx);
}
void Tetris::DrawFieldBlocks()
{
	for (uint y = 0; y < fieldH; y++)
	{
		for (uint x = 0; x < fieldW; x++)
		{
			uint i = y * fieldW + x;
			if (blockBuffer_Shown[i] == ' ')
			{
				continue;
			}
			else
			{
				blocks_Field[y][x].Draw(gfx);
			}
		}
	}
}
void Tetris::DrawNextTetro()
{
	if (!gameIsOver)
	{
		for (uint y = 0; y < tetroH; y++)
		{
			for (uint x = 0; x < tetroW; x++)
			{
				uint i = y * tetroW + x;
				if (tetromino[nextTetro][i] == '.')
				{
					continue;
				}
				else
				{
					blocks_NextTetro[y][x].Draw(gfx);
				}
			}
		}
	}
}
void Tetris::DrawCounter()
{
	if (!gameIsOver && !gameIsPaused)
	{
		blocks_Counter[tickCounter].Draw(gfx);
	}
}
void Tetris::DrawBlur()
{
	if (gameIsPaused || gameIsOver) 
	{
		std::vector<Color> input;
		input = ConvertSurfaceToColorVector(gfx.CopySysBuffer());

		uint w = scrW;
		uint h = scrH;
		size_t wh = static_cast<size_t>(w) * h;

		std::vector<Color> output(wh);
		
		for (uint i = 0; i < nBlur; i++)
		{
			input = Blur(w, h, input);
		}
		output = input;

		for (uint y = 0; y < h; y++)
		{
			for (uint x = 0; x < w; x++)
			{
				size_t i = static_cast<size_t>(y) * w + x;
				gfx.PutPixel(x, y, output[i]);
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
void Tetris::DrawPause()
{
	if (gameIsPaused)
	{		
		block_Pause.Draw(gfx);
	}
}
void Tetris::DrawGameOver()
{
	if (gameIsOver)
	{		
		block_GameOver.Draw(gfx);
	}
}
void Tetris::DrawKeys()
{
	if (true)
	{
		std::vector<Color> keyColor(nKeys, Colors::White);

		if (mouse.IsInWindow())
		{
			int mouseX = mouse.GetPosX();
			int mouseY = mouse.GetPosY();

			for (int i = 0; i < nKeys; i++)
			{
				if (mouseOverKey[i])
				{
					keyColor[i] = Colors::Green;
				}
				else
				{
					keyColor[i] = Colors::White;
				}
			}
		}

		for (int i = 0; i < nKeys; i++)
		{
			gfx.DrawRect(rect_KeyReleased[i], keyColor[i]);
		}

	}

	std::vector<unsigned int> VKs({ VK_ESCAPE,VK_UP,VK_SPACE,VK_LEFT,VK_DOWN,VK_RIGHT });
	
	for (int i = 0; i < nKeys; i++)
	{
		if (kbd.KeyIsPressed(VKs[i]) || mousePressKey[i])
		{
			block_KeyPressed[i].Draw(gfx);
		}
		else
		{
			block_KeyReleased[i].Draw(gfx);
		}
	}
}
void Tetris::DrawButtons()
{
	if (true)
	{
		std::vector<Color> buttonColor(nButtons, Colors::White);

		if (mouse.IsInWindow())
		{
			int mouseX = mouse.GetPosX();
			int mouseY = mouse.GetPosY();

			for (int i = 0; i < nButtons; i++)
			{
				if (mouseOverButton[i])
				{
					buttonColor[i] = Colors::Green;
				}
				else
				{
					buttonColor[i] = Colors::White;
				}
			}
		}

		for (int i = 0; i < nButtons; i++)
		{
			gfx.DrawRect(rect_Button[i], buttonColor[i]);
		}
	}

	for (int i = 0; i < nButtons; i++)
	{
		block_Button[i].Draw(gfx);
	}
}

/*-------------------------------------------*/

int		Tetris::Rotate(int x, int y,  int r)
{
	int i = 0;

	switch (r % 4)
	{
	case 0: //   0 degrees 
		i = 0 + (y * 4) + x;
		break;				
							
		//  0  1  2  3
		//  4  5  6  7
		//  8  9 10 11
		// 12 13 14 15					

	case 1: //  90 degrees
		i = 12 + y - (x * 4);
		break;				
							
		// 12  8  4  0
		// 13  9  5  1
		// 14 10  6  2
		// 15 11  7  3				

	case 2: // 120 degrees
		i = 15 - (y * 4) - x;
		break;				
							
		// 15 14 13 12
		// 11 10  9  8
		//  7  6  5  4
		//  3  2  1  0				

	case 3: // 270 degrees
		i = 3 - y + (x * 4);
		break;				
							
		//  3  7 11 15
		//  2  6 10 14
		//  1  5  9 13
		//  0  4  8 12				
	}

	return i;
}
bool	Tetris::DoesTetroFit(int offsetRotation, int offsetX, int offsetY)
{
	const uint rotation = currentRotation + offsetRotation;
	const uint X = currentX + offsetX;
	const uint Y = currentY + offsetY;

	for (uint y = 0; y < tetroH; y++)
	{
		for (uint x = 0; x < tetroW; x++)
		{
			const bool isInsideField = X + x >= 0 && X + x < fieldW && Y + y >= 0 && Y + y < fieldH;

			if ( isInsideField )
			{
				const uint i = Rotate(x, y, rotation);
				const uint j = (Y + y) * fieldW + (X + x);

				if (tetromino[currentTetro][i] != L'.' && blockBuffer_Fixed[j] != 0)
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
	uint w = surface.GetWidth();
	uint h = surface.GetHeight();

	std::vector<Color> output;

	for (uint y = 0; y < h; y++)
	{
		for (uint x = 0; x < w; x++)
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
		
	const float val = 1.0f / 9.0f;
	std::array<std::array<float, 3>, 3> box = {	val,val,val,
												val,val,val,
												val,val,val };
	
	/*
	0 1 2  c  c  c
	3 4 5  o  o  o
	6 7 8  l  l  l
		   0  1  2
	row 0 00 01 02
	row 1 10 11 12
	row 2 20 21 22
	*/

	std::vector<Color> output;
	
	Vec3 pixel_out;

    for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			pixel_out = { 0.0f,0.0f,0.0f };

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int cx = x + col;
					int cy = y + row;

					if (cx > 0 && cx < w && cy > 0 && cy < h)
					{
						int i = cy * w + cx;

						pixel_out.x += (input[i].GetR() * 
							box[static_cast<size_t>(row) + 1][static_cast<size_t>(col) + 1]);
						pixel_out.y += (input[i].GetG() *
							box[static_cast<size_t>(row) + 1][static_cast<size_t>(col) + 1]);
						pixel_out.z += (input[i].GetB() *
							box[static_cast<size_t>(row) + 1][static_cast<size_t>(col) + 1]);
					}
				}
			}

			output.push_back({ 
				static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.x))),
				static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.y))),
				static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.z))) });
		}
	}

	assert(input.size() == output.size());

	return output;
}

/* FUNCTIONS I COULDN'T GET TO WORK */
/*void	Tetris::Benchmark(void* pFunction)
{
	std::ofstream file;
	file.open("Benchmark.txt"); // creates file and names it and opens for writing to

	const unsigned int number = 100;

	for (int i = 0; i < number; i++)
	{
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now(); 

		&pFunction;

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

		file << i << " " << duration.count() << " microseconds" << std::endl;
	}
}*/
/*void	Tetris::BoxBlur(const Surface & input, std::vector<Color>& output)
{
	output.clear();

	const uint height = input.GetHeight();
	const uint width = input.GetWidth();

	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			uchar rTotal = 0;
			uchar gTotal = 0;
			uchar bTotal = 0;

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int currentX = x + col;
					int currentY = y + row;

					if (currentX > 0 && currentX < (int)width && currentY > 0 && currentY < (int)height)
					{
						Color c = input.GetPixel(currentX, currentY);

						uchar r = c.GetR();
						uchar g = c.GetG();
						uchar b = c.GetB();

						rTotal += r;
						gTotal += g;
						bTotal += b;
					}
				}
			}

			uint div = 9;

			uchar rFinal = rTotal / div;
			uchar gFinal = gTotal / div;
			uchar bFinal = bTotal / div;

			output.push_back( Color(rFinal, gFinal, bFinal) );
			
		}
	}
	assert(output.size() == (input.GetWidth() * input.GetHeight()));
}*/
/*auto Tetris::boxesForGauss(int sigma, int n)  // standard deviation, number of boxes
{
	float wIdeal = std::sqrt((12 * sigma*sigma / (float)n) + 1);  // Ideal averaging filter width 
	int wl = (int)std::floor(wIdeal);  if (wl % 2 == 0) wl--;
	int wu = wl + 2;

	float mIdeal = (12 * sigma*sigma - (float)n * wl*wl - 4 * (float)n*wl - 3 * (float)n) / (-4 * wl - 4);
	int m = (int)std::round(mIdeal);
	// float sigmaActual = std::sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	std::vector<int> sizes;  
	
	for (int i = 0; i < n; i++) 
		sizes.push_back(i < m ? wl : wu);

	return sizes;
}*/
/*void Tetris::gaussBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
{
	auto bxs = boxesForGauss(r, 3);

	boxBlur_4(scl, tcl, w, h, (bxs[0] - 1) / 2);
	boxBlur_4(tcl, scl, w, h, (bxs[1] - 1) / 2);
	boxBlur_4(scl, tcl, w, h, (bxs[2] - 1) / 2);
}*/
/*void Tetris::boxBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
{
	for (auto i = 0; i < scl.size(); i++) tcl[i] = scl[i];

	boxBlurH_4(tcl, scl, w, h, r);
	boxBlurT_4(scl, tcl, w, h, r);
}*/
/*void Tetris::boxBlurH_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
{
	auto iarr = 1 / (r + r + 1);

	for (auto i = 0; i < h; i++) 
	{
		auto ti = i * w;
		auto li = ti;
		auto ri = ti + r;
		auto fv = scl[ti];
		auto lv = scl[ti + w - 1];
		auto val = (r + 1)*fv;

		for (auto j = 0; j < r; j++)
		{
			val += scl[ti + j];
		}

		for (auto j = 0; j <= r; j++) 
		{ 
			val += scl[ri++] - fv;
			tcl[ti++] = (int)std::round(val*iarr);
		}

		for (auto j = r + 1; j < w - r; j++)
		{ 
			val += scl[ri++] - scl[li++];
			tcl[ti++] = (int)std::round(val*iarr);
		}

		for (auto j = w - r; j < w; j++) 
		{ 
			val += lv - scl[li++];
			tcl[ti++] = (int)std::round(val*iarr);
		}
	}
}*/
/*void Tetris::boxBlurT_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
{
	auto iarr = 1 / (r + r + 1);

	for (auto i = 0; i < w; i++) 
	{
		auto ti = i, li = ti, ri = ti + r * w;
		auto fv = scl[ti];
		auto lv = scl[ti + w * (h - 1)];
		auto val = (r + 1)*fv;

		for (auto j = 0; j < r; j++)
		{
			val += scl[ti + j * w];
		}

		for (auto j = 0; j <= r; j++)
		{ 
			val += scl[ri] - fv;
			tcl[ti] = (int)std::round(val*iarr);
			ri += w; ti += w;
		}

		for (auto j = r + 1; j < h - r; j++)
		{ 
			val += scl[ri] - scl[li];
			tcl[ti] = (int)std::round(val*iarr);
			li += w; ri += w; ti += w; 
		}

		for (auto j = h - r; j < h; j++)
		{ 
			val += lv - scl[li];  
			tcl[ti] = (int)std::round(val*iarr);
			li += w; ti += w;
		}
	}
}*/
/*void Tetris::gaussBlur_1(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
{
	// scl = source channel, tcl = target channel, width, height, radius

	int rs = (int)std::ceil(r * 2.57);     // significant radius

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int val = 0, wsum = 0;

			for (int iy = i - rs; iy < i + rs + 1; iy++)
			{
				for (int ix = j - rs; ix < j + rs + 1; ix++)
				{
					int x = std::min<int>(w - 1, std::max<int>(0, ix));
					int y = std::min<int>(h - 1, std::max<int>(0, iy));
					int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
					int wght = (int)(std::exp(-dsq / (2 * r*r)) / (PI * 2 * r*r));
					val += scl[y*w + x] * wght;  wsum += wght;
				}
			}
			tcl[i*w + j] = (uchar)std::round(val / wsum);
		}
	}
}*/
/*std::vector<Color> Tetris::FastBlur(const std::vector<Color>& input, const int w, const int h, const int radius)
{
	assert(radius >= 1);

	std::vector<Color> output(w, h);

	const unsigned int wm = w - 1;
	const unsigned int hm = h - 1;
	const unsigned int wh = w * h;

	const unsigned int div = radius + radius + 1;

	std::vector<unsigned int> r(wh);
	std::vector<unsigned int> g(wh);
	std::vector<unsigned int> b(wh);
	std::vector<unsigned int> vmin(wh);
	std::vector<unsigned int> vmax(wh);
	std::vector<unsigned int> pix(wh);

	for (size_t y = 0; y < h; y++)
	{
		for (size_t x = 0; x < w; x++)
		{
			size_t i = y * w + x;
			pix[i] = input[i].dword;
		}
	}
	
	size_t size = 256;

	std::vector<unsigned int> dv(size * div);

	for (int i = 0; i < size * div; i++)
	{
		dv[i] = (i / div);
	}

	size_t yw = 0;
	size_t yi = 0;

	for (int y = 0; y < h; y++)
	{
		unsigned int rsum = 0;
		unsigned int gsum = 0;
		unsigned int bsum = 0;


		for (int i = -radius; i <= radius; i++)
		{
			const int p = pix[yi + std::min<int>(wm, std::max<int>(i, 0))];

			rsum += (p & 0xff0000) >> 16;
			gsum += (p & 0x00ff00) >> 8;
			bsum += p & 0x0000ff;
		}

		for (int x = 0; x < w; x++)
		{
			r[yi] = dv[rsum];
			g[yi] = dv[gsum];
			b[yi] = dv[bsum];

			if (y == 0)
			{
				vmin[x] = std::min<int>(x + radius + 1, wm);
				vmax[x] = std::max<int>(x - radius, 0);
			}

			const int p1 = pix[yw + vmin[x]];
			const int p2 = pix[yw + vmax[x]];

			rsum += ((p1 & 0xff0000) - (p2 & 0xff0000)) >> 16;
			gsum += ((p1 & 0x00ff00) - (p2 & 0x00ff00)) >> 8;
			bsum += (p1 & 0x0000ff) - (p2 & 0x0000ff);

			yi++;
		}
		yw += w;
	}

	for (int x = 0; x < w; x++)
	{
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;

		int yp = -radius * w;

		for (int i = -radius; i <= radius; i++)
		{
			yi = std::max<int>(0, yp) + x;
			rsum += r[yi];
			gsum += g[yi];
			bsum += b[yi];
			yp += w;
		}

		yi = x;

		for (int y = 0; y < h; y++)
		{
			pix[yi] = 0xff000000 | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];

			if (x == 0)
			{
				vmin[y] = std::min<int>(y + radius + 1, hm) * w;
				vmax[y] = std::max<int>(y - radius, 0) * w;
			}

			const int p1 = x + vmin[y];
			const int p2 = x + vmax[y];

			rsum += r[p1] - r[p2];
			gsum += g[p1] - g[p2];
			bsum += b[p1] - b[p2];

			yi += w;
		}
	}
	
	for (unsigned int i = 0; i < wh; i++)
	{
		output[i].dword = pix[i];
	}

	return std::move(output);
}*/
/*Surface Tetris::FastBlur(const Surface& input, int radius)
{
	assert(radius >= 1);

	int w = input.GetWidth();
	int h = input.GetHeight();

	assert(w >= 1 && h >= 1);

	Surface output(w,h);

	const int wm = w - 1;
	const int hm = h - 1;
	const int wh = w * h;

	const int div = radius + radius + 1;

	std::vector<unsigned int> r(wh);
	std::vector<unsigned int> g(wh);
	std::vector<unsigned int> b(wh);
	std::vector<unsigned int> vmin(wh);
	std::vector<unsigned int> vmax(wh);
	std::vector<unsigned int> pix(wh);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			int i = y * w + x;
			pix[i] = input.GetBufferPtrConst()[i].dword;
		}
	}

	size_t size = 256;

	std::vector<int> dv(size * div);

	for (int i = 0; i < size * div; i++)
	{
		dv[i] = (i / div);
	}

	size_t yw = 0;

	for (int y = 0; y < h; y++)
	{
		unsigned int rsum = 0;
		unsigned int gsum = 0;
		unsigned int bsum = 0;

		size_t yi = 0;

		for (int i = -radius; i <= radius; i++)
		{
			const int p = pix[yi + std::min<int>(wm, std::max<int>(i, 0))];

			rsum += (p & 0xff0000) >> 16;
			gsum += (p & 0x00ff00) >> 8;
			bsum += p & 0x0000ff;
		}

		for (int x = 0; x < w; x++)
		{
			r[yi] = dv[rsum];
			g[yi] = dv[gsum];
			b[yi] = dv[bsum];

			if (y == 0)
			{
				vmin[x] = std::min<int>(x + radius + 1, wm);
				vmax[x] = std::max<int>(x - radius, 0);
			}

			const int p1 = pix[yw + vmin[x]];
			const int p2 = pix[yw + vmax[x]];

			rsum += ((p1 & 0xff0000) - (p2 & 0xff0000)) >> 16;
			gsum += ((p1 & 0x00ff00) - (p2 & 0x00ff00)) >> 8;
			bsum += (p1 & 0x0000ff) - (p2 & 0x0000ff);

			yi++;
		}
		yw += w;
	}

	for (int x = 0; x < w; x++)
	{
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;

		int yi = 0;
		int yp = -radius * w;

		for (int i = -radius; i <= radius; i++)
		{
			yi = std::max<int>(0, yp) + x;
			rsum += r[yi];
			gsum += g[yi];
			bsum += b[yi];
			yp += w;
		}

		yi = x;

		for (int y = 0; y < h; y++)
		{
			pix[yi] = 0xff000000 | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];

			if (x == 0)
			{
				vmin[y] = std::min<int>(y + radius + 1, hm) * w;
				vmax[y] = std::max<int>(y - radius, 0) * w;
			}

			const int p1 = x + vmin[y];
			const int p2 = x + vmax[y];

			rsum += r[p1] - r[p2];
			gsum += g[p1] - g[p2];
			bsum += b[p1] - b[p2];

			yi += w;
		}
	}

	for (int i = 0; i < wh; i++)
	{
		output.GetBufferPtr()[i].dword = pix[i];
	}

	return std::move(output);
}*/
/*void Tetris::superFastBlur(unsigned char* pix, int w, int h, int radius)
{
	if (radius < 1) return;

	int wm = w - 1;
	int hm = h - 1;
	int wh = w * h;
	int div = radius + radius + 1;

	unsigned char* r = new unsigned char[wh];
	unsigned char* g = new unsigned char[wh];
	unsigned char* b = new unsigned char[wh];

	int rsum, gsum, bsum, x, y, i, p, p1, p2, yp, yi, yw;

	int* vMIN = new int[std::max<int>(w, h)];
	int* vMAX = new int[std::max<int>(w, h)];

	unsigned char* dv = new unsigned char[256 * div];

	for (i = 0; i < 256 * div; i++) dv[i] = (i / div);

	yw = yi = 0;

	for (y = 0; y < h; y++) {
		rsum = gsum = bsum = 0;
		for (i = -radius; i <= radius; i++) 
		{
			p = (yi + std::min<int>(wm, std::max<int>(i, 0))) * 3;
			rsum += pix[p];
			gsum += pix[p + 1];
			bsum += pix[p + 2];
		}
		for (x = 0; x < w; x++) {

			r[yi] = dv[rsum];
			g[yi] = dv[gsum];
			b[yi] = dv[bsum];

			if (y == 0) {
				vMIN[x] = std::min<int>(x + radius + 1, wm);
				vMAX[x] = std::max<int>(x - radius, 0);
			}
			p1 = (yw + vMIN[x]) * 3;
			p2 = (yw + vMAX[x]) * 3;

			rsum += pix[p1] - pix[p2];
			gsum += pix[p1 + 1] - pix[p2 + 1];
			bsum += pix[p1 + 2] - pix[p2 + 2];

			yi++;
		}
		yw += w;
	}

	for (x = 0; x < w; x++) 
	{
		rsum = gsum = bsum = 0;
		yp = -radius * w;
		for (i = -radius; i <= radius; i++) 
		{
			yi = std::max<int>(0, yp) + x;
			rsum += r[yi];
			gsum += g[yi];
			bsum += b[yi];
			yp += w;
		}
		yi = x;
		for (y = 0; y < h; y++) 
		{
			pix[yi * 3] = dv[rsum];
			pix[yi * 3 + 1] = dv[gsum];
			pix[yi * 3 + 2] = dv[bsum];
			if (x == 0) {
				vMIN[y] = std::min<int>(y + radius + 1, hm) * w;
				vMAX[y] = std::max<int>(y - radius, 0) * w;
			}
			p1 = x + vMIN[y];
			p2 = x + vMAX[y];

			rsum += r[p1] - r[p2];
			gsum += g[p1] - g[p2];
			bsum += b[p1] - b[p2];

			yi += w;
		}
	}

	delete[] r;
	delete[] g;
	delete[] b;

	delete[] vMIN;
	delete[] vMAX;
	delete[] dv;
}*/