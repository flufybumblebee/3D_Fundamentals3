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

	values.emplace(' ', 0);
	values.emplace('A', 1);
	values.emplace('B', 2);
	values.emplace('C', 3);
	values.emplace('D', 4);
	values.emplace('E', 5);
	values.emplace('F', 6);
	values.emplace('G', 7);
	values.emplace('=', 8);
	values.emplace('#', 9);
}

/*-------------------------------------------*/

void Tetris::Initialise()
{
	InitialiseBackground();
	InitialiseBlocks();
	InitialiseDigits();
	InitialiseTetrominos();
	InitialiseField();
	InitialiseFieldGrid();
	InitialiseNextTetro();
	InitialisePaused();
	InitialiseGameOver();

	InitialiseScore();
	InitialiseLevel();
	InitialiseCounter();
	InitialiseSounds();

	InitialiseTetris();

	InitialiseKeys();
	InitialiseButtons();

	InitialiseBox();

}
void Tetris::Setup()
{	
	game_is_paused		= false;
	game_is_over		= false;
	next_tetro			= RandomInt(0u, 6u);
	current_tetro		= RandomInt(0u, 6u);
	current_x			= FIELD_COLS / 2u - 2u;
	current_y			= 0u;
	current_rotation	= 0u;
	counter_ticks		= 0u;
	counter_frames		= 0u;
	line_count			= 0u;
	speed				= 120u;

	score				= 0u;
	score_buffer.clear();
	ExtractDigits(score_buffer, score);

	level				= 0u;
	level_buffer.clear();
	ExtractDigits(level_buffer, level);

	SetBackground();	

	ResetField();
	SetFieldBlocks();
	SetNextTetro();
	SetCounter();
}
void Tetris::Input()
{
	if (mouse.IsInWindow())
	{
		const unsigned int MOUSE_X = mouse.GetPosX();
		const unsigned int MOUSE_Y = mouse.GetPosY();
		const bool LEFT_IS_PRESSED = mouse.LeftIsPressed();

		for (int i = 0; i < keys.size(); i++)
		{
			keys[i].Set(MOUSE_X,MOUSE_Y,LEFT_IS_PRESSED);
		}
		
		button_quit.Set(MOUSE_X, MOUSE_Y, LEFT_IS_PRESSED);
		button_volume.Set(MOUSE_X, MOUSE_Y, LEFT_IS_PRESSED);
	}

	Pause();
	Restart(); 

	Quit();
	Volume();

	const bool TICK = counter_frames > 0u && counter_frames % 8u == 0u;

	if (!game_is_paused && !game_is_over)
	{
		if (TICK)
		{
			if (keys[KEY::LEFT].GetMousePress() || kbd.KeyIsPressed(VK_LEFT))
			{
				if (DoesTetroFit(0, MOVE::LEFT, 0))
				{
					current_x--;
				}
			}
			else if (keys[KEY::RIGHT].GetMousePress() || kbd.KeyIsPressed(VK_RIGHT))
			{
				if (DoesTetroFit(0, MOVE::RIGHT, 0))
				{
					current_x++;
				}
			}
			else if (keys[KEY::DOWN].GetMousePress() || kbd.KeyIsPressed(VK_DOWN))
			{
				if (DoesTetroFit(0, 0, MOVE::DOWN))
				{
					current_y++;
				}
			}
		}

		if (!key_is_pressed[KEY::ROTATE])
		{
			if (keys[KEY::ROTATE].GetMousePress() || kbd.KeyIsPressed(VK_UP))
			{
				if (DoesTetroFit(MOVE::ROTATE, 0, 0))
				{
					current_rotation++;
					sounds[SOUND::ROTATE].Play(frequency, volume);
				}

				key_is_pressed[KEY::ROTATE] = true;
			}
		}
		else
		{
			if (!keys[KEY::ROTATE].GetMousePress() && !kbd.KeyIsPressed(VK_UP))
			{
				key_is_pressed[KEY::ROTATE] = false;
			}
		}
	}
}

/*-------------------------------------------*/

void Tetris::Update()
{	
	Input();

	if (!game_is_over && !game_is_paused)
	{
		if (counter_frames == speed)
		{
			if (DoesTetroFit(0, 0, MOVE::DOWN))
			{
				if (!keys[KEY::DOWN].GetMousePress() || kbd.KeyIsPressed(VK_DOWN))
					current_y++; // force tetris down
			}
			else
			{
				SetFixedWithTetro();
				CheckForLines();

				DeleteLines();
				SetNextTetro();

				SetScore();
				SetLevel();

				game_is_over = !DoesTetroFit(0, 0, 0);
				if (game_is_over) sounds[SOUND::GAME_OVER].Play(frequency, volume);
			}

			counter_frames = 0u;
		}
		else
		{
			counter_frames++;
		}
		
		SetShownWithFixed();
		SetShownWithTetro();
		SetFieldBlocks();

		SetCounter();
	}		
}
void Tetris::Draw()
{
	DrawBackground();

	if (!game_is_paused && !game_is_over)
	{
		DrawFieldGrid();
		DrawField();
		DrawNextTetro();
	}

	DrawBox();
	DrawBlur();
	DrawScore();
	DrawLevel();
	DrawCounter();

	DrawKeys();
	DrawButtons();

	DrawPause();
	DrawGameOver();

	DrawTetris();
}

/*-------------------------------------------*/

void Tetris::InitialiseBackground()
{
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Blocks3DRainbow.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\BlocksBlue.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\BlocksGreen.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Nature0.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Nature1.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Nature2.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Nature3.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Nature4.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Nature5.png"));
	ptr_background_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Backgrounds\\Space1.png"));
	
	background_position = { 0u,SCREEN_H-1u,0u,SCREEN_W-1u };	
	const size_t MIN = 0u;
	const size_t MAX = ptr_background_textures.size() - 1u;
	current_background = RandomInt(MIN, MAX);
	background_block = std::move(Block(background_position, ptr_background_textures[current_background]));
}
void Tetris::InitialiseBlocks()
{
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_DarkGrey.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Orange.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Cyan.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Green.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Red.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Blue.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Magenta.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Yellow.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Red.png"));
	ptr_block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Blocks\\BlocksA\\Block_Grey.png"));
}
void Tetris::InitialiseDigits()
{
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_0.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_1.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_2.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_3.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_4.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_5.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_6.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_7.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_8.png"));
	ptr_digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_9.png"));
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
void Tetris::InitialiseField()
{
	const unsigned int TOP		= BLOCK_H;
	const unsigned int BOTTOM	= BLOCK_H + BLOCK_H * FIELD_ROWS;
	const unsigned int LEFT		= BLOCK_W;
	const unsigned int RIGHT	= BLOCK_W + BLOCK_W * FIELD_COLS;

	field_position = { TOP,BOTTOM,LEFT,RIGHT };

	for (uint y = 0; y < FIELD_ROWS; y++)
	{
		for (uint x = 0; x < FIELD_COLS; x++)
		{
			const RectUI POSITION = {
				TOP		+ (y * BLOCK_H),
				TOP		+ (y * BLOCK_H) + BLOCK_H,
				LEFT	+ (x * BLOCK_W),
				LEFT	+ (x * BLOCK_W) + BLOCK_W };

			field_blocks[y][x] = std::move(Block(POSITION, ptr_block_textures[0]));
		}
	}
}
void Tetris::InitialiseNextTetro()
{
	const unsigned int TOP	= BLOCK_H;
	const unsigned int LEFT = BLOCK_W + BLOCK_W * FIELD_COLS;

	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{
			const RectUI POSITION = {
				TOP		+ (y * BLOCK_H),
				TOP		+ (y * BLOCK_H) + BLOCK_H,
				LEFT	+ (x * BLOCK_W),
				LEFT	+ (x * BLOCK_W) + BLOCK_W };

			next_tetro_blocks[y][x] = std::move(Block(POSITION, ptr_block_textures[0]));
		}
	}
}

void Tetris::InitialisePaused()
{
	ptr_paused_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Words\\Paused1.png"));

	const unsigned int LEFT		= BLOCK_W;
	const unsigned int RIGHT	= BLOCK_W + FIELD_W;
	const unsigned int TOP		= SCREEN_H / 2u - DIGIT_H / 2u;
	const unsigned int BOTTOM	= SCREEN_H / 2u + DIGIT_H / 2u;

	paused_block = std::move(Block(RectUI(TOP,BOTTOM,LEFT,RIGHT), ptr_paused_textures[0]));
}
void Tetris::InitialiseGameOver()
{
	ptr_gameover_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Words\\GameOver1.png"));

	const unsigned int TOP		= SCREEN_H / 2u - DIGIT_H;
	const unsigned int BOTTOM	= SCREEN_H / 2u + DIGIT_H;
	const unsigned int LEFT		= BLOCK_W * 3u;
	const unsigned int RIGHT	= BLOCK_W * 11u;

	gameover_block = std::move(Block(RectUI(TOP, BOTTOM, LEFT, RIGHT), ptr_gameover_textures[0]));
}

void Tetris::InitialiseLevel()
{
	{
		ptr_level_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Words\\Level1.png"));

		const unsigned int TOP		= BLOCK_H * 6u;
		const unsigned int BOTTOM	= BLOCK_H * 6u + DIGIT_H;
		const unsigned int LEFT		= BLOCK_W * (FIELD_COLS + 2u);
		const unsigned int RIGHT	= BLOCK_W * (FIELD_COLS + 2u) + DIGIT_W * 6u;

		level_block = std::move(Block(RectUI(TOP, BOTTOM, LEFT, RIGHT), ptr_level_textures[0]));
	}

	{
		const unsigned int TOP = BLOCK_H * 8u;
		const unsigned int RIGHT = SCREEN_W - BLOCK_W;

		{
			for (uint y = 0u; y < LEVEL_ROWS; y++)
			{
				for (uint x = 0u; x < LEVEL_COLS; x++)
				{
					const RectUI POSITION = {
						TOP,
						TOP + DIGIT_H,
						RIGHT - (DIGIT_W * x) - DIGIT_W,
						RIGHT - (DIGIT_W * x) };

					level_blocks[y][x] = std::move(Block(POSITION, ptr_digit_textures[y]));
				}
			}
		}
	}
}
void Tetris::InitialiseScore()
{
	{
		ptr_score_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Words\\Score1.png"));

		const unsigned int TOP		= BLOCK_H * 10u;
		const unsigned int BOTTOM	= BLOCK_H * 10u + DIGIT_H;
		const unsigned int LEFT		= BLOCK_W * (FIELD_COLS + 2u);
		const unsigned int RIGHT	= BLOCK_W * (FIELD_COLS + 2u) + DIGIT_W * 6u;

		score_block = std::move(Block(RectUI(TOP, BOTTOM, LEFT, RIGHT), ptr_score_textures[0]));
	}

	{
		const unsigned int TOP		= BLOCK_H * 12u;
		const unsigned int RIGHT	= SCREEN_W - BLOCK_W;

		for (uint y = 0u; y < SCORE_ROWS; y++)
		{
			for (uint x = 0u; x < SCORE_COLS; x++)
			{
				const RectUI POSITION = {
					TOP,
					TOP + DIGIT_H,
					RIGHT - (DIGIT_W * x) - DIGIT_W,
					RIGHT - (DIGIT_W * x) };

				score_blocks[y][x] = std::move(Block(POSITION, ptr_digit_textures[y]));
			}
		}
	}	
}
void Tetris::InitialiseCounter()
{
	const unsigned int TOP		= BLOCK_H * 17u;
	const unsigned int BOTTOM	= BLOCK_H * 19u;
	const unsigned int LEFT		= SCREEN_W - BLOCK_W * 3u;
	const unsigned int RIGHT	= SCREEN_W - BLOCK_W * 1u;

	const RectUI POSITION = { TOP,BOTTOM,LEFT,RIGHT };

	for (uint i = 0; i < COUNT_NUM; i++)
	{
		counter_blocks[i] = std::move(Block(POSITION, ptr_digit_textures[i]));
	}
}
void Tetris::InitialiseKeys()
{
	ptr_key_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Keys\\key_restart.png"));
	ptr_key_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Keys\\key_up.png"));
	ptr_key_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Keys\\key_pause.png"));
	ptr_key_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Keys\\key_left.png"));
	ptr_key_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Keys\\key_down.png"));
	ptr_key_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Keys\\key_right.png"));
	
	/*
		l m r
		e i i
		f d g
		t d h
		  l t
		  e

		0 1 2  top
		3 4 5  bottom

		0 = restart	1 = rotate cw	2 = pause		
		3 = left	4 = down		5 = right
		
		key layout

		T0,L0----  T0,L1----  T0,L2----
		|.......|  |.......|  |.......|
		|restart|  |rotate.|  |.pause.| top row
		|.......|  |.......|  |.......|
		----T1,R0  ----T0,R1  ----T0,R2

		B0,L0----  B0,L1----  B0,L2----
		|.......|  |.......|  |.......|
		|.left..|  |.down..|  |.right.| bottom row
		|.......|  |.......|  |.......|
		----B1,R0  ----B0,R1  ----B0,R2
	
		TOP = the bottom of the tetris playing field then up by two button heights
		LEFT = the right of the playing field then right by one button width
	*/
	
	/*
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
	*/

	std::vector<RectUI> position_a;
	std::vector<RectUI> position_b;

	const uint TOP		= BLOCK_H * 15u;
	const uint LEFT		= BLOCK_W * 14u;
	const uint OFFSET	= 5u;

	for (uint y = 0u; y < 2u; y++)
	{
		for (uint x = 0u; x < 3u; x++)
		{
			position_a.emplace_back(
				TOP		+ (KEY_H * (y + 0u)) - 0u,
				TOP		+ (KEY_H * (y + 1u)) - 1u,
				LEFT	+ (KEY_W * (x + 0u)) - 0u,
				LEFT	+ (KEY_W * (x + 1u)) - 1u);

			position_b.emplace_back(
				TOP		+ (KEY_H * (y + 0u)) - 0u + OFFSET,
				TOP		+ (KEY_H * (y + 1u)) - 1u - OFFSET,
				LEFT	+ (KEY_W * (x + 0u)) - 0u + OFFSET,
				LEFT	+ (KEY_W * (x + 1u)) - 1u - OFFSET);
		}
	}

	/*std::vector<RectUI> position_a{
		{ T0,T1,L0,R0 },
		{ T0,T1,L1,R1 },
		{ T0,T1,L2,R2 },

		{ B0,B1,L0,R0 },
		{ B0,B1,L1,R1 },
		{ B0,B1,L2,R2 } };

	std::vector<RectUI> position_b{
		{ T0 + OFFSET,T1 - OFFSET,L0 + OFFSET,R0 - OFFSET },
		{ T0 + OFFSET,T1 - OFFSET,L1 + OFFSET,R1 - OFFSET },
		{ T0 + OFFSET,T1 - OFFSET,L2 + OFFSET,R2 - OFFSET },

		{ B0 + OFFSET,B1 - OFFSET,L0 + OFFSET,R0 - OFFSET },
		{ B0 + OFFSET,B1 - OFFSET,L1 + OFFSET,R1 - OFFSET },
		{ B0 + OFFSET,B1 - OFFSET,L2 + OFFSET,R2 - OFFSET } };*/

	assert(position_a.size() == position_b.size());

	for( int i = 0; i < ptr_key_textures.size(); i++ )
	{
		Block block_a{ position_a[i], ptr_key_textures[i] };
		Block block_b{ position_a[i], ptr_key_textures[i] };
		Block block_c{ position_b[i], ptr_key_textures[i] };
		Block block_d{ position_b[i], ptr_key_textures[i] };

		keys.emplace_back(block_a, block_b, block_c, block_d);
	}
}
void Tetris::InitialiseButtons()
{
	// QUIT BUTTON

	ptr_button_quit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Buttons\\quit_green.png"));
	ptr_button_quit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Buttons\\quit_red.png"));

	{
		const uint TOP		= BLOCK_H * 16u;
		const uint BOTTOM	= BLOCK_H * 18u;
		const uint LEFT		= BLOCK_W * 2u;
		const uint RIGHT	= BLOCK_W * 4u;

		const RectUI POSITION = { TOP,BOTTOM,LEFT,RIGHT };
				
		Block block_a(POSITION, ptr_button_quit_textures[0]);
		Block block_b(POSITION, ptr_button_quit_textures[1]);
		Block block_c(POSITION, ptr_button_quit_textures[0]);
		Block block_d(POSITION, ptr_button_quit_textures[1]);
		
		button_quit = { block_a,block_b,block_c,block_d };
	}

	/*---------------------------------------------------------*/

	// VOLUME BUTTON

	ptr_button_volume_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Buttons\\volume_full_white.png"));
	ptr_button_volume_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Buttons\\volume_full_green.png"));
	ptr_button_volume_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Buttons\\volume_off_white.png"));
	ptr_button_volume_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Buttons\\volume_off_green.png"));

	{
		const uint TOP		= BLOCK_H * 16u;
		const uint BOTTOM	= BLOCK_H * 18u;
		const uint LEFT		= BLOCK_W * 10u;
		const uint RIGHT	= BLOCK_W * 12u;

		const RectUI POSITION = { TOP,BOTTOM,LEFT,RIGHT };

		Block block_a(POSITION, ptr_button_volume_textures[0]);
		Block block_b(POSITION, ptr_button_volume_textures[1]);
		Block block_c(POSITION, ptr_button_volume_textures[2]);
		Block block_d(POSITION, ptr_button_volume_textures[3]);

		button_volume = { block_a,block_b,block_c,block_d };
	}
}
void Tetris::InitialiseSounds()
{
	sounds.emplace_back(Sound(L"Sounds\\pop1.wav"));		// rotate
	sounds.emplace_back(Sound(L"Sounds\\success0.wav"));	// line 1
	sounds.emplace_back(Sound(L"Sounds\\success1.wav"));	// line 2
	sounds.emplace_back(Sound(L"Sounds\\success2.wav"));	// line 3
	sounds.emplace_back(Sound(L"Sounds\\success3.wav"));	// line 4
	sounds.emplace_back(Sound(L"Sounds\\pop0.wav"));		// new tetro
	sounds.emplace_back(Sound(L"Sounds\\fail0.wav"));		// game over
}
void Tetris::InitialiseTetris()
{
	ptr_tetris_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Words\\Tetris1.png"));

	const unsigned int LEFT		= BLOCK_W * 14u;
	const unsigned int RIGHT	= SCREEN_W - BLOCK_W;
	const unsigned int TOP		= BLOCK_H;
	const unsigned int BOTTOM	= BLOCK_H * 3;

	tetris_block = std::move(Block(RectUI(TOP, BOTTOM, LEFT, RIGHT), ptr_tetris_textures[0]));
}
void Tetris::InitialiseBox()
{
	Surface surf(FIELD_W, FIELD_H);
	Color c = Color(155, 0, 0, 0);

	for (uint y = 0; y < FIELD_H; y++)
	{
		for (uint x = 0; x < FIELD_W; x++)
		{
			surf.PutPixel(x, y, c);
		}
	}
	
	ptr_box_textures.emplace_back(std::make_shared<Surface>(std::move(surf)));
	box_block = std::move(Block(field_position, ptr_box_textures[0]));
}

void Tetris::InitialiseFieldGrid()
{
	//ptr_field_grid_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Fields\\field_grid_2.png"));
	//field_grid_block = std::move(Block(field_position, ptr_field_grid_textures[0]));
}

/*-------------------------------------------*/

void Tetris::Pause()
{
	if (!game_is_over)
	{
		if (!key_is_pressed[KEY::PAUSE])
		{
			if (kbd.KeyIsPressed(VK_SPACE) || keys[KEY::PAUSE].GetMousePress())
			{
				if (!game_is_paused)
				{
					game_is_paused = true;
				}
				else
				{
					game_is_paused = false;
				}

				key_is_pressed[KEY::PAUSE] = true;
			}
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_SPACE) && !keys[KEY::PAUSE].GetMousePress())
			{
				key_is_pressed[KEY::PAUSE] = false;
			}
		}
	}
}
void Tetris::Restart()
{
	if (game_is_paused || game_is_over)
	{
		if (kbd.KeyIsPressed(VK_RETURN) || keys[KEY::RESTART].GetMousePress())
		{
			Setup();
		}
	}
}

void Tetris::Quit()
{
	if (button_quit.GetMousePress())
	{
		PostQuitMessage(0);
	}
}
void Tetris::Volume()
{
	if (!mouse_is_pressed)
	{
		if (button_volume.GetMousePress())
		{
			if (button_volume_full)
			{				
				volume = 0.0f;
				button_volume_full = false;
			}
			else
			{
				volume = 1.0f;
				button_volume_full = true;
			}

			mouse_is_pressed = true;
		}
	}
	else
	{
		if (!mouse.LeftIsPressed())
		{
			mouse_is_pressed = false;
		}
	}
}

/*-------------------------------------------*/

void Tetris::SetBackground()
{
	if (current_background < ptr_background_textures.size() - 1u)
	{
		current_background++;
	}
	else
	{
		current_background = 0u;
	}

	background_block.SetTexture( ptr_background_textures[current_background] );
}
void Tetris::ResetField()
{
	for (uint y = 0u; y < FIELD_ROWS; y++)	
	{
		for (uint x = 0u; x < FIELD_COLS; x++)
		{
			const uint INDEX = y * FIELD_COLS + x;
			const uint VALUE = (x == 0u || x == FIELD_COLS - 1u || y == FIELD_ROWS - 1u) ? 9u : 0u;
			fixed_buffer[INDEX] = VALUE;

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
			const uint INDEX_2 = values[shown_buffer[INDEX_1]];
			field_blocks[y][x].SetTexture(ptr_block_textures[INDEX_2]);
		}
	}
}
void Tetris::SetNextTetro()
{
	current_x			= FIELD_COLS / 2u - 2u;
	current_y			= 0u;
	current_rotation	= 0u;

	current_tetro = next_tetro;
	next_tetro = RandomInt(0u, 6u);

	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{	
			const uint INDEX_1 = y * TETRO_COLS + x;
			const uint INDEX_2 = (tetromino[next_tetro][INDEX_1] != '.') ? next_tetro + 1u : 0u;

			next_tetro_blocks[y][x].SetTexture(ptr_block_textures[INDEX_2]);
		}
	}

	sounds[SOUND::NEW_TETRO].Play(frequency,volume);
}
void Tetris::SetScore()                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
{
	score += 25u;

	if (!lines.empty()) score += (1u << lines.size()) * 100u;

	ExtractDigits(score_buffer, score);	
}
void Tetris::SetLevel()
{
	if (line_count >= 4u)
	{
		line_count -= 4u;
		level_goal = true;
		// play sound
	}

	if (level_goal)
	{		
		level_prev = level;
		level++;
		speed -= 1u;
		level_goal = false;
	}	

	ExtractDigits(level_buffer, level);
}
void Tetris::SetCounter()
{
	/*if (counter_ticks < 9u)
	{
		counter_ticks++;
	}
	else
	{
		counter_ticks = 0u;
	}*/

	counter_ticks = line_count;
}
void Tetris::CheckForLines()
{
	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		if (current_y + y < FIELD_ROWS - 1u)
		{
			bool isLine = true;
			for (uint x = 1u; x < FIELD_COLS - 1u; x++)
			{
				const uint INDEX = (current_y + y) * FIELD_COLS + x;
				isLine &= (fixed_buffer[INDEX] != 0u);
			}

			if (isLine)
			{
				line_count++;

				// set line to '=' (red)
				for (uint x = 1u; x < FIELD_COLS - 1u; x++)
				{
					const uint INDEX = (current_y + y) * FIELD_COLS + x;
					fixed_buffer[INDEX] = 8u;
				}
				
				lines.push_back(current_y + y);
			}
		}
	}
}
void Tetris::DeleteLines()
{
	if (!lines.empty())
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(8000)); // delay

		/*for (auto& v : lines)
		{
			for (int x = 1; x < FIELD_COLS - 1; x++)
			{
				for (int y = v; y > 0; y--)
				{
					int i = y * FIELD_COLS + x;
					int j = (y - 1) * FIELD_COLS + x;
					fixed_buffer[i] = fixed_buffer[j];
				}
				fixed_buffer[x] = 0;
			}
		}*/

		const uint LINES_NUM = static_cast<uint>(lines.size());		
		
		for (uint x = 1u; x < FIELD_COLS - 1u; x++)
		{
			for (uint i = 0u; i < LINES_NUM; i++)
			{
				for (uint y = lines[i]; y > 0u; y--)
				{					
					const uint INDEX_0 = y * FIELD_COLS + x;
					const uint INDEX_1 = (y - 1u) * FIELD_COLS + x;
					fixed_buffer[INDEX_0] = fixed_buffer[INDEX_1];
				}					
				fixed_buffer[x] = 0;
			}
		}
		
		lines.clear();

		switch (LINES_NUM)
		{
		case 1: sounds[SOUND::LINE_ONE].Play(frequency, volume); break;

		case 2:	sounds[SOUND::LINE_TWO].Play(frequency, volume); break;

		case 3:	sounds[SOUND::LINE_THREE].Play(frequency, volume); break;

		case 4:	sounds[SOUND::LINE_FOUR].Play(frequency, volume); break;

		default: break;
		}
	}
}
void Tetris::SetFixedWithTetro()
{
	for (uint y = 0u; y < TETRO_ROWS; y++)
	{
		for (uint x = 0u; x < TETRO_COLS; x++)
		{
			if (tetromino[current_tetro][Rotate(x, y, current_rotation)] == 'X')
			{
				const uint INDEX = (current_y + y) * FIELD_COLS + (current_x + x);
				fixed_buffer[INDEX] = current_tetro + 1u;
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
			if (tetromino[current_tetro][Rotate(x, y, current_rotation)] == 'X')
			{		
				const uint INDEX = (current_y + y) * FIELD_COLS + (current_x + x);
				shown_buffer[INDEX] = " ABCDEFG=#"[current_tetro + 1u];
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
			shown_buffer[INDEX] = " ABCDEFG=#"[fixed_buffer[INDEX]];
		}
	}
}

/*-------------------------------------------*/

void Tetris::DrawBackground()
{
	background_block.Draw(gfx);
}
void Tetris::DrawField()
{
	for (uint y = 0u; y < FIELD_ROWS; y++)
	{
		for (uint x = 0u; x < FIELD_COLS; x++)
		{
			const uint INDEX = y * FIELD_COLS + x;

			if (shown_buffer[INDEX] == ' ')
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
	if (!game_is_over)
	{
		for (uint y = 0u; y < TETRO_ROWS; y++)
		{
			for (uint x = 0u; x < TETRO_COLS; x++)
			{
				const uint INDEX = y * TETRO_COLS + x;

				if (tetromino[next_tetro][INDEX] == '.')
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
void Tetris::DrawFieldGrid()
{
	const Color COLOR_0 = Color(155, 255, 255, 255);
	gfx.DrawRectAlpha(true, field_position, COLOR_0);

	const Color COLOR_1 = Color(0,0,0);

	const uint TOP		= field_position.top;
	const uint BOTTOM	= field_position.bottom;
	const uint LEFT		= field_position.left;
	const uint RIGHT	= field_position.right;

	for (uint y = 0; y < FIELD_ROWS; y++)
	{
		for (uint x = 0; x < FIELD_COLS; x++)
		{
			// horizontal lines
			gfx.DrawLineAlpha(LEFT + (x * BLOCK_W), TOP, LEFT + (x * BLOCK_W), BOTTOM, COLOR_1);
		}
	}

	for (int y = 0; y < FIELD_ROWS; y++)
	{
		for (int x = 0; x < FIELD_COLS; x++)
		{
			// vertical lines
			gfx.DrawLineAlpha(LEFT, TOP + (y * BLOCK_H), RIGHT, TOP + (y * BLOCK_H), COLOR_1);
		}
	}

	//field_grid_block.Draw(gfx);
}
void Tetris::DrawCounter()
{
	if (!game_is_over && !game_is_paused)
	{
		counter_blocks[counter_ticks].Draw(gfx);
	}
}
void Tetris::DrawBox()
{
	if (game_is_over || game_is_paused)
	{
		box_block.Draw(gfx);
	}
}
void Tetris::DrawBlur() 
{
	if (game_is_paused || game_is_over) 
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
	score_block.Draw(gfx);

	for (size_t i = 0u; i < score_buffer.size(); i++)
	{
		score_blocks[score_buffer[i]][i].Draw(gfx);
	}

	if (TESTING)
	{
		gfx.DrawRect(false, score_block.GetPosition(), Colors::Red);

		for (size_t i = 0u; i < score_buffer.size(); i++)
		{
			gfx.DrawRect(false, score_blocks[score_buffer[i]][i].GetPosition(), Colors::Red);
		}
	}
}
void Tetris::DrawLevel()
{
	level_block.Draw(gfx);

	for (size_t i = 0u; i < level_buffer.size(); i++)
	{		
		level_blocks[level_buffer[i]][i].Draw(gfx);		
	}
	
	if (TESTING)
	{
		gfx.DrawRect(false, level_block.GetPosition(), Colors::Red);

		for (size_t i = 0u; i < level_buffer.size(); i++)
		{
			gfx.DrawRect(false,level_blocks[level_buffer[i]][i].GetPosition(), Colors::Red);
		}
	}
}
void Tetris::DrawKeys()
{	
	for (size_t i = 0u; i < keys.size(); i++)
	{
		keys[i].Draw(gfx);
	}

	if (TESTING)
	{
		for (size_t i = 0u; i < keys.size(); i++)
		{
			gfx.DrawRect(false, keys[i].GetPosition(), Colors::Red);
		}
	}
}
void Tetris::DrawButtons()
{
	if (game_is_over || game_is_paused)
	{
		button_quit.Draw(gfx);
		button_volume.Draw(gfx);

		if (TESTING)
		{
			gfx.DrawRect(false, button_quit.GetPosition(), Colors::Red);
			gfx.DrawRect(false, button_volume.GetPosition(), Colors::Red);
		}
	}
}
void Tetris::DrawTetris()
{
	if (game_is_over || game_is_paused)
	{
		tetris_block.Draw(gfx);

		if (TESTING)
		{
			gfx.DrawRect(false, tetris_block.GetPosition(), Colors::Red);
		}
	}
}
void Tetris::DrawPause()
{
	if (game_is_paused)
	{		
		paused_block.Draw(gfx);

		if (TESTING)
		{
			gfx.DrawRect(false, paused_block.GetPosition(), Colors::Red);
		}
	}
}
void Tetris::DrawGameOver()
{
	if (game_is_over)
	{		
		gameover_block.Draw(gfx);

		if (TESTING)
		{
			gfx.DrawRect(false, gameover_block.GetPosition(), Colors::Red);
		}
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
	const unsigned int ROTATION = current_rotation + offsetRotation;
	const unsigned int X = current_x + offsetX;
	const unsigned int Y = current_y + offsetY;

	for (unsigned int y = 0u; y < TETRO_ROWS; y++)
	{
		for (unsigned int x = 0u; x < TETRO_COLS; x++)
		{			
			const bool FITS_LEFT	= X + x >=	0u;
			const bool FITS_RIGHT	= X + x <	FIELD_COLS;
			const bool FITS_TOP		= Y + y >=	0u;
			const bool FITS_BOTTOM	= Y + y <	FIELD_ROWS;

			if ( FITS_LEFT && FITS_RIGHT && FITS_TOP && FITS_BOTTOM )
			{
				const unsigned int INDEX_0 = Rotate(x, y, ROTATION);
				const unsigned int INDEX_1 = (Y + y) * FIELD_COLS + (X + x);

				if (tetromino[current_tetro][INDEX_0] != L'.' && fixed_buffer[INDEX_1] != 0u)
				{
					return false;
				}
			}
		}
	}
	return true;
}
void	Tetris::ExtractDigits(std::vector<unsigned int>& ints, const unsigned int& NUM)
{
	ints.clear();
	unsigned int result = NUM;

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

/*-------------------------------------------*/

std::vector<Color> Tetris::ConvertSurfaceToColorVector(const Surface& surface)
{
	std::vector<Color> output;

	for (uint y = 0u; y < surface.GetHeight(); y++)
	{
		for (uint x = 0u; x < surface.GetWidth(); x++)
		{
			output.emplace_back(surface.GetPixel(x, y));
		}
	}
	
	return output;
}
std::vector<Color> Tetris::Blur(const int& WIDTH,const int& HEIGHT,const std::vector<Color>& INPUT)
{
	assert( INPUT.size() == size_t(WIDTH) * HEIGHT );
		
	constexpr float val = 1.0f / 9.0f;

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

						pixel_out.x += (INPUT[INDEX].GetR() * box[ROW][COL]);
						pixel_out.y += (INPUT[INDEX].GetG() * box[ROW][COL]);
						pixel_out.z += (INPUT[INDEX].GetB() * box[ROW][COL]);
					}
				}
			}

			output.push_back({ 
				static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.x))),
				static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.y))),
				static_cast<uchar>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.z))) });
		}
	}

	assert(output.size() == INPUT.size());

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
					int current_x = x + col;
					int current_y = y + row;

					if (current_x > 0 && current_x < (int)width && current_y > 0 && current_y < (int)height)
					{
						Color c = blur.GetPixel(current_x, current_y);

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