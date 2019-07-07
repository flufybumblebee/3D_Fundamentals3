#include "Grid.h"

#include "Random.h"
#include "BumbleFunctions.h"

Grid::Grid(
	const unsigned int& COLS,
	const unsigned int& ROWS,
	const unsigned int& MINES,
	const unsigned int& OFFSET)
	:
	COLS(std::max<unsigned int>(MIN_COLS, COLS)),
	ROWS(std::max<unsigned int>(MIN_ROWS, ROWS)),
	SIZE(COLS * ROWS),
	MINES(std::max<unsigned int>(1u, std::min<unsigned int>(SIZE - 1u, MINES))),
	OFFSET(OFFSET),
	TILE_SIZE(BumbleFunctions::SetSize(COLS,ROWS,OFFSET))
{
	/*if (SCREEN_H <= SCREEN_W)
	{
		const unsigned int WIDTH = SCREEN_W - OFFSET * 2u;
		const unsigned int HEIGHT = SCREEN_H - OFFSET * 7u;
		TILE_SIZE = HEIGHT / ROWS;

		while (TILE_SIZE * COLS > WIDTH)
		{
			TILE_SIZE--;
		}
	}
	else
	{
		const unsigned int WIDTH = SCREEN_W - OFFSET * 2u;
		const unsigned int HEIGHT = SCREEN_H - OFFSET * 7u;
		TILE_SIZE = WIDTH / COLS;

		while (TILE_SIZE * ROWS > HEIGHT)
		{
			TILE_SIZE--;
		}
	}*/

	const unsigned int TOP		= OFFSET * 6u;
	const unsigned int BOTTOM	= TOP + TILE_SIZE * ROWS - 1u;
	const unsigned int LEFT		= OFFSET;
	const unsigned int RIGHT	= LEFT + TILE_SIZE * COLS - 1u;

	grid_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);

	InitialiseTiles();
	SetTileValues();
}

void Grid::Update()
{
	for (auto& t : tiles)
	{
		if (!t.GetIsRevealed())
		{
			if (!t.GetIsFlag())
			{
				t.SetTexture(tile_textures[TILE::BLANK_TILE]);
			}
			else
			{
				t.SetTexture(tile_textures[TILE::FLAG]);
			}
		}
		else
		{
			t.SetTexture(tile_textures[t.GetValue()]);
		}
	}
}
void Grid::Draw(Graphics& gfx)
{
	DrawGrid(gfx);
	DrawTiles(gfx);
	DrawMouseOverTiles(gfx);
}

void Grid::InitialiseTiles()
{
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_0_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_1_blue.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_2_green.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_3_red.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_4_cyan.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_5_magenta.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_yellow.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_violet.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\mine.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_blank.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_flag.png"));

	for (unsigned int y = 0u; y < ROWS; y++)     
	{
		for (unsigned int x = 0u; x < COLS; x++)
		{
			const unsigned int TOP		= grid_position.top + TILE_SIZE * y;
			const unsigned int BOTTOM	= grid_position.top + TILE_SIZE * y + TILE_SIZE - 1u;
			const unsigned int LEFT		= grid_position.left + TILE_SIZE * x;
			const unsigned int RIGHT	= grid_position.left + TILE_SIZE * x + TILE_SIZE - 1u;

			const RectUI POSITION = RectUI(TOP, BOTTOM, LEFT, RIGHT);
			
			tiles.emplace_back(POSITION, tile_textures[TILE::BLANK_TILE]);
		}
	}
}

void Grid::SetTileValues()
{
	for (auto& t : tiles)
	{
		t.Reset();
	}

	std::vector<unsigned int> index_array;

	for (unsigned int i = 0u; i < MINES; i++)
	{
		unsigned int index = rnd::RandomInt(0u, SIZE - 1u);

		for (unsigned int j = 0u; j < index_array.size(); j++)
		{
			while (index == index_array[j])
			{
				index = rnd::RandomInt(0u, SIZE - 1u);
			}
		}
		index_array.emplace_back(index);

		tiles[index].SetValue(9u);
	}

	int i = 0;
	for (int y = 0; y < static_cast<int>(ROWS); y++)
	{
		for (int x = 0; x < static_cast<int>(COLS); x++)
		{
			i = static_cast<size_t>(y) * COLS + x;

			if (tiles[i].GetValue() == 9u)
			{
				SetTileValue(x - 1, y - 1);
				SetTileValue(x + 0, y - 1);
				SetTileValue(x + 1, y - 1);

				SetTileValue(x - 1, y + 0);
				SetTileValue(x + 1, y + 0);

				SetTileValue(x - 1, y + 1);
				SetTileValue(x + 0, y + 1);
				SetTileValue(x + 1, y + 1);
			}
		}
	}
}
void Grid::SetTileValue(const int& X, const int& Y)
{
	const unsigned int INDEX = static_cast<size_t>(Y) * COLS + X;
	
	if (X >= 0 && X < static_cast<int>(COLS) &&
		Y >= 0 && Y < static_cast<int>(ROWS) &&
		tiles[INDEX].GetValue() != 9u)
	{
		tiles[INDEX].SetValue(tiles[INDEX].GetValue() + 1u);
	}
}
void Grid::RevealTiles(const int& X, const int& Y)
{
	RevealTile(X - 1, Y - 1);
	RevealTile(X + 0, Y - 1);
	RevealTile(X + 1, Y - 1);

	RevealTile(X - 1, Y + 0);
	RevealTile(X + 1, Y + 0);

	RevealTile(X - 1, Y + 1);
	RevealTile(X + 0, Y + 1);
	RevealTile(X + 1, Y + 1);
}
void Grid::RevealTile(const int& X, const int& Y)
{
	if (X >= 0 && X < static_cast<int>(COLS) &&
		Y >= 0 && Y < static_cast<int>(ROWS))
	{
		const int INDEX = Y * COLS + X;

		if (!tiles[INDEX].GetIsRevealed() && !tiles[INDEX].GetIsFlag())
		{
			tiles[INDEX].SetIsRevealed(true);

			if (tiles[INDEX].GetValue() == 0u)
			{
				RevealTiles(X, Y);
			}
		}
	}
}

unsigned int Grid::GetCols() const
{
	return COLS;
}
unsigned int Grid::GetRows() const
{
	return ROWS;
}
unsigned int Grid::GetGridSize() const
{
	return SIZE;
}
unsigned int Grid::GetMines() const
{
	return MINES;
}
unsigned int Grid::GetTileSize() const
{
	return TILE_SIZE;
}

unsigned int Grid::GetValue(const unsigned int& INDEX) const
{
	return tiles[INDEX].GetValue();
}
bool Grid::GetIsFlag(const unsigned int& INDEX) const
{
	return tiles[INDEX].GetIsFlag();
}
bool Grid::GetIsRevealed(const unsigned int& INDEX) const
{
	return tiles[INDEX].GetIsRevealed();
}
bool Grid::GetMouseOver(const unsigned int& INDEX) const
{
	return tiles[INDEX].GetMouseOver();
}

void Grid::SetIsFlag(const unsigned int& INDEX, const bool& IS_FLAG)
{
	tiles[INDEX].SetIsFlag(IS_FLAG);
}
void Grid::SetIsRevealed(const unsigned int& INDEX, const bool& IS_REVEALED)
{
	tiles[INDEX].SetIsRevealed(IS_REVEALED);
}
void Grid::SetMouseOver(const unsigned int& INDEX, Mouse& mouse)
{
	tiles[INDEX].SetMouseOver(mouse);
}

void Grid::DrawGrid(Graphics& gfx)
{
	gfx.DrawRect(true, grid_position, Color(155, 155, 155));

	for (unsigned int y = 0; y <= ROWS; y++)
	{
		for (unsigned int x = 0; x <= COLS; x++)
		{
			// horizontal lines
			gfx.DrawLine(
				grid_position.left,
				grid_position.top + TILE_SIZE * y,
				grid_position.right,
				grid_position.top + TILE_SIZE * y,
				Colors::LightGray);

			// verticle lines
			gfx.DrawLine(
				grid_position.left + TILE_SIZE * x,
				grid_position.top,
				grid_position.left + TILE_SIZE * x,
				grid_position.bottom,
				Colors::LightGray);
		}
	}
}
void Grid::DrawTiles(Graphics& gfx)
{
	for (auto t : tiles)
	{
		if(!t.GetIsRevealed())
		{
			t.Draw(gfx);
		}
		else if( t.GetValue() != 0u )
		{
			t.Draw(gfx);
		}
	}
}
void Grid::DrawMouseOverTiles(Graphics& gfx)
{
	/*for (auto& b : blocks)
	{
		if (b.GetMouseOver())
		{
			gfx.DrawRect(false, b.GetPosition(), Colors::Red);
		}
	}*/

	for (auto& t : tiles)
	{
		if (t.GetMouseOver())
		{
			gfx.DrawRect(false, t.GetPosition(), Colors::Red);
		}
	}
}