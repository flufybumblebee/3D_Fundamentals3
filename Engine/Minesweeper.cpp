#include "Minesweeper.h"

Minesweeper::Minesweeper(const unsigned int& mines, const unsigned int& columns, const unsigned int& rows)
	:
	MINES(mines),
	GRID_COLS(columns),
	GRID_ROWS(rows),
	GRID_SIZE(columns * rows)
{
	assert(MINES < GRID_SIZE);

	block_values.assign(GRID_SIZE, 0u);
	block_revealed.assign(GRID_SIZE, false);
	block_flag.assign(GRID_SIZE, false);

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
													
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_0.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_1.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_2.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_3.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_4.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_5.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_6.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_7.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\digit_8.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\mine.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\tile_dark_grey.png"));
	block_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\flag.png"));
	
	const unsigned int OFFSET = 10u;
		
	GRID_W = SCREEN_W - OFFSET;
	GRID_H = SCREEN_H - OFFSET * 7;

	BLOCK_W = GRID_W / GRID_COLS;
	BLOCK_H = GRID_H / GRID_ROWS;

	display_position = RectUI(OFFSET, OFFSET * 5, OFFSET, SCREEN_W - OFFSET);
	grid_position = RectUI(OFFSET * 6, SCREEN_H - OFFSET, OFFSET, SCREEN_W - OFFSET);

	for(unsigned int y = 0u; y < GRID_ROWS; y++)
	{
		for(unsigned int x = 0u; x < GRID_COLS; x++)
		{
			const size_t INDEX = static_cast<size_t>(y) * GRID_COLS + x;

			const unsigned int TOP		= OFFSET * 6 + BLOCK_H * y;
			const unsigned int BOTTOM	= OFFSET * 6 + BLOCK_H * y + BLOCK_H - 0u;
			const unsigned int LEFT		= OFFSET * 1 + BLOCK_W * x;
			const unsigned int RIGHT	= OFFSET * 1 + BLOCK_W * x + BLOCK_W - 0u;

			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), block_textures[TEXTURE::TILE]);
		}
	} 
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
							// if( block revealed ) check nearest neighbours
							// else				    set block as flag
							if(!block_revealed[i])
							{
								if(block_flag[i])
								{
									blocks[i].SetTexture(block_textures[TEXTURE::TILE]);
									block_flag[i] = false;
								}
								else
								{
									blocks[i].SetTexture(block_textures[TEXTURE::FLAG]);
									block_flag[i] = true;
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
	gfx.DrawRect(true, display_position, Color(155, 155, 155));
	gfx.DrawRect(true, grid_position, Color(155, 155, 155));

	for(unsigned int y = 0; y <= GRID_ROWS; y++)
	{
		for(unsigned int x = 0; x <= GRID_COLS; x++)
		{
			// horizontal lines
			gfx.DrawLine(grid_position.left,
						 grid_position.top + BLOCK_H * y,
						 grid_position.right,
						 grid_position.top + BLOCK_H * y,
						 Colors::LightGray);
			
			// verticle lines
			gfx.DrawLine(grid_position.left + BLOCK_W * x,
						 grid_position.top,
						 grid_position.left + BLOCK_W * x,
						 grid_position.bottom,
						 Colors::LightGray);
		}
	}

	for(unsigned int i = 0; i < GRID_SIZE; i++)
	{
		if(!block_revealed[i] )
		{
			blocks[i].Draw(gfx);
		}
		else if(block_values[i] != 0)
		{
			blocks[i].Draw(gfx);
		}
	}
	
	for(unsigned int i = 0; i < GRID_SIZE; i++)
	{
		if(blocks[i].GetMouseOver())
		{
			gfx.DrawRect(false, blocks[i].GetPosition(), Colors::Red);
		}
	}
}

inline void Minesweeper::SetBlockValue(const int& X, const int& Y, const int& COLS, const int& ROWS)
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

inline void Minesweeper::RevealBlock(const int& X, const int& Y, const int& COLS, const int& ROWS)
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
