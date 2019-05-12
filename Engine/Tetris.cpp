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
	InitialiseButtons();
	InitialiseSettingsBox();
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
	InputMouse();
	InputKeyboard();
	Pause();
	Reset();
	Quit();
	Settings();
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
	DrawButtons();
	DrawBox();
}

/*-------------------------------------------*/

void Tetris::InitialiseTextures()
{
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Blocks3DRainbow.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksBlue.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksGreen.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature0.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature2.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature5.png"));
	texture_Background.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Space1.png"));

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

	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_White.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_White.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_White.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_White.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_White.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_White.png"));

	/*button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_Red.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_Green.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_Blue.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_Yellow.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_Magenta.png"));
	button_textures.push_back(Surface::FromFile(L"Textures\\Keys\\Button_Cyan.png"));*/

	quit_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\quit_green.png"));
	quit_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\quit_red.png"));

	settings_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\cog_white.png"));
	settings_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\cog_green.png"));

	sound_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_on_white.png"));
	sound_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_on_green.png"));

	sound_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_off_white.png"));
	sound_button_textures.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_off_green.png"));

	box_texture.push_back(Surface::FromFile(L"Textures\\Box.png"));
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
void Tetris::InitialiseButtons()
{
	const uint top = (scrH / 2u) + ((fieldH / 2u) * blockH) - (buttonH * 2u);
	const uint left = (scrW / 2u) + ((fieldW / 2u) * blockW) + buttonW;

	{
		uint t0 = top + (buttonH * 0u);
		uint t1 = top + (buttonH * 1u);

		uint b0 = top + (buttonH * 1u) - 1u;
		uint b1 = top + (buttonH * 2u) - 1u;

		uint l0 = left + (buttonW * 0u);
		uint l1 = left + (buttonW * 1u);
		uint l2 = left + (buttonW * 2u);

		uint r0 = left + (buttonW * 1u) - 1u;
		uint r1 = left + (buttonW * 2u) - 1u;
		uint r2 = left + (buttonW * 3u) - 1u;

		RectUI top_left = { t0,b0,l0,r0 };
		RectUI top_middle = { t0,b0,l1,r1 };
		RectUI top_right = { t0,b0,l2,r2 };

		RectUI bottom_left = { t1,b1,l0,r0 };
		RectUI bottom_middle = { t1,b1,l1,r1 };
		RectUI bottom_right = { t1,b1,l2,r2 };

		/*
		l m r
		e i i
		f d g
		t d h
		  l t
		  e

		0 1 2 top
		3 4 5 bottom
		*/

		button_rects.emplace_back(top_left);
		button_rects.emplace_back(top_middle);
		button_rects.emplace_back(top_right);

		button_rects.emplace_back(bottom_left);
		button_rects.emplace_back(bottom_middle);
		button_rects.emplace_back(bottom_right);
	}

	/*-----------------------------------------------------------*/

	{
		const uint offset = 5u;

		uint t0 = top + (buttonH * 0u) + offset;
		uint t1 = top + (buttonH * 1u) + offset;

		uint b0 = top + (buttonH * 1u) - offset - 1u;
		uint b1 = top + (buttonH * 2u) - offset - 1u;

		uint l0 = left + (buttonW * 0u) + offset;
		uint l1 = left + (buttonW * 1u) + offset;
		uint l2 = left + (buttonW * 2u) + offset;

		uint r0 = left + (buttonW * 1u) - offset - 1u;
		uint r1 = left + (buttonW * 2u) - offset - 1u;
		uint r2 = left + (buttonW * 3u) - offset - 1u;

		RectUI top_left = { t0,b0,l0,r0 };
		RectUI top_middle = { t0,b0,l1,r1 };
		RectUI top_right = { t0,b0,l2,r2 };

		RectUI bottom_left = { t1,b1,l0,r0 };
		RectUI bottom_middle = { t1,b1,l1,r1 };
		RectUI bottom_right = { t1,b1,l2,r2 };

		/*
		l m r
		e i i
		f d g
		t d h
		  l t
		  e

		0 1 2 top
		3 4 5 bottom
		*/

		rect_KeyPressed.push_back(top_left);
		rect_KeyPressed.push_back(top_middle);
		rect_KeyPressed.push_back(top_right);

		rect_KeyPressed.push_back(bottom_left);
		rect_KeyPressed.push_back(bottom_middle);
		rect_KeyPressed.push_back(bottom_right);
	}
		
	for (int i = 0; i < button_rects.size(); i++)
	{
		Block blockA(button_rects[i], &button_textures[i]);
		Block blockB(button_rects[i], &button_textures[i]);

		buttons.emplace_back(Button(false,blockA,blockB));
	}

	/*-----------------------------------------------------------*/
	
	/* t = top, b = bottom, l = left, r = right */

	{
		const int t = buttonH * 0u;
		const int b = buttonH * 1u;
		const int l = buttonW * 0u;
		const int r = buttonW * 1u;

		RectI rect = { t,b,l,r };
		button_rects.emplace_back(rect);
	}

	{
		const int t = (scrH - 1u) - (buttonH * 1u);
		const int b = (scrH - 1u) - (buttonH * 0u);
		const int l = buttonW * 0u;
		const int r = buttonW * 1u;

		RectI rect = { t,b,l,r };
		button_rects.emplace_back(rect);
	}

	{
		const int t = (scrH - 1u) - (buttonH * 2u);
		const int b = (scrH - 1u) - (buttonH * 1u);
		const int l = buttonW * 0u;
		const int r = buttonW * 1u;

		RectI rect = { t,b,l,r };
		button_rects.emplace_back(rect);
	}

	/*-----------------------------------------------------------*/

	{
		const Block block_a = Block(button_rects[QUIT], &quit_button_textures[0]);
		const Block block_b = Block(button_rects[QUIT], &quit_button_textures[1]);
		buttons.emplace_back(Button(false, block_a, block_b));
	}
	
	{
		const Block block_a = Block(button_rects[SETTINGS], &settings_button_textures[0]);
		const Block block_b = Block(button_rects[SETTINGS], &settings_button_textures[1]);
		buttons.emplace_back(Button(false, block_a, block_b));
	}
	
	{
		const Block block_a = Block(button_rects[SOUND], &sound_button_textures[0]);
		const Block block_b = Block(button_rects[SOUND], &sound_button_textures[1]);
		buttons.emplace_back(Button(false,block_a,block_b));
	}

	mouseOverButton.assign(buttons.size(), false);
	mousePressButton.assign(buttons.size(), false);
}
void Tetris::InitialiseSettingsBox()
{
	RectUI pos = {
		(scrH / 2u) - (box_texture[0].GetHeight() / 2u),
		(scrH / 2u) + (box_texture[0].GetHeight() / 2u),
		(scrW / 2u) - (box_texture[0].GetWidth() / 2u),
		(scrW / 2u) + (box_texture[0].GetWidth() / 2u) };
	box_block = Block(pos, &box_texture[0]);
}

/*-------------------------------------------*/

void Tetris::InputMouse()
{
	if (mouse.IsInWindow())
	{
		const uint mouseX = mouse.GetPosX();
		const uint mouseY = mouse.GetPosY();
		const bool leftIsPressed = mouse.LeftIsPressed();

		/*-------------------------------------------*/

		for (size_t i = 0u; i < buttons.size(); i++)
		{
			const bool fitsLeft		= mouseX >=	button_rects[i].left;
			const bool fitsRight	= mouseX <	button_rects[i].right;
			const bool fitsTop		= mouseY >=	button_rects[i].top;
			const bool fitsBottom	= mouseY <	button_rects[i].bottom;
			
			if (fitsLeft && fitsRight && fitsTop && fitsBottom)
			{
				mouseOverButton[i] = true;

				buttons[i].SetState(true);

				if (mouseOverButton[i] && leftIsPressed)
				{
					mousePressButton[i] = true;
				}
				else
				{
					mousePressButton[i] = false;
				}
			}
			else
			{
				mouseOverButton[i] = false;
				buttons[i].SetState(false);
			}
		}
	}
}
void Tetris::InputKeyboard()
{
	if (!gameIsPaused && !gameIsOver)
	{
		const bool tick = frameCounter > 0u && frameCounter % 20u == 0u;
				 	
		if (!keyIsPressed_LEFT)
		{
			if (kbd.KeyIsPressed(VK_LEFT) || mousePressButton[LEFT])
			{
				if (DoesTetroFit(0, MOVE_LEFT, 0))
				{
					currentX--;
				}
				keyIsPressed_LEFT = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_LEFT) && !mousePressButton[LEFT])
			{
				keyIsPressed_LEFT = false;
			}
		}

		if (!keyIsPressed_RIGHT)
		{
			if (kbd.KeyIsPressed(VK_RIGHT) || mousePressButton[RIGHT])
			{
				if (DoesTetroFit(0, MOVE_RIGHT, 0))
				{
					currentX++;
				}
				keyIsPressed_RIGHT = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_RIGHT) && !mousePressButton[RIGHT])
			{
				keyIsPressed_RIGHT = false;
			}
		}

		if (!keyIsPressed_DOWN)
		{
			if (kbd.KeyIsPressed(VK_DOWN) || mousePressButton[DOWN])
			{
				if (DoesTetroFit(0, 0, MOVE_DOWN))
				{
					currentY++;
				}
				keyIsPressed_DOWN = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_DOWN) && !mousePressButton[DOWN])
			{
				keyIsPressed_DOWN = false;
			}
		}		

		if (!keyIsPressed_UP)
		{
			if (kbd.KeyIsPressed(VK_UP) || mousePressButton[UP])
			{
				if (DoesTetroFit(ROTATE_CW, 0, 0))
				{
					currentRotation++;
					sound_move.Play(frequency, volume);
				}

				keyIsPressed_UP = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_UP) && !mousePressButton[UP])
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
			if (kbd.KeyIsPressed(VK_SPACE) || mousePressButton[PAUSE])
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
			if (!kbd.KeyIsPressed(VK_SPACE) && !mousePressButton[PAUSE])
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
		if (mousePressButton[RESTART] || kbd.KeyIsPressed(VK_RETURN))
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
	if (!mouseIsPressed)
	{
		if (mousePressButton[SETTINGS])
		{
			if (button_Settings_SHOW)
			{				
				button_Settings_SHOW = false;				
			}
			else
			{				
				button_Settings_SHOW = true;
				gameIsPaused = true;				
			}

			mouseIsPressed = true;
		}
	}
	else
	{
		if (!mouse.LeftIsPressed())
		{
			mouseIsPressed = false;
		}
	}

	if (!gameIsPaused)
	{
		button_Settings_SHOW = false;
	}
}
void Tetris::Sound()
{
	if (!mouseIsPressed)
	{
		if (mousePressButton[SOUND])
		{
			if (button_Volume_FULL)
			{
				buttons[SOUND].SetTextures(&sound_button_textures[0], &sound_button_textures[1]);
				button_Volume_FULL = false;
				volume = 0.0f;
			}
			else
			{
				buttons[SOUND].SetTextures(&sound_button_textures[2], &sound_button_textures[3]);
				button_Volume_FULL = true;
				volume = 1.0f;
			}

			mouseIsPressed = true;
		}
	}
	else
	{
		if (!mouse.LeftIsPressed())
		{
			mouseIsPressed = false;
		}
	}
}

void Tetris::SetButton(BUTTON btn)
{
	if (!mouseIsPressed)
	{
		if (mousePressButton[btn])
		{
			if (true)
			{
				buttons[btn].SetTextures(&button_textures[btn], &button_textures[btn + 1]);
				
				
			}
			else
			{
				buttons[btn].SetTextures(&button_textures[btn + 2], &button_textures[btn + 3]);
				
				
			}

			mouseIsPressed = true;
		}
	}
	else
	{
		if (!mouse.LeftIsPressed())
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
void Tetris::DrawButtons()
{
	if (true)
	{
		std::vector<Color> buttonColor(buttons.size(), Colors::White);

		if (mouse.IsInWindow())
		{
			int mouseX = mouse.GetPosX();
			int mouseY = mouse.GetPosY();

			for (int i = 0; i < buttons.size(); i++)
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

		for (int i = 0; i < buttons.size(); i++)
		{
			gfx.DrawRect(button_rects[i], buttonColor[i]);
		}
	}

	for ( auto b : buttons )
	{
		b.Draw(gfx);
	}
}

void Tetris::DrawBox()
{
	if (button_Settings_SHOW)
	{
		box_block.Draw(gfx);
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