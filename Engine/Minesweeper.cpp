#include "Minesweeper.h"

Minesweeper::Minesweeper( const unsigned int& COLS, const unsigned int& ROWS, const unsigned int& MINES)
	:
	GRID_COLS(std::max<unsigned int>(MIN_COLS, COLS)),
	GRID_ROWS(std::max<unsigned int>(MIN_ROWS, ROWS)),
	GRID_SIZE(GRID_COLS * GRID_ROWS),
	MINES(std::max<unsigned int>(1u,std::min<unsigned int>(GRID_SIZE - 1u,MINES)))
{
	mines = this->MINES;
	
	block_revealed.assign(GRID_SIZE, false);
	block_flag.assign(GRID_SIZE, false);

	SetBlockValues();
	
	InitialiseBorder();
	InitialiseGridBlocks();
	InitialiseDigitTextures();
	InitialiseMinesCounter();
	InitialiseResetButton();
	InitialiseTimer();
	InitialiseGameOver();

	InitialiseSounds();
}

/*--------------------------------------------*/

void Minesweeper::Update(Mouse& mouse)
{
	reset_button_blocks[0].SetMouseOver(mouse);

	if (reset_button_blocks[0].GetMouseOver())
	{
		if (!mouse_pressed)
		{
			if (mouse.LeftIsPressed())
			{
				sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
				Setup();
				reset_button_pressed = true;
				mouse_pressed = true;
			}
			else
			{
				reset_button_pressed = false;
			}
		}
		else
		{
			if (!mouse.LeftIsPressed())
			{
				mouse_pressed = false;
			}
		}
	}

	if(!gameover && mouse.IsInWindow())
	{
		for(auto& b : blocks)
		{
			b.SetMouseOver(mouse);
		}		

		int i = 0;

		for(unsigned int y = 0u; y < GRID_ROWS; y++)
		{
			for(unsigned int x = 0u; x < GRID_COLS; x++)
			{
				i = y * GRID_COLS + x;				

				if(blocks[i].GetMouseOver())
				{
					if(!mouse_pressed)
					{
						if(mouse.LeftIsPressed())
						{
							if (!block_revealed[i])
							{
								sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);

								blocks[i].SetTexture(block_textures[block_values[i]]);
								block_revealed[i] = true;

								if (block_values[i] == 9u)
								{
									gameover = true;
									gamewon = false;
								}
								else if (block_values[i] == 0u)
								{
									RevealBlocks(x, y, GRID_COLS, GRID_ROWS);
								}

								mouse_pressed = true;
							}
						}
						else if(mouse.RightIsPressed())
						{
							if(!block_revealed[i])
							{
								if(block_flag[i])
								{
									sounds[SOUNDS::CLICK_2].Play(1.0f, 1.0f);
									if (flags > 0) { flags--; }											
									assert(flags >= 0u);

									if (mines < MINES)
									{
										mines++;
										blocks[i].SetTexture(block_textures[GRID::TILE]);
										block_flag[i] = false;
									}
									assert(mines <= MINES);
								}
								else
								{
									sounds[SOUNDS::CLICK_1].Play(1.0f, 1.0f);
									if (flags < MINES) { flags++; }
									assert(flags <= MINES);

									if (mines > 0u)
									{ 
										mines--;
										blocks[i].SetTexture(block_textures[GRID::FLAG]);
										block_flag[i] = true;
									}
									assert(mines >= 0u);
								}
							}
							mouse_pressed = true;
						}

						if (mouse_pressed && !timer_started)
						{
							timer_started = true;
							t1 = std::chrono::high_resolution_clock::now();
						}
					}
					else
					{
						if(!mouse.LeftIsPressed() && !mouse.RightIsPressed())
						{
							mouse_pressed = false;
						}
					}
				}
			}
		}		
	}

	if (!gameover)
	{
		SetTimer();
		SetMinesCounter();
	}

	if (!gameover)
	{
		unsigned int count_mines_and_flags = 0u;
		unsigned int count_revealed_blocks = 0u;
		for (unsigned int i = 0u; i < GRID_SIZE; i++)
		{
			if (block_values[i] == 9u && block_flag[i])
			{
				count_mines_and_flags++;
			}

			if (block_revealed[i])
			{
				count_revealed_blocks++;
			}
		}
				
		if (count_mines_and_flags == MINES && count_revealed_blocks == GRID_SIZE - MINES)
		{
			gameover = true;
			gamewon = true;
		}		
	}


	if(gameover)
	{
		if (gamewon)
		{
			if (!sound_played)
			{
				sounds[1].Play(1.0f, 1.0f);
				sound_played = true;
			}
			reset_button_blocks[0].SetTexture(reset_button_textures[1]);
			reset_button_blocks[1].SetTexture(reset_button_textures[1]);
		}
		else
		{
			if (!sound_played)
			{
				sounds[0].Play(1.0f, 1.0f);
				sound_played = true;
			}
			reset_button_blocks[0].SetTexture(reset_button_textures[2]);
			reset_button_blocks[1].SetTexture(reset_button_textures[2]);

			for (unsigned int i = 0u; i < GRID_SIZE; i++)
			{
				if (!block_revealed[i] && block_values[i] == 9u)
				{
					blocks[i].SetTexture(block_textures[block_values[i]]);
					block_revealed[i] = true;
				}
			}
		}
	}
}
void Minesweeper::Draw(Graphics& gfx)
{
	DrawBorder(gfx);
	DrawDisplayBackground(gfx);
	DrawGrid(gfx);
	DrawGridBlocks(gfx);
	DrawMouseOverBlocks(gfx);
	DrawMinesCounter(gfx);
	DrawResetButton(gfx);
	DrawTimer(gfx);
	DrawGameOver(gfx);
}

/*--------------------------------------------*/

void Minesweeper::Setup()
{
	timer_started	= false;
	gameover		= false;
	gamewon			= false;
	sound_played	= false;
	mines			= MINES;
	flags			= 0u;
	time			= 0u;
	frames			= 0u;
	index0			= 0u;
	index1			= 0u;

	block_revealed.assign(GRID_SIZE, false);
	block_flag.assign(GRID_SIZE, false);
	SetBlockValues();

	reset_button_blocks[0].SetTexture(reset_button_textures[0]);
	reset_button_blocks[1].SetTexture(reset_button_textures[0]);

	for (auto& b : blocks)
	{
		b.SetTexture(block_textures[GRID::TILE]);
	}
}

/*--------------------------------------------*/

void Minesweeper::InitialiseDigitTextures()
{
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_0_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_1_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_2_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_3_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_4_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_5_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_9_white.png"));
}
void Minesweeper::InitialiseBorder()
{
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_horizontal.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_verticle.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_top_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_top_right.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_bottom_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_bottom_right.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_t_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_t_right.png"));

	{
		const unsigned int TOP = 0u;
		const unsigned int BOTTOM = OFFSET * 6 + BLOCK_SIZE * GRID_ROWS + OFFSET - 1u;
		const unsigned int LEFT = 0u;
		const unsigned int RIGHT = OFFSET + BLOCK_SIZE * GRID_COLS + OFFSET - 1u;;

		border_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);
	}

	// Top Horizontal
	RectUI corner_top_left{ 
		border_position.top,
		OFFSET,
		border_position.left,
		border_position.left + OFFSET };

	RectUI top{
		border_position.top,
		OFFSET,
		border_position.left + OFFSET,
		border_position.right - OFFSET };

	RectUI corner_top_right{
		border_position.top,
		OFFSET,
		border_position.right - OFFSET,
		border_position.right};

	border_blocks.emplace_back(corner_top_left, border_textures[BORDER::CORNER_TOP_LEFT]);
	border_blocks.emplace_back(top, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(corner_top_right, border_textures[BORDER::CORNER_TOP_RIGHT]);
	
	// Middle Horizontal
	RectUI t_left{
		border_position.top + OFFSET * 5u,
		border_position.top + OFFSET * 6u,
		border_position.left,
		border_position.left + OFFSET };

	RectUI middle{
		border_position.top + OFFSET * 5u,
		border_position.top + OFFSET * 6u,
		border_position.left + OFFSET,
		border_position.right - OFFSET };

	RectUI t_right{
		border_position.top + OFFSET * 5u,
		border_position.top + OFFSET * 6u,
		border_position.right - OFFSET,
		border_position.right };

	border_blocks.emplace_back(t_left, border_textures[BORDER::T_LEFT]);
	border_blocks.emplace_back(middle, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(t_right, border_textures[BORDER::T_RIGHT]);

	// Bottom Horizontal
	RectUI corner_bottom_left{
		border_position.bottom - OFFSET,
		border_position.bottom,
		border_position.left,
		border_position.left + OFFSET };

	RectUI bottom{
		border_position.bottom - OFFSET,
		border_position.bottom,
		border_position.left + OFFSET,
		border_position.right - OFFSET };

	RectUI corner_bottom_right{
		border_position.bottom - OFFSET,
		border_position.bottom,
		border_position.right - OFFSET,
		border_position.right };

	border_blocks.emplace_back(corner_bottom_left, border_textures[BORDER::CORNER_BOTTOM_LEFT]);
	border_blocks.emplace_back(bottom, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(corner_bottom_right, border_textures[BORDER::CORNER_BOTTOM_RIGHT]);

	// Verticle Left
	RectUI left_top{
		border_position.top + OFFSET,
		border_position.top + OFFSET * 5,
		border_position.left,
		border_position.left + OFFSET };

	RectUI left_bottom{
		border_position.top + OFFSET * 6u,
		border_position.bottom - OFFSET,
		border_position.left,
		border_position.left + OFFSET };

	border_blocks.emplace_back(left_top, border_textures[BORDER::VERTICLE]);
	border_blocks.emplace_back(left_bottom, border_textures[BORDER::VERTICLE]);

	// Verticle Right
	RectUI right_top{
		border_position.top + OFFSET,
		border_position.top + OFFSET * 5,
		border_position.right - OFFSET,
		border_position.right };

	RectUI right_bottom{
		border_position.top + OFFSET * 6u,
		border_position.bottom - OFFSET,
		border_position.right - OFFSET,
		border_position.right };

	border_blocks.emplace_back(right_top, border_textures[BORDER::VERTICLE]);
	border_blocks.emplace_back(right_bottom, border_textures[BORDER::VERTICLE]);
}
void Minesweeper::InitialiseGridBlocks() 
{
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_0_white.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_1_blue.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_2_green.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_3_red.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_4_cyan.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_5_magenta.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_yellow.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_violet.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\mine.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_blank.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_flag.png"));

	for(unsigned int y = 0u; y < GRID_ROWS; y++)
	{
		for(unsigned int x = 0u; x < GRID_COLS; x++)
		{
			const unsigned int TOP		= OFFSET * 6u + BLOCK_SIZE * y;
			const unsigned int BOTTOM	= OFFSET * 6u + BLOCK_SIZE * y + BLOCK_SIZE - 1u;
			const unsigned int LEFT		= OFFSET * 1u + BLOCK_SIZE * x;
			const unsigned int RIGHT	= OFFSET * 1u + BLOCK_SIZE * x + BLOCK_SIZE - 1u;

			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), block_textures[GRID::TILE]);
		}
	}

	const unsigned int TOP		= OFFSET * 6u;
	const unsigned int BOTTOM	= OFFSET * 6u + BLOCK_SIZE * GRID_ROWS - 1u;
	const unsigned int LEFT		= OFFSET * 1u;
	const unsigned int RIGHT	= OFFSET * 1u + BLOCK_SIZE * GRID_COLS - 1u;

	grid_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);
}
void Minesweeper::InitialiseMinesCounter()
{	
	const unsigned int TOP = OFFSET * 3u - BLOCK_SIZE / 2u;
	const unsigned int RIGHT = OFFSET + BLOCK_SIZE * 3u;

	for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
	{
		for (unsigned int x = 0u; x < DIGIT_COLS; x++)
		{
			const RectUI POSITION = {
				TOP,
				TOP + BLOCK_SIZE - 1u,
				RIGHT - (BLOCK_SIZE * x) - BLOCK_SIZE,
				RIGHT - (BLOCK_SIZE * x) - 1u };

			mines_counter[y][x] = std::move(Block(POSITION, digit_textures[y]));
		}
	}
}
void Minesweeper::InitialiseResetButton()
{
	reset_button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_smiley_0.png"));
	reset_button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_smiley_1.png"));
	reset_button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_smiley_2.png"));

	const unsigned int TOP = OFFSET * 3u - BLOCK_SIZE / 2u;
	const unsigned int BOTTOM = TOP + BLOCK_SIZE - 1u;
	const unsigned int LEFT = OFFSET + (BLOCK_SIZE * GRID_COLS) / 2u - BLOCK_SIZE / 2u;
	const unsigned int RIGHT = LEFT + BLOCK_SIZE - 1u;

	reset_button_position				= { TOP,BOTTOM,LEFT,RIGHT };
	const RectUI RESET_BUTTON_OFFSET	= { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

	reset_button_blocks[0] = { reset_button_position, reset_button_textures[0] };
	reset_button_blocks[1] = { RESET_BUTTON_OFFSET, reset_button_textures[0] };
}
void Minesweeper::InitialiseTimer()
{
	const unsigned int TOP = OFFSET * 3u - BLOCK_SIZE / 2u;
	const unsigned int RIGHT = OFFSET + BLOCK_SIZE * GRID_COLS;

	for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
	{
		for (unsigned int x = 0u; x < DIGIT_COLS; x++)
		{
			const RectUI POSITION = {
				TOP,
				TOP + BLOCK_SIZE - 1u,
				RIGHT - (BLOCK_SIZE * x) - BLOCK_SIZE,
				RIGHT - (BLOCK_SIZE * x) - 1u };

			timer[y][x] = std::move(Block(POSITION, digit_textures[y]));
		}
	}
}
void Minesweeper::InitialiseGameOver()
{
	if (GRID_ROWS <= GRID_COLS)
	{
		const unsigned int SIZE = (BLOCK_SIZE * GRID_ROWS) / 2u;
		const unsigned int TOP = OFFSET * 6u + (BLOCK_SIZE * GRID_ROWS) / 2u - SIZE;
		const unsigned int BOTTOM = OFFSET * 6u + (BLOCK_SIZE * GRID_ROWS) / 2u + SIZE;
		const unsigned int LEFT = (OFFSET + BLOCK_SIZE * GRID_COLS + OFFSET) / 2u - SIZE;
		const unsigned int RIGHT = (OFFSET + BLOCK_SIZE * GRID_COLS + OFFSET) / 2u + SIZE;
		gameover_position = { TOP,BOTTOM,LEFT,RIGHT };
	}
	else
	{
		const unsigned int SIZE = (BLOCK_SIZE * GRID_COLS) / 2u;
		const unsigned int TOP = OFFSET * 6u + (BLOCK_SIZE * GRID_ROWS) / 2u - SIZE;
		const unsigned int BOTTOM = OFFSET * 6u + (BLOCK_SIZE * GRID_ROWS) / 2u + SIZE;
		const unsigned int LEFT = (OFFSET + BLOCK_SIZE * GRID_COLS + OFFSET) / 2u - SIZE;
		const unsigned int RIGHT = (OFFSET + BLOCK_SIZE * GRID_COLS + OFFSET) / 2u + SIZE;
		gameover_position = { TOP,BOTTOM,LEFT,RIGHT };
	}

	// EXPLOSION ANIMATION
	{
		const std::wstring filepath = L"Textures\\Minesweeper\\Explosion\\";
		const std::wstring filetype = L".png";

		for (unsigned int i = 0; i < EXPLOSION_NUM; i++)
		{
			const std::wstring number = std::to_wstring(i);
			const std::wstring filename =
				filepath + (std::wstring(4 - number.length(), '0') + number) + filetype;
			explosion_textures[i] = std::make_shared<Surface>(filename);
		}

		for (unsigned int i = 0; i < EXPLOSION_NUM; i++)
		{
			explosion_blocks[i] = { gameover_position, explosion_textures[i] };
		}
	}
	// FLAG ANIMATION

	{
		const std::wstring filepath = L"Textures\\Minesweeper\\Flag\\";
		const std::wstring filetype = L".png";

		for (unsigned int i = 0; i < FLAG_NUM; i++)
		{
			const std::wstring number = std::to_wstring(i + 10u);
			const std::wstring filename =
				filepath + (std::wstring(4 - number.length(), '0') + number) + filetype;
			flag_textures[i] = std::make_shared<Surface>(filename);
		}

		for (unsigned int i = 0; i < FLAG_NUM; i++)
		{
			flag_blocks[i] = { gameover_position, flag_textures[i] };
		}
	}
}

void Minesweeper::InitialiseSounds()
{
	sounds.emplace_back(L"Sounds\\explosion.wav");
	sounds.emplace_back(L"Sounds\\fanfare.wav");
	sounds.emplace_back(L"Sounds\\click_0.wav");
	sounds.emplace_back(L"Sounds\\click_1.wav");
	sounds.emplace_back(L"Sounds\\click_2.wav");
}

/*--------------------------------------------*/

void Minesweeper::SetBlockValues()
{
	block_values.assign(GRID_SIZE, 0u);

	std::vector<unsigned int> mines_index;

	for(unsigned int i = 0u; i < MINES; i++)
	{
		unsigned int INDEX = rnd::RandomInt(0u, GRID_SIZE - 1u);

		for(int i = 0u; i < mines_index.size(); i++)
		{
			while(INDEX == mines_index[i])
			{
				INDEX = rnd::RandomInt(0u, GRID_SIZE - 1u);
			}
		}
		mines_index.emplace_back(INDEX);
		block_values[INDEX] = 9u;
	}
		
	int ROWS = static_cast<int>(GRID_ROWS);
	int COLS = static_cast<int>(GRID_COLS);

	for(int y = 0; y < ROWS; y++)
	{
		for(int x = 0; x < COLS; x++)
		{
			if(block_values[static_cast<size_t>(y) * COLS + x] == 9u)
			{				
				SetBlockValue(x - 1, y - 1, COLS, ROWS);
				SetBlockValue(x + 0, y - 1, COLS, ROWS);
				SetBlockValue(x + 1, y - 1, COLS, ROWS);

				SetBlockValue(x - 1, y + 0, COLS, ROWS);
				SetBlockValue(x + 1, y + 0, COLS, ROWS);

				SetBlockValue(x - 1, y + 1, COLS, ROWS);
				SetBlockValue(x + 0, y + 1, COLS, ROWS);
				SetBlockValue(x + 1, y + 1, COLS, ROWS);				
			}										
		}											
	}
}
void Minesweeper::SetBlockValue(const int& X, const int& Y, const int& COLS, const int& ROWS)
{
	if(X >= 0 && X < COLS && Y >= 0 && Y < ROWS && block_values[static_cast<size_t>(Y) * COLS + X] != 9u)
	{
		block_values[static_cast<size_t>(Y) * COLS + X]++;
	}
}
void Minesweeper::RevealBlocks(const int& X, const int& Y, const int& COLS, const int& ROWS)
{
	RevealBlock(X - 1, Y - 1, COLS, ROWS);
	RevealBlock(X + 0, Y - 1, COLS, ROWS);
	RevealBlock(X + 1, Y - 1, COLS, ROWS);

	RevealBlock(X - 1, Y + 0, COLS, ROWS);
	RevealBlock(X + 1, Y + 0, COLS, ROWS);

	RevealBlock(X - 1, Y + 1, COLS, ROWS);
	RevealBlock(X + 0, Y + 1, COLS, ROWS);
	RevealBlock(X + 1, Y + 1, COLS, ROWS);
}
void Minesweeper::RevealBlock(const int& X, const int& Y, const int& COLS, const int& ROWS)
{
	if(X >= 0 && X < COLS && Y >= 0 && Y < ROWS)
	{
		const int INDEX = Y * COLS + X;
		
		if(!block_revealed[INDEX])
		{
			blocks[INDEX].SetTexture(block_textures[block_values[INDEX]]);
			block_revealed[INDEX] = true;

			if(block_values[INDEX] == 0u)
			{
				RevealBlocks(X, Y, COLS, ROWS);
			}		
		}
	}
}

void Minesweeper::SetMinesCounter()
{
	ExtractDigits(mines_number, mines);
	while (mines_number.size() < 3u){ mines_number.emplace_back(0u);	}
}
void Minesweeper::SetTimer()
{
	if (timer_started)
	{
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
		time = static_cast<unsigned int>(duration.count()) / 1000000u;
	}

	if (time > 999) { time = 0u; t1 = std::chrono::high_resolution_clock::now(); }

	ExtractDigits(timer_number, time);
	
	while (timer_number.size() < 3u)
	{
		timer_number.emplace_back(0u);
	}
}
void Minesweeper::ExtractDigits(std::vector<unsigned int>& vec, const unsigned int& NUM)
{
	vec.clear();
	unsigned int result = NUM;

	if(result > 0)
	{
		while(result > 0u)
		{
			vec.push_back(result % 10u);
			result /= 10u;
		}
	}
	else
	{
		vec.push_back(0u);
	}
}

/*--------------------------------------------*/

void Minesweeper::DrawGrid(Graphics& gfx)
{
	gfx.DrawRect(true, grid_position, Color(155, 155, 155));

	for(unsigned int y = 0; y <= GRID_ROWS; y++)
	{
		for(unsigned int x = 0; x <= GRID_COLS; x++)
		{
			// horizontal lines
			gfx.DrawLine(grid_position.left,
						 grid_position.top + BLOCK_SIZE * y,
						 grid_position.right,
						 grid_position.top + BLOCK_SIZE * y,
						 Colors::LightGray);
			
			// verticle lines
			gfx.DrawLine(grid_position.left + BLOCK_SIZE * x,
						 grid_position.top,
						 grid_position.left + BLOCK_SIZE * x,
						 grid_position.bottom,
						 Colors::LightGray);
		}
	}
}
void Minesweeper::DrawGridBlocks(Graphics& gfx)
{
	for(unsigned int i = 0; i < GRID_SIZE; i++)
	{
		if(!block_revealed[i])
		{
			blocks[i].Draw(gfx);
		}
		else if(block_values[i] != 0)
		{
			blocks[i].Draw(gfx);
		}
	}
}
void Minesweeper::DrawMouseOverBlocks(Graphics& gfx)
{
	for(auto& b : blocks)
	{
		if(b.GetMouseOver())
		{
			gfx.DrawRect(false, b.GetPosition(), Colors::Red);
		}
	}
}

void Minesweeper::DrawBorder(Graphics& gfx)
{
	for(auto b : border_blocks)
	{
		b.Draw(gfx);
	}
}

void Minesweeper::DrawDisplayBackground(Graphics& gfx)
{
	const unsigned int TOP = OFFSET;
	const unsigned int BOTTOM = OFFSET * 5u - 1u;
	const unsigned int LEFT = OFFSET;
	const unsigned int RIGHT = OFFSET + BLOCK_SIZE * GRID_COLS - 1u;

	background_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);

	gfx.DrawRect(true, background_position, Color(155, 155, 155));
}
void Minesweeper::DrawMinesCounter(Graphics& gfx)
{	
	for (unsigned int i = 0u; i < mines_number.size(); i++)
	{
		mines_counter[mines_number[i]][i].Draw(gfx);
	}	
}
void Minesweeper::DrawResetButton(Graphics& gfx)
{
	if (reset_button_pressed)
	{
		reset_button_blocks[1].Draw(gfx);
	}
	else
	{
		reset_button_blocks[0].Draw(gfx);
	}
}
void Minesweeper::DrawTimer(Graphics& gfx)
{
	for (unsigned int i = 0u; i < timer_number.size(); i++)
	{
		timer[timer_number[i]][i].Draw(gfx);
	}
}
void Minesweeper::DrawGameOver(Graphics& gfx)
{	
	if (gameover)
	{		
		if (gamewon)
		{
			if (index0 < FLAG_NUM)
			{
				flag_blocks[index0].Draw(gfx);
			}
			index0++;
		}
		else
		{
			if (index1 < EXPLOSION_NUM)
			{
				explosion_blocks[index1].Draw(gfx);
			}
			index1++;
		}
	}
}