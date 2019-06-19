#include "Minesweeper.h"

Minesweeper::Minesweeper(const unsigned int& MINES, const unsigned int& COLS, const unsigned int& ROWS)
	:
	MINES(MINES),
	GRID_COLS(COLS),
	GRID_ROWS(ROWS),
	GRID_SIZE(COLS * ROWS)
{
	assert(MINES < GRID_SIZE);

	mines = MINES;
	block_revealed.assign(GRID_SIZE, false);
	block_flag.assign(GRID_SIZE, false);

	InitialiseTextures();
	InitialiseBlockValues();		

	{
		const unsigned int TOP		= 0u;
		const unsigned int BOTTOM	= OFFSET * 6 + BLOCK_SIZE * GRID_ROWS + OFFSET - 1u;
		const unsigned int LEFT		= 0u;
		const unsigned int RIGHT	= OFFSET + BLOCK_SIZE * GRID_COLS + OFFSET - 1u;;

		border_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);
	}

	{
		const unsigned int TOP		= OFFSET;
		const unsigned int BOTTOM	= OFFSET * 5u - 1u;
		const unsigned int LEFT		= OFFSET;
		const unsigned int RIGHT	= OFFSET + BLOCK_SIZE * GRID_COLS - 1u;
		
		display_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);
	}

	{
		const unsigned int TOP		= OFFSET * 6u;
		const unsigned int BOTTOM	= OFFSET * 6u + BLOCK_SIZE * GRID_ROWS - 1u;	
		const unsigned int LEFT		= OFFSET;
		const unsigned int RIGHT	= OFFSET + BLOCK_SIZE * GRID_COLS - 1u;
		
		grid_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);				
	}	
	
	InitialiseBorder();
	InitialiseBlocks();	
	InitialiseDisplayMines();
}

void Minesweeper::Update(Mouse& mouse)
{
	if(!gameover && mouse.IsInWindow())
	{
		for(auto& b : blocks)
		{
			b.SetMouseOver(mouse);
		}

		int i = 0;
		const int COLS = static_cast<int>(GRID_COLS);
		const int ROWS = static_cast<int>(GRID_ROWS);

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
							// reveal block
							blocks[i].SetTexture(block_textures[block_values[i]]);
							block_revealed[i] = true;

							if(block_values[i] == 9u)
							{
								gameover = true;
							}
							else if(block_values[i] == 0u)
							{								
								RevealBlocks(x, y, COLS, ROWS);
							}

							mouse_pressed = true;
						}
						else if(mouse.RightIsPressed())
						{
							if(!block_revealed[i])
							{
								if(block_flag[i])
								{
									blocks[i].SetTexture(block_textures[GRID::TILE]);
									block_flag[i] = false;
									flags--;
									assert(flags >= 0u);
									mines++;
									assert(mines <= MINES);
								}
								else
								{
									blocks[i].SetTexture(block_textures[GRID::FLAG]);
									block_flag[i] = true;
									flags++;
									mines--;
									assert(mines >= 0u);
								}
							}
							mouse_pressed = true;
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

	ExtractDigits(mines_number, mines);

	if(gameover)
	{
		for(unsigned int i = 0u; i < GRID_SIZE; i++)
		{
			if(!block_revealed[i] && block_values[i] == 9u )
			{
				blocks[i].SetTexture(block_textures[block_values[i]]);
				block_revealed[i] = true;
			}
		}
	}
}
void Minesweeper::Draw(Graphics& gfx)
{
	DrawBorder(gfx);
	DrawDisplay(gfx);
	DrawGrid(gfx);
	DrawBlocks(gfx);
	DrawMouseOverBlocks(gfx);	
}

void Minesweeper::InitialiseTextures()
{
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_0_white.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_1_blue.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_2_green.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_3_red.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_4_cyan.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_5_magenta.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_6_yellow.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_7_violet.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_8_white.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\mine.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\tile.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\flag.png"));

	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_0_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_1_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_2_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_3_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_4_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_5_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_6_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_7_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_8_white.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_9_white.png"));

	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_horizontal.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_verticle.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_corner_top_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_corner_top_right.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_corner_bottom_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_corner_bottom_right.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_t_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\border_t_right.png"));
}
void Minesweeper::InitialiseBorder()
{
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
void Minesweeper::InitialiseBlockValues()
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
void Minesweeper::InitialiseBlocks()
{
	for(unsigned int y = 0u; y < GRID_ROWS; y++)
	{
		for(unsigned int x = 0u; x < GRID_COLS; x++)
		{
			const size_t INDEX = static_cast<size_t>(y) * GRID_COLS + x;

			const unsigned int TOP		= grid_position.top + BLOCK_SIZE * y;
			const unsigned int BOTTOM	= grid_position.top + BLOCK_SIZE * y + BLOCK_SIZE - 1u;
			const unsigned int LEFT		= grid_position.left + BLOCK_SIZE * x;
			const unsigned int RIGHT	= grid_position.left + BLOCK_SIZE * x + BLOCK_SIZE - 1u;

			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), block_textures[GRID::TILE]);
		}
	}
}

void Minesweeper::InitialiseDisplayMines()
{
	/*for(int i = 0; i < mines_number.size(); i++)
	{
		display_mines[i][i].
	}*/
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

void Minesweeper::ExtractDigits(std::vector<unsigned int>& ints, const unsigned int& NUM)
{
	ints.clear();
	unsigned int result = NUM;

	if(result > 0)
	{
		while(result > 0u)
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

void Minesweeper::DrawDisplay(Graphics& gfx)
{
	gfx.DrawRect(true, display_position, Color(155, 155, 155));
}
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
void Minesweeper::DrawBlocks(Graphics& gfx)
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
void Minesweeper::DrawBorder(Graphics& gfx)
{
	for(auto b : border_blocks)
	{
		b.Draw(gfx);
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

void Minesweeper::DrawDisplayMines(Graphics& gfx)
{
	for(int i = 0; i < mines_number.size(); i++)
	{
		//gfx.DrawTriangleTex()
	}
}
