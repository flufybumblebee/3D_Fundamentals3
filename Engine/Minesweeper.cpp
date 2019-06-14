#include "Minesweeper.h"

Minesweeper::Minesweeper(const unsigned int& mines, const unsigned int& columns, const unsigned int& rows)
	:
	MINE_NUM(mines),
	GRID_COLS(columns),
	GRID_ROWS(rows),
	GRID_SIZE(columns * rows)
{
	assert(MINE_NUM < GRID_SIZE);

	block_value.assign(GRID_SIZE, 0u);

	for (unsigned int i = 0u; i < MINE_NUM; i++)
	{
		const unsigned int INDEX = rnd::RandomInt(0u, GRID_SIZE - 1u);
		block_value[INDEX] = 9u;
	}

	for (int y = 0u; y < static_cast<int>(GRID_ROWS); y++)
	{
		for (int x = 0u; x < static_cast<int>(GRID_COLS); x++)
		{
			if (block_value[y * static_cast<size_t>(GRID_COLS) + x] == 9u)
			{
				int Y;
				int X;
				size_t INDEX;

				if (true)
				{
					// TOP ROW
					if (y - 1 >= 0 && x - 1 >= 0)
					{
						Y = y - 1;
						X = x - 1;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}
					if (y - 1 >= 0)
					{
						Y = y - 1;
						X = x;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}
					if (y - 1 >= 0 && x + 1 < static_cast<int>(GRID_COLS))
					{
						Y = y - 1;
						X = x + 1;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}

					// MIDDLE ROW
					if (x - 1 >= 0)
					{
						Y = y;
						X = x - 1;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}
					if (x + 1 < static_cast<int>(GRID_COLS))
					{
						Y = y;
						X = x + 1;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}

					// BOTTOM ROW
					if (y + 1 < static_cast<int>(GRID_ROWS) && x - 1 >= 0)
					{
						Y = y + 1;
						X = x - 1;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}
					if (y + 1 < static_cast<int>(GRID_ROWS))
					{
						Y = y + 1;
						X = x;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}
					if (y + 1 < static_cast<int>(GRID_ROWS) && x + 1 < static_cast<int>(GRID_COLS))
					{
						Y = y + 1;
						X = x + 1;
						INDEX = static_cast<size_t>(Y) * GRID_COLS + X;

						if (block_value[INDEX] != 9u)
						{
							block_value[INDEX]++;
						}
					}
				}
			}
		}
	}

	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_0.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_1.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_2.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_3.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_4.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_5.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_6.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_7.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_8.png"));
	block_value_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Digits\\digit_9.png"));


	block_user_textures.emplace_back(std::make_shared<Surface>(L"Textures\\mine.png"));

	BLOCK_W = SCREEN_W / GRID_COLS;
	BLOCK_H = SCREEN_H / GRID_ROWS;

	for (unsigned int y = 0u; y < GRID_ROWS; y++)
	{
		for (unsigned int x = 0u; x < GRID_COLS; x++)
		{
			const size_t INDEX = static_cast<size_t>(y) * GRID_COLS + x;

			const unsigned int TOP = BLOCK_H * y;
			const unsigned int BOTTOM = BLOCK_H * y + BLOCK_H - 1u;
			const unsigned int LEFT = BLOCK_W * x;
			const unsigned int RIGHT = BLOCK_W * x + BLOCK_W - 1u;

			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), block_value_textures[block_value[INDEX]]);
		}
	}
}

void Minesweeper::Update(Mouse& mouse)
{
	if (mouse.IsInWindow())
	{
		for (unsigned int i = 0; i < GRID_SIZE; i++)
		{
			blocks[i].SetMouseOver(mouse);
		}
	}
}

void Minesweeper::Draw(Graphics& gfx)
{
	for (auto g : blocks)
	{
		g.Draw(gfx);
	}

	for (unsigned int i = 0; i < GRID_SIZE; i++)
	{
		if (blocks[i].GetMouseOver())
		{
			gfx.DrawRect(false, blocks[i].GetPosition(), Colors::Red);
		}
	}
}
