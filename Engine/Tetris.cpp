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
#include "Bumble.h"

using namespace rnd;

using uint	= unsigned int;
using uchar = unsigned char;

using Time	= std::chrono::steady_clock;
using ms	= std::chrono::microseconds;

Tetris::Tetris(Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	:
	kbd(kbd),
	mouse(mouse),
	gfx(gfx)
{
	Initialise();
	Setup();
}

/*-------------------------------------------*/

void Tetris::Initialise()
{
	InitialiseTextures();
	InitialiseBackground();
	InitialiseField();
	InitialiseTetrominos();
	InitialiseNextTetro();
	InitialiseCounter();
	InitialiseScore();
	InitialisePause();
	InitialiseGameOver();
	InitialiseKeys();
	InitialiseButtons();
	InitialiseSettingsBox();
	InitialiseSounds();
}
void Tetris::Setup()
{	
	gameIsPaused	= false;
	gameIsOver		= false;
	nextTetro		= RandomInt(0u, 6u);
	currentTetro	= RandomInt(0u, 6u);
	currentX		= FIELD_COLS / 2u - 2u;
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
	SetKeysWithMouse();
	SetButtonsWithMouse();
	InputKeyboard();
	Pause();
	Reset(); 
	Quit();
	Settings();
	Volume();
}

/*-------------------------------------------*/

void Tetris::Update()
{	
	Input();

	if (!gameIsOver && !gameIsPaused)
	{
		if (frameCounter == speed)
		{
			if (DoesTetroFit(0,0,MOVE::DOWN))
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
				if (gameIsOver) sounds[SOUND::GAME_OVER].Play(frequency, volume);
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
	DrawBox();
}

/*-------------------------------------------*/

void Tetris::InitialiseTextures()
{
	// BLOCK TEXTURES
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_DarkGrey.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Orange.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Cyan.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Green.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Red.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Blue.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Magenta.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Yellow.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Red.png"));
	block_textures.push_back(Surface::FromFile(L"Textures\\Blocks\\Block_Grey.png"));	
	
	// WORD TEXTURES
	pause_textures.push_back(Surface::FromFile(L"Textures\\Words\\Pause.png"));
	gameover_textures.push_back(Surface::FromFile(L"Textures\\Words\\GameOver.png"));

	// DIGIT TEXTURES
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_0.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_1.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_2.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_3.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_4.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_5.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_6.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_7.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_8.png"));
	digit_textures.push_back(Surface::FromFile(L"Textures\\Digits\\digit_9.png"));

	// KEY TEXTURES
	key_textures.push_back(Surface::FromFile(L"Textures\\Keys\\key_restart.png"));
	key_textures.push_back(Surface::FromFile(L"Textures\\Keys\\key_up.png"));
	key_textures.push_back(Surface::FromFile(L"Textures\\Keys\\key_pause.png"));
	key_textures.push_back(Surface::FromFile(L"Textures\\Keys\\key_left.png"));
	key_textures.push_back(Surface::FromFile(L"Textures\\Keys\\key_down.png"));
	key_textures.push_back(Surface::FromFile(L"Textures\\Keys\\key_right.png"));
	
	// BUTTON TEXTURES
	button_texture_a.push_back(Surface::FromFile(L"Textures\\Buttons\\quit_green.png"));
	button_texture_b.push_back(Surface::FromFile(L"Textures\\Buttons\\quit_red.png"));

	button_texture_a.push_back(Surface::FromFile(L"Textures\\Buttons\\cog_white.png"));
	button_texture_b.push_back(Surface::FromFile(L"Textures\\Buttons\\cog_green.png"));

	button2_texture_a.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_on_white.png"));
	button2_texture_a.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_on_green.png"));

	button2_texture_b.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_off_white.png"));
	button2_texture_b.push_back(Surface::FromFile(L"Textures\\Buttons\\sound_off_green.png"));
	
	// SETTINGS TEXTURE
	box_textures.push_back(Surface::FromFile(L"Textures\\Box.png"));
}
void Tetris::InitialiseBackground()
{
	// BACKGROUND TEXTURES
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Blocks3DRainbow.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksBlue.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksGreen.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature0.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature2.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature5.png"));
	background_textures.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Space1.png"));

	background_position = { 0u,SCREEN_H-1u,0u,SCREEN_W-1u };	
	uint min = 0u;
	uint max = static_cast<uint>(background_textures.size()) - 1u;
	current_background = RandomInt(min, max);
	background_block = Block(background_position, &background_textures[current_background]);
}
void Tetris::InitialiseField()
{
	const unsigned int TOP		= (SCREEN_H / 2u) - ((FIELD_ROWS / 2u) * BLOCK_H);
	const unsigned int BOTTOM	= (SCREEN_H / 2u) + ((FIELD_ROWS / 2u) * BLOCK_H);
	const unsigned int LEFT		= (SCREEN_W / 2u) - ((FIELD_COLS / 2u) * BLOCK_W);
	const unsigned int RIGHT	= (SCREEN_W / 2u) + ((FIELD_COLS / 2u) * BLOCK_W);

	field_position = { TOP,BOTTOM,LEFT,RIGHT };

	for (uint y = 0; y < FIELD_ROWS; y++)
	{
		for (uint x = 0; x < FIELD_COLS; x++)
		{
			const RectUI position = RectUI(
				TOP		+ (y * BLOCK_H),
				TOP		+ (y * BLOCK_H) + BLOCK_H,
				LEFT	+ (x * BLOCK_W),
				LEFT	+ (x * BLOCK_W) + BLOCK_W);
			
			field_blocks[y][x] = Block(position, &block_textures[0]);
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
	const unsigned int TOP	= (SCREEN_H / 2u) - ((FIELD_ROWS / 2u) * BLOCK_H);
	const unsigned int LEFT = (SCREEN_W / 2u) - ((FIELD_COLS / 2u) * BLOCK_W);

	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{
			RectUI position = RectUI(
				TOP		+ (y * BLOCK_H),
				TOP		+ (y * BLOCK_H) + BLOCK_H,
				LEFT	+ (x * BLOCK_W) + (BLOCK_W * FIELD_COLS),
				LEFT	+ (x * BLOCK_W) + (BLOCK_W * FIELD_COLS) + BLOCK_W);

			next_tetro_blocks[y][x] = Block(position, &block_textures[0]);
		}
	}
}
void Tetris::InitialiseScore()
{	
	for (uint y = 0; y < SCORE_ROWS; y++)
	{
		for (uint x = 0; x < SCORE_COLS; x++)
		{
			RectUI position = RectUI(
				(DIGIT_H / 5u),
				(DIGIT_H / 5u) + DIGIT_H,
				(SCREEN_W - 1u) - (DIGIT_W * x) - (DIGIT_W / 5u * (x + 1u)) - DIGIT_W,
				(SCREEN_W - 1u) - (DIGIT_W * x) - (DIGIT_W / 5u * (x + 1u)));

			score_blocks[y][x] = Block(position, &digit_textures[y]);
		}
	}	
}
void Tetris::InitialisePause()
{
	pause_position = RectUI(
		(SCREEN_H / 2u) - (PAUSE_H / 2u),
		(SCREEN_H / 2u) + (PAUSE_H / 2u),
		(SCREEN_W / 2u) - (PAUSE_W / 2u),
		(SCREEN_W / 2u) + (PAUSE_W / 2u));

	const size_t SIZE	= pause_textures.size();
	const size_t MIN	= 0;
	const size_t MAX	= SIZE - 1;
	const size_t INDEX = (SIZE > 1) ? RandomInt(MIN, MAX) : 0;

	pause_block = Block(pause_position, &pause_textures[INDEX]);
}
void Tetris::InitialiseGameOver()
{
	gameover_position = RectUI(
		(SCREEN_H / 2u) - (GAMEOVER_H / 2u),
		(SCREEN_H / 2u) + (GAMEOVER_H / 2u),
		(SCREEN_W / 2u) - (GAMEOVER_W / 2u),
		(SCREEN_W / 2u) + (GAMEOVER_W / 2u));

	const size_t SIZE = gameover_textures.size();
	const size_t MIN = 0;
	const size_t MAX = SIZE - 1;
	const size_t INDEX = (SIZE > 1) ? RandomInt(MIN,MAX) : 0;

	gameover_block = Block(gameover_position, &gameover_textures[INDEX]);
}
void Tetris::InitialiseCounter()
{
	for (uint i = 0; i < COUNT_NUM; i++)
	{
			RectI position = RectI( 
				(SCREEN_H / 2u) - (DIGIT_H / 2u),
				(SCREEN_H / 2u) + (DIGIT_H / 2u),
				(SCREEN_W / 2u) - ((FIELD_COLS / 2u) * BLOCK_W) - (DIGIT_W * 3u),
				(SCREEN_W / 2u) - ((FIELD_COLS / 2u) * BLOCK_W) - (DIGIT_W * 2u));

			counter_blocks[i] = Block(position, &digit_textures[i]);	
	}
}
void Tetris::InitialiseKeys()
{
	/*
		l m r
		e i i
		f d g
		t d HEIGHT
		  l t
		  e

		0 1 2  TOP
		3 4 5  BOTTOM

		0 = restart	1 = rotate cw	2 = pause		
		3 = LEFT	4 = down		5 RIGHT
	*/

	/*
		key layout

		T0,L0----  T0,L1----  T0,L2----
		|.......|  |.......|  |.......|
		|restart|  |rotate.|  |.pause.| TOP row
		|.......|  |.......|  |.......|
		----T1,R0  ----T0,R1  ----T0,R2

		B0,L0----  B0,L1----  B0,L2----
		|.......|  |.......|  |.......|
		|.LEFT..|  |.down..|  |.RIGHT.| BOTTOM row
		|.......|  |.......|  |.......|
		----B1,R0  ----B0,R1  ----B0,R2
	*/

	// TOP = the BOTTOM of the tetris playing field then up by two button heights
	// LEFT = the RIGHT of the playing field then RIGHT by one button width

	const uint TOP	= (SCREEN_H / 2u) + ((FIELD_ROWS / 2u) * BLOCK_H) - (KEY_H * 2u);
	const uint LEFT = (SCREEN_W / 2u) + ((FIELD_COLS / 2u) * BLOCK_W) + KEY_W;

	// Key state a - key not pressed / default state
	// mouse over doesn't change the state of the key currently

	const uint T0 = TOP + (KEY_H * 0u);
	const uint T1 = TOP + (KEY_H * 1u) - 1u;

	const uint B0 = TOP + (KEY_H * 1u);
	const uint B1 = TOP + (KEY_H * 2u) - 1u;

	const uint L0 = LEFT + (KEY_W * 0u);
	const uint L1 = LEFT + (KEY_W * 1u);
	const uint L2 = LEFT + (KEY_W * 2u);

	const uint R0 = LEFT + (KEY_W * 1u) - 1u;
	const uint R1 = LEFT + (KEY_W * 2u) - 1u;
	const uint R2 = LEFT + (KEY_W * 3u) - 1u;

	{
		RectUI top_left			= { T0,T1,L0,R0 };
		RectUI top_middle		= { T0,T1,L1,R1 };
		RectUI top_right		= { T0,T1,L2,R2 };

		RectUI bottom_left		= { B0,B1,L0,R0 };
		RectUI bottom_middle	= { B0,B1,L1,R1 };
		RectUI bottom_right		= { B0,B1,L2,R2 };

		key_position_a.emplace_back(std::move(top_left));
		key_position_a.emplace_back(std::move(top_middle));
		key_position_a.emplace_back(std::move(top_right));

		key_position_a.emplace_back(std::move(bottom_left));
		key_position_a.emplace_back(std::move(bottom_middle));
		key_position_a.emplace_back(std::move(bottom_right));
	}
	
	
	// Key state b - key pressed state

	const uint OFFSET = 5u;
	
	{
		RectUI top_left			= { T0 + OFFSET,T1 - OFFSET,L0 + OFFSET,R0 - OFFSET };
		RectUI top_middle		= { T0 + OFFSET,T1 - OFFSET,L1 + OFFSET,R1 - OFFSET };
		RectUI top_right		= { T0 + OFFSET,T1 - OFFSET,L2 + OFFSET,R2 - OFFSET };

		RectUI bottom_left		= { B0 + OFFSET,B1 - OFFSET,L0 + OFFSET,R0 - OFFSET };
		RectUI bottom_middle	= { B0 + OFFSET,B1 - OFFSET,L1 + OFFSET,R1 - OFFSET };
		RectUI bottom_right		= { B0 + OFFSET,B1 - OFFSET,L2 + OFFSET,R2 - OFFSET };

		key_position_b.emplace_back(std::move(top_left));
		key_position_b.emplace_back(std::move(top_middle));
		key_position_b.emplace_back(std::move(top_right));

		key_position_b.emplace_back(std::move(bottom_left));
		key_position_b.emplace_back(std::move(bottom_middle));
		key_position_b.emplace_back(std::move(bottom_right));
	}

	assert(key_position_a.size() == key_position_b.size());

	const size_t size = key_position_a.size();

	for (uint i = 0; i < size; i++)
	{
		Button new_key(
			false,
			Block(key_position_a[i], &key_textures[i]),
			Block(key_position_b[i], &key_textures[i]));

		key.emplace_back(std::move(new_key));
	}

	mouseOverKey.assign(key.size(), false);
	mousePressKey.assign(key.size(), false);
}
void Tetris::InitialiseButtons()
{
	{
		// QUIT BUTTON

		const uint TOP		= BUTTON_H * 0u;
		const uint BOTTOM	= BUTTON_H * 1u;
		const uint LEFT		= BUTTON_W * 0u;
		const uint RIGHT	= BUTTON_W * 1u;

		RectUI position = { TOP,BOTTOM,LEFT,RIGHT };
		button_position.emplace_back(std::move(position));

		Block block_a(button_position[BUTTON::QUIT], &button_texture_a[BUTTON::QUIT]);
		Block block_b(button_position[BUTTON::QUIT], &button_texture_b[BUTTON::QUIT]);
		
		Button button_quit(false, std::move(block_a), std::move(block_b));
		button.emplace_back(std::move(button_quit));
	}
	
	{
		// SETTINGS BUTTON

		const uint TOP		= (SCREEN_H - 1u) - (BUTTON_H * 1u);
		const uint BOTTOM	= (SCREEN_H - 1u) - (BUTTON_H * 0u);
		const uint LEFT		= BUTTON_W * 0u;
		const uint RIGHT	= BUTTON_W * 1u;

		RectUI position = { TOP,BOTTOM,LEFT,RIGHT };
		button_position.emplace_back(std::move(position));

		Block block_a(button_position[BUTTON::SETTINGS], &button_texture_a[BUTTON::SETTINGS]);
		Block block_b(button_position[BUTTON::SETTINGS], &button_texture_b[BUTTON::SETTINGS]);		

		Button button_settings(false, std::move(block_a), std::move(block_b));
		button.emplace_back(std::move(button_settings));
	}
	
	/*{
		// VOLUME BUTTON

		const uint TOP		= (SCREEN_H - 1u) - (BUTTON_H * 2u);
		const uint BOTTOM	= (SCREEN_H - 1u) - (BUTTON_H * 1u);
		const uint LEFT		= BUTTON_W * 0u;
		const uint RIGHT	= BUTTON_W * 1u;

		RectUI rect = { TOP,BOTTOM,LEFT,RIGHT };
		button2_position_a.emplace_back(std::move(rect));
		button2_position_b.emplace_back(std::move(rect));

		Block block_a(button2_position_a[BUTTON::VOLUME], &button2_texture_a[BUTTON::VOLUME]);
		Block block_b(button2_position_a[BUTTON::VOLUME], &button2_texture_a[BUTTON::VOLUME + 1]);
		Button button_a(false, std::move(block_a), std::move(block_b));

		Block block_c(button2_position_b[BUTTON::VOLUME], &button2_texture_b[BUTTON::VOLUME]);
		Block block_d(button2_position_b[BUTTON::VOLUME], &button2_texture_b[BUTTON::VOLUME + 1]);
		Button button_b(false, std::move(block_c), std::move(block_d));

		Button2 button_volume(false, std::move(button_a), std::move(button_b));
		button2.emplace_back(std::move(button_volume));
	}*/

	mouseOverButton.assign(button.size(), false);
	mousePressButton.assign(button.size(), false);
}
void Tetris::InitialiseSounds()
{
	sounds.emplace_back(Sound(L"Sounds\\pop1.wav"));
	sounds.emplace_back(Sound(L"Sounds\\success0.wav"));
	sounds.emplace_back(Sound(L"Sounds\\success1.wav"));
	sounds.emplace_back(Sound(L"Sounds\\success2.wav"));
	sounds.emplace_back(Sound(L"Sounds\\success3.wav"));
	sounds.emplace_back(Sound(L"Sounds\\pop0.wav"));
	sounds.emplace_back(Sound(L"Sounds\\fail0.wav"));
}

void Tetris::InitialiseSettingsBox()
{
	box_block = Block(field_position, &box_textures[0]);
}

/*-------------------------------------------*/

void Tetris::SetKeysWithMouse()
{
	if (mouse.IsInWindow())
	{
		const uint mouseX = mouse.GetPosX();
		const uint mouseY = mouse.GetPosY();
		const bool leftIsPressed = mouse.LeftIsPressed();

		/*-------------------------------------------*/

		if (!mouseIsPressed)
		{
			for (size_t i = 0u; i < key.size(); i++)
			{
				const bool FITS_LEFT	= mouseX >= key_position_a[i].left;
				const bool FITS_RIGHT	= mouseX <	key_position_a[i].right;
				const bool FITS_TOP		= mouseY >= key_position_a[i].top;
				const bool FITS_BOTTOM	= mouseY <	key_position_a[i].bottom;

				if (FITS_LEFT && FITS_RIGHT && FITS_TOP && FITS_BOTTOM)
				{
					mouseOverKey[i] = true;

					if (mouseOverKey[i] && leftIsPressed)
					{
						mousePressKey[i] = true;
						key[i].SetState(true);
						mouseIsPressed = true;
					}
					else
					{
						mousePressKey[i] = false;
						key[i].SetState(false);
					}
				}
				else
				{
					mouseOverKey[i] = false;
					key[i].SetState(false);
				}
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
}
void Tetris::SetButtonsWithMouse()
{
	if (mouse.IsInWindow())
	{
		const uint mouseX = mouse.GetPosX();
		const uint mouseY = mouse.GetPosY();
		const bool leftIsPressed = mouse.LeftIsPressed();

		/*-------------------------------------------*/

		for (size_t i = 0u; i < button.size(); i++)
		{
			const bool FITS_LEFT	= mouseX >= button_position[i].left;
			const bool FITS_RIGHT	= mouseX <	button_position[i].right;
			const bool FITS_TOP		= mouseY >= button_position[i].top;
			const bool FITS_BOTTOM	= mouseY <	button_position[i].bottom;

			if (FITS_LEFT && FITS_RIGHT && FITS_TOP && FITS_BOTTOM)
			{
				mouseOverButton[i] = true;

				button[i].SetState(true);

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
				button[i].SetState(false);
			}
		}
	}
}
void Tetris::InputKeyboard()
{
	if (!gameIsPaused && !gameIsOver)
	{
		const bool TICK = frameCounter > 0u && frameCounter % 20u == 0u;
				 	
		if (!keyIsPressed_LEFT)
		{
			if (kbd.KeyIsPressed(VK_LEFT) || mousePressKey[KEY::LEFT])
			{
				if (DoesTetroFit(0, MOVE::LEFT, 0))
				{
					currentX--;
				}
				keyIsPressed_LEFT = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_LEFT) && !mousePressKey[KEY::LEFT])
			{
				keyIsPressed_LEFT = false;
			}
		}

		if (!keyIsPressed_RIGHT)
		{
			if (kbd.KeyIsPressed(VK_RIGHT) || mousePressKey[KEY::RIGHT])
			{
				if (DoesTetroFit(0, MOVE::RIGHT, 0))
				{
					currentX++;
				}
				keyIsPressed_RIGHT = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_RIGHT) && !mousePressKey[KEY::RIGHT])
			{
				keyIsPressed_RIGHT = false;
			}
		}

		if (!keyIsPressed_DOWN)
		{
			if (kbd.KeyIsPressed(VK_DOWN) || mousePressKey[KEY::DOWN])
			{
				if (DoesTetroFit(0, 0, MOVE::DOWN))
				{
					currentY++;
				}
				keyIsPressed_DOWN = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_DOWN) && !mousePressKey[KEY::DOWN])
			{
				keyIsPressed_DOWN = false;
			}
		}		

		if (!keyIsPressed_UP)
		{
			if (kbd.KeyIsPressed(VK_UP) || mousePressKey[KEY::ROTATE])
			{
				if (DoesTetroFit(MOVE::ROTATE, 0, 0))
				{
					currentRotation++;
					sounds[SOUND::ROTATE].Play(frequency, volume);
				}

				keyIsPressed_UP = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_UP) && !mousePressKey[KEY::ROTATE])
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
			if (kbd.KeyIsPressed(VK_SPACE) || mousePressKey[KEY::PAUSE])
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
			if (!kbd.KeyIsPressed(VK_SPACE) && !mousePressKey[KEY::PAUSE])
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
		if (mousePressKey[KEY::RESTART] || kbd.KeyIsPressed(VK_RETURN))
		{
			Setup();
		}
	}
}
void Tetris::Quit()
{
	if (mousePressButton[BUTTON::QUIT])
	{
		PostQuitMessage(0);
	}
}
void Tetris::Settings()
{
	if (!mouseIsPressed)
	{
		if (mousePressButton[BUTTON::SETTINGS])
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
void Tetris::Volume()
{
	if (!mouseIsPressed)
	{
		if (/*mousePressButton[BUTTON::VOLUME]*/false)
		{
			if (button_Volume_FULL)
			{				
				volume = 0.0f;
				button_Volume_FULL = false;
			}
			else
			{
				volume = 1.0f;
				button_Volume_FULL = true;
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

//void Tetris::SetButton(BUTTON btn)
//{
//	size_t button = static_cast<size_t>(btn);
//
//	if (!mouseIsPressed)
//	{
//		if (mousePressButton[button])
//		{
//			if (true)
//			{
//								
//			}
//			else
//			{
//								
//			}
//
//			mouseIsPressed = true;
//		}
//	}
//	else
//	{
//		if (!mouse.LeftIsPressed())
//		{
//			mouseIsPressed = false;
//		}
//	}
//}

/*-------------------------------------------*/

void Tetris::SetBackground()
{
	if (current_background < background_textures.size() - 1u)
	{
		current_background++;
	}
	else
	{
		current_background = 0u;
	}

	background_block.SetTexture( &background_textures[current_background] );
}
void Tetris::ResetScore()
{
	blockBuffer_Score.clear();
	score = 0u;
	ExtractDigits(blockBuffer_Score, score);
}
void Tetris::ResetField()
{
	for (uint y = 0u; y < FIELD_ROWS; y++)	
	{
		for (uint x = 0u; x < FIELD_COLS; x++)
		{
			const uint INDEX = y * FIELD_COLS + x;
			const uint VALUE = (x == 0u || x == FIELD_COLS - 1u || y == FIELD_ROWS - 1u) ? 9u : 0u;
			blockBuffer_Fixed[INDEX] = VALUE;

			//if (x == 0 || x == FIELD_COLS - 1) field[y*FIELD_COLS + x] = 9;
			//else if(y == FIELD_ROWS - 1) field[y*FIELD_COLS + x] = 9;
			//else field[y*FIELD_COLS + x] = 0;
		}
	}
}
void Tetris::SetFieldBlocks()
{
	for (uint y = 0u; y < FIELD_ROWS; y++)
	{
		for (uint x = 0u; x < FIELD_COLS; x++)
		{
			const uint INDEX_1 = y * FIELD_COLS + x;
			const uint INDEX_2 = ConvertCharToInt(blockBuffer_Shown[INDEX_1]);
			field_blocks[y][x].SetTexture(&block_textures[INDEX_2]);
		}
	}
}
void Tetris::SetNextTetro()
{
	currentX = FIELD_COLS / 2u - 2u;
	currentY = 0u;
	currentRotation = 0u;

	currentTetro = nextTetro;
	nextTetro = RandomInt(0u, 6u);

	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{	
			const uint INDEX_1 = y * TETRO_COLS + x;
			const uint INDEX_2 = (tetromino[nextTetro][INDEX_1] != '.') ? nextTetro + 1u : 0u;

			next_tetro_blocks[y][x].SetTexture(&block_textures[INDEX_2]);
		}
	}

	sounds[SOUND::NEW_TETRO].Play(frequency,volume);
}
void Tetris::SetScore()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
{
	score += 25u;

	if (!lines.empty()) score += (1u << lines.size()) * 100u;

	ExtractDigits(blockBuffer_Score, score);

	if (score % 1000u == 0u) speed -= (speed / 2u);
}
void Tetris::SetLevel()
{
	prevLevel = level;
	level++;
}
void Tetris::SetCounter()
{
	if (tickCounter < 9u)
	{
		tickCounter++;
	}
	else
	{
		tickCounter = 0u;
	}
}
void Tetris::CheckForLines()
{
	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		if (currentY + y < FIELD_ROWS - 1u)
		{
			bool isLine = true;
			for (uint x = 1u; x < FIELD_COLS - 1u; x++)
			{
				const uint INDEX = (currentY + y) * FIELD_COLS + x;
				isLine &= (blockBuffer_Fixed[INDEX] != 0);
			}

			if (isLine)
			{
				// set line to '=' (red)
				for (uint x = 1u; x < FIELD_COLS - 1u; x++)
				{
					const uint INDEX = (currentY + y) * FIELD_COLS + x;
					blockBuffer_Fixed[INDEX] = 8u;
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
			for (int x = 1; x < FIELD_COLS - 1; x++)
			{
				for (int y = v; y > 0; y--)
				{
					int i = y * FIELD_COLS + x;
					int j = (y - 1) * FIELD_COLS + x;
					blockBuffer_Fixed[i] = blockBuffer_Fixed[j];
				}
				blockBuffer_Fixed[x] = 0;
			}
		}*/

		const uint LINES_NUM = static_cast<uint>(lines.size());		
		
		for (uint x = 1u;x < FIELD_COLS - 1u; x++)
		{
			for (uint i = 0u; i < LINES_NUM; i++)
			{
				for (uint y = lines[i]; y > 0u; y--)
				{					
					const uint INDEX_1 = y * FIELD_COLS + x;
					const uint INDEX_2 = (y - 1) * FIELD_COLS + x;
					blockBuffer_Fixed[INDEX_1] = blockBuffer_Fixed[INDEX_2];
				}					
				blockBuffer_Fixed[x] = 0;
			}
		}

		lines.clear();

		switch (LINES_NUM)
		{
		case 1:
			sounds[SOUND::LINE_ONE].Play(frequency, volume);
			break;
		case 2:
			sounds[SOUND::LINE_TWO].Play(frequency, volume);
			break;
		case 3:
			sounds[SOUND::LINE_THREE].Play(frequency, volume);
			break;
		case 4:
			sounds[SOUND::LINE_FOUR].Play(frequency, volume);
			break;
		default:
			break;
		}
	}
}
void Tetris::SetFixedWithTetro()
{
	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{
			if (tetromino[currentTetro][Rotate(x, y, currentRotation)] == 'X')
			{
				const uint INDEX = (currentY + y) * FIELD_COLS + (currentX + x);
				blockBuffer_Fixed[INDEX] = currentTetro + 1u;
			}
		}
	}
}
void Tetris::SetShownWithTetro()
{
	// add current tetris piece to display buffer
	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{
			// set to the ascii table value
			if (tetromino[currentTetro][Rotate(x, y, currentRotation)] == 'X')
			{		
				const uint INDEX = (currentY + y) * FIELD_COLS + (currentX + x);
				blockBuffer_Shown[INDEX] = " ABCDEFG=#"[currentTetro + 1u];
			}
		}
	}
}
void Tetris::SetShownWithFixed()
{
	// add all fixed blocks to displayBuffer from fixedBuffer
	for (uint y = 0u; y < FIELD_ROWS; y++)
	{
		for (uint x = 0u; x < FIELD_COLS; x++)
		{
			const uint INDEX = y * FIELD_COLS + x;
			blockBuffer_Shown[INDEX] = " ABCDEFG=#"[blockBuffer_Fixed[INDEX]];
		}
	}
}

/*-------------------------------------------*/

void Tetris::DrawBackground()
{
	background_block.Draw(gfx);
}
void Tetris::DrawFieldBlocks()
{
	for (uint y = 0u; y < FIELD_ROWS; y++)
	{
		for (uint x = 0u; x < FIELD_COLS; x++)
		{
			const uint INDEX = y * FIELD_COLS + x;

			if (blockBuffer_Shown[INDEX] == ' ')
			{
				continue;
			}
			else
			{
				field_blocks[y][x].Draw(gfx);
			}
		}
	}
}
void Tetris::DrawNextTetro()
{
	if (!gameIsOver)
	{
		for (uint y = 0u; y < TETRO_ROWS; y++)
		{
			for (uint x = 0u; x < TETRO_COLS; x++)
			{
				const uint INDEX = y * TETRO_COLS + x;
				if (tetromino[nextTetro][INDEX] == '.')
				{
					continue;
				}
				else
				{
					next_tetro_blocks[y][x].Draw(gfx);
				}
			}
		}
	}
}
void Tetris::DrawCounter()
{
	if (!gameIsOver && !gameIsPaused)
	{
		counter_blocks[tickCounter].Draw(gfx);
	}
}
void Tetris::DrawBlur()
{
	if (gameIsPaused || gameIsOver) 
	{
		std::vector<Color> blur;
		blur = ConvertSurfaceToColorVector(gfx.CopySysBuffer());
				
		for (uint i = 0u; i < BLUR_NUM; i++)
		{
			blur = Blur(SCREEN_W, SCREEN_H, blur);
		}

		for (uint y = 0u; y < SCREEN_H; y++)
		{
			for (uint x = 0u; x < SCREEN_W; x++)
			{
				const uint INDEX = y * SCREEN_W + x;

				gfx.PutPixel(x, y, blur[INDEX]);
			}
		}			
	}
}
void Tetris::DrawScore()
{
	for (size_t i = 0u; i < blockBuffer_Score.size(); i++)
	{
		score_blocks[blockBuffer_Score[i]][i].Draw(gfx);
	}
}
void Tetris::DrawPause()
{
	if (gameIsPaused)
	{		
		pause_block.Draw(gfx);
	}
}
void Tetris::DrawGameOver()
{
	if (gameIsOver)
	{		
		gameover_block.Draw(gfx);
	}
}
void Tetris::DrawKeys()
{
	if (false)
	{
		std::vector<Color> keyColor(key.size(), Colors::White);

		if (mouse.IsInWindow())
		{
			int mouseX = mouse.GetPosX();
			int mouseY = mouse.GetPosY();

			for (size_t i = 0u; i < key.size(); i++)
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

		for (size_t i = 0u; i < key.size(); i++)
		{
			gfx.DrawRect(false, key_position_a[i], keyColor[i]);
		}
	}

	for (auto k : key)
	{
		k.Draw(gfx);
	}
}
void Tetris::DrawButtons()
{
	if (false)
	{
		std::vector<Color> buttonColor(button.size(), Colors::White);

		if (mouse.IsInWindow())
		{
			int mouseX = mouse.GetPosX();
			int mouseY = mouse.GetPosY();

			for (size_t i = 0u; i < button.size(); i++)
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

		for (size_t i = 0u; i < button.size(); i++)
		{
			gfx.DrawRect(false,button_position[i], buttonColor[i]);
		}
	}

	for ( auto b : button )
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
	const uint ROTATION = currentRotation + offsetRotation;
	const uint X = currentX + offsetX;
	const uint Y = currentY + offsetY;

	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{			
			const bool FITS_LEFT	= X + x >=	0u;
			const bool FITS_RIGHT	= X + x <	FIELD_COLS;
			const bool FITS_TOP		= Y + y >=	0u;
			const bool FITS_BOTTOM	= Y + y <	FIELD_ROWS;

			if ( FITS_LEFT && FITS_RIGHT && FITS_TOP && FITS_BOTTOM )
			{
				const uint INDEX_1 = Rotate(x, y, ROTATION);
				const uint INDEX_2 = (Y + y) * FIELD_COLS + (X + x);

				if (tetromino[currentTetro][INDEX_1] != L'.' && blockBuffer_Fixed[INDEX_2] != 0u)
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
	uint result = num;

	if (result > 0)
	{
		while (result > 0u)
		{
			ints.push_back(result % 10u);
			result /= 10u;
		}
	}
	else
	{
		ints.push_back(0u);
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
	for (uint i = 0u; i < 10u; i++)
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

	return 0u;
}

/*-------------------------------------------*/

std::vector<Color> Tetris::ConvertSurfaceToColorVector(Surface surface)
{
	const uint WIDTH = surface.GetWidth();
	uint HEIGHT = surface.GetHeight();

	std::vector<Color> output;

	for (uint y = 0u; y < HEIGHT; y++)
	{
		for (uint x = 0u; x < WIDTH; x++)
		{
			output.push_back(surface.GetPixel(x, y));
		}
	}

	return output;
}
std::vector<Color> Tetris::Blur(const int WIDTH,const int HEIGHT,const std::vector<Color>& input)
{
	const size_t SIZE_A = size_t(WIDTH) * HEIGHT;
	const size_t SIZE_B = input.size();
	assert( SIZE_A == SIZE_B );
		
	const float val = 1.0f / 9.0f;
	std::array<std::array<float, 3u>, 3u> box = {	val,val,val,
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

    for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			pixel_out = { 0.0f,0.0f,0.0f };

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					const int X = x + col;
					const int Y = y + row;

					if (X > 0 && X < WIDTH && Y > 0 && Y < HEIGHT)
					{
						const int INDEX = Y * WIDTH + X;
						const int ROW = row + 1;
						const int COL = col + 1;

						pixel_out.x += (input[INDEX].GetR() * box[ROW][COL]);
						pixel_out.y += (input[INDEX].GetG() * box[ROW][COL]);
						pixel_out.z += (input[INDEX].GetB() * box[ROW][COL]);
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
		std::chrono::high_resolution_clock::time_point T1 = std::chrono::high_resolution_clock::now(); 

		&pFunction;

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - T1);

		file << i << " " << duration.count() << " microseconds" << std::endl;
	}
}*/
/*void	Tetris::BoxBlur(const Surface & blur, std::vector<Color>& output)
{
	output.clear();

	const uint height = blur.GetHeight();
	const uint width = blur.GetWidth();

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
						Color c = blur.GetPixel(currentX, currentY);

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
	assert(output.size() == (blur.GetWidth() * blur.GetHeight()));
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
/*void Tetris::gaussBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int WIDTH, int HEIGHT, int r)
{
	auto bxs = boxesForGauss(r, 3);

	boxBlur_4(scl, tcl, WIDTH, HEIGHT, (bxs[0] - 1) / 2);
	boxBlur_4(tcl, scl, WIDTH, HEIGHT, (bxs[1] - 1) / 2);
	boxBlur_4(scl, tcl, WIDTH, HEIGHT, (bxs[2] - 1) / 2);
}*/
/*void Tetris::boxBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int WIDTH, int HEIGHT, int r)
{
	for (auto i = 0; i < scl.size(); i++) tcl[i] = scl[i];

	boxBlurH_4(tcl, scl, WIDTH, HEIGHT, r);
	boxBlurT_4(scl, tcl, WIDTH, HEIGHT, r);
}*/
/*void Tetris::boxBlurH_4(std::vector<uchar> scl, std::vector<uchar> tcl, int WIDTH, int HEIGHT, int r)
{
	auto iarr = 1 / (r + r + 1);

	for (auto i = 0; i < HEIGHT; i++) 
	{
		auto ti = i * WIDTH;
		auto li = ti;
		auto ri = ti + r;
		auto fv = scl[ti];
		auto lv = scl[ti + WIDTH - 1];
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

		for (auto j = r + 1; j < WIDTH - r; j++)
		{ 
			val += scl[ri++] - scl[li++];
			tcl[ti++] = (int)std::round(val*iarr);
		}

		for (auto j = WIDTH - r; j < WIDTH; j++) 
		{ 
			val += lv - scl[li++];
			tcl[ti++] = (int)std::round(val*iarr);
		}
	}
}*/
/*void Tetris::boxBlurT_4(std::vector<uchar> scl, std::vector<uchar> tcl, int WIDTH, int HEIGHT, int r)
{
	auto iarr = 1 / (r + r + 1);

	for (auto i = 0; i < WIDTH; i++) 
	{
		auto ti = i, li = ti, ri = ti + r * WIDTH;
		auto fv = scl[ti];
		auto lv = scl[ti + WIDTH * (HEIGHT - 1)];
		auto val = (r + 1)*fv;

		for (auto j = 0; j < r; j++)
		{
			val += scl[ti + j * WIDTH];
		}

		for (auto j = 0; j <= r; j++)
		{ 
			val += scl[ri] - fv;
			tcl[ti] = (int)std::round(val*iarr);
			ri += WIDTH; ti += WIDTH;
		}

		for (auto j = r + 1; j < HEIGHT - r; j++)
		{ 
			val += scl[ri] - scl[li];
			tcl[ti] = (int)std::round(val*iarr);
			li += WIDTH; ri += WIDTH; ti += WIDTH; 
		}

		for (auto j = HEIGHT - r; j < HEIGHT; j++)
		{ 
			val += lv - scl[li];  
			tcl[ti] = (int)std::round(val*iarr);
			li += WIDTH; ti += WIDTH;
		}
	}
}*/
/*void Tetris::gaussBlur_1(std::vector<uchar> scl, std::vector<uchar> tcl, int WIDTH, int HEIGHT, int r)
{
	// scl = source channel, tcl = target channel, width, height, radius

	int rs = (int)std::ceil(r * 2.57);     // significant radius

	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			int val = 0, wsum = 0;

			for (int iy = i - rs; iy < i + rs + 1; iy++)
			{
				for (int ix = j - rs; ix < j + rs + 1; ix++)
				{
					int x = std::min<int>(WIDTH - 1, std::max<int>(0, ix));
					int y = std::min<int>(HEIGHT - 1, std::max<int>(0, iy));
					int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
					int wght = (int)(std::exp(-dsq / (2 * r*r)) / (PI * 2 * r*r));
					val += scl[y*WIDTH + x] * wght;  wsum += wght;
				}
			}
			tcl[i*WIDTH + j] = (uchar)std::round(val / wsum);
		}
	}
}*/
/*std::vector<Color> Tetris::FastBlur(const std::vector<Color>& blur, const int WIDTH, const int HEIGHT, const int radius)
{
	assert(radius >= 1);

	std::vector<Color> output(WIDTH, HEIGHT);

	const unsigned int wm = WIDTH - 1;
	const unsigned int hm = HEIGHT - 1;
	const unsigned int wh = WIDTH * HEIGHT;

	const unsigned int div = radius + radius + 1;

	std::vector<unsigned int> r(wh);
	std::vector<unsigned int> g(wh);
	std::vector<unsigned int> b(wh);
	std::vector<unsigned int> vmin(wh);
	std::vector<unsigned int> vmax(wh);
	std::vector<unsigned int> pix(wh);

	for (size_t y = 0; y < HEIGHT; y++)
	{
		for (size_t x = 0; x < WIDTH; x++)
		{
			size_t i = y * WIDTH + x;
			pix[i] = blur[i].dword;
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

	for (int y = 0; y < HEIGHT; y++)
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

		for (int x = 0; x < WIDTH; x++)
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
		yw += WIDTH;
	}

	for (int x = 0; x < WIDTH; x++)
	{
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;

		int yp = -radius * WIDTH;

		for (int i = -radius; i <= radius; i++)
		{
			yi = std::max<int>(0, yp) + x;
			rsum += r[yi];
			gsum += g[yi];
			bsum += b[yi];
			yp += WIDTH;
		}

		yi = x;

		for (int y = 0; y < HEIGHT; y++)
		{
			pix[yi] = 0xff000000 | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];

			if (x == 0)
			{
				vmin[y] = std::min<int>(y + radius + 1, hm) * WIDTH;
				vmax[y] = std::max<int>(y - radius, 0) * WIDTH;
			}

			const int p1 = x + vmin[y];
			const int p2 = x + vmax[y];

			rsum += r[p1] - r[p2];
			gsum += g[p1] - g[p2];
			bsum += b[p1] - b[p2];

			yi += WIDTH;
		}
	}
	
	for (unsigned int i = 0; i < wh; i++)
	{
		output[i].dword = pix[i];
	}

	return std::move(output);
}*/
/*Surface Tetris::FastBlur(const Surface& blur, int radius)
{
	assert(radius >= 1);

	int WIDTH = blur.GetWidth();
	int HEIGHT = blur.GetHeight();

	assert(WIDTH >= 1 && HEIGHT >= 1);

	Surface output(WIDTH,HEIGHT);

	const int wm = WIDTH - 1;
	const int hm = HEIGHT - 1;
	const int wh = WIDTH * HEIGHT;

	const int div = radius + radius + 1;

	std::vector<unsigned int> r(wh);
	std::vector<unsigned int> g(wh);
	std::vector<unsigned int> b(wh);
	std::vector<unsigned int> vmin(wh);
	std::vector<unsigned int> vmax(wh);
	std::vector<unsigned int> pix(wh);

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			int i = y * WIDTH + x;
			pix[i] = blur.GetBufferPtrConst()[i].dword;
		}
	}

	size_t size = 256;

	std::vector<int> dv(size * div);

	for (int i = 0; i < size * div; i++)
	{
		dv[i] = (i / div);
	}

	size_t yw = 0;

	for (int y = 0; y < HEIGHT; y++)
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

		for (int x = 0; x < WIDTH; x++)
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
		yw += WIDTH;
	}

	for (int x = 0; x < WIDTH; x++)
	{
		int rsum = 0;
		int gsum = 0;
		int bsum = 0;

		int yi = 0;
		int yp = -radius * WIDTH;

		for (int i = -radius; i <= radius; i++)
		{
			yi = std::max<int>(0, yp) + x;
			rsum += r[yi];
			gsum += g[yi];
			bsum += b[yi];
			yp += WIDTH;
		}

		yi = x;

		for (int y = 0; y < HEIGHT; y++)
		{
			pix[yi] = 0xff000000 | (dv[rsum] << 16) | (dv[gsum] << 8) | dv[bsum];

			if (x == 0)
			{
				vmin[y] = std::min<int>(y + radius + 1, hm) * WIDTH;
				vmax[y] = std::max<int>(y - radius, 0) * WIDTH;
			}

			const int p1 = x + vmin[y];
			const int p2 = x + vmax[y];

			rsum += r[p1] - r[p2];
			gsum += g[p1] - g[p2];
			bsum += b[p1] - b[p2];

			yi += WIDTH;
		}
	}

	for (int i = 0; i < wh; i++)
	{
		output.GetBufferPtr()[i].dword = pix[i];
	}

	return std::move(output);
}*/
/*void Tetris::superFastBlur(unsigned char* pix, int WIDTH, int HEIGHT, int radius)
{
	if (radius < 1) return;

	int wm = WIDTH - 1;
	int hm = HEIGHT - 1;
	int wh = WIDTH * HEIGHT;
	int div = radius + radius + 1;

	unsigned char* r = new unsigned char[wh];
	unsigned char* g = new unsigned char[wh];
	unsigned char* b = new unsigned char[wh];

	int rsum, gsum, bsum, x, y, i, p, p1, p2, yp, yi, yw;

	int* vMIN = new int[std::max<int>(WIDTH, HEIGHT)];
	int* vMAX = new int[std::max<int>(WIDTH, HEIGHT)];

	unsigned char* dv = new unsigned char[256 * div];

	for (i = 0; i < 256 * div; i++) dv[i] = (i / div);

	yw = yi = 0;

	for (y = 0; y < HEIGHT; y++) {
		rsum = gsum = bsum = 0;
		for (i = -radius; i <= radius; i++) 
		{
			p = (yi + std::min<int>(wm, std::max<int>(i, 0))) * 3;
			rsum += pix[p];
			gsum += pix[p + 1];
			bsum += pix[p + 2];
		}
		for (x = 0; x < WIDTH; x++) {

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
		yw += WIDTH;
	}

	for (x = 0; x < WIDTH; x++) 
	{
		rsum = gsum = bsum = 0;
		yp = -radius * WIDTH;
		for (i = -radius; i <= radius; i++) 
		{
			yi = std::max<int>(0, yp) + x;
			rsum += r[yi];
			gsum += g[yi];
			bsum += b[yi];
			yp += WIDTH;
		}
		yi = x;
		for (y = 0; y < HEIGHT; y++) 
		{
			pix[yi * 3] = dv[rsum];
			pix[yi * 3 + 1] = dv[gsum];
			pix[yi * 3 + 2] = dv[bsum];
			if (x == 0) {
				vMIN[y] = std::min<int>(y + radius + 1, hm) * WIDTH;
				vMAX[y] = std::max<int>(y - radius, 0) * WIDTH;
			}
			p1 = x + vMIN[y];
			p2 = x + vMAX[y];

			rsum += r[p1] - r[p2];
			gsum += g[p1] - g[p2];
			bsum += b[p1] - b[p2];

			yi += WIDTH;
		}
	}

	delete[] r;
	delete[] g;
	delete[] b;

	delete[] vMIN;
	delete[] vMAX;
	delete[] dv;
}*/