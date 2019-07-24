#include "Grid.h"

#include "Random.h"
#include "Bumble.h"

#include <algorithm>

Grid::Grid(
	const unsigned int& COLS,
	const unsigned int& ROWS,
	const unsigned int& MINES,
	const unsigned int& OFFSET)
	:
	COLS(std::max<unsigned int>(MIN_COLS, COLS)),
	ROWS(std::max<unsigned int>(MIN_ROWS, ROWS)),
	SIZE(this->COLS * this->ROWS),
	MINES(std::max<unsigned int>(1u, std::min<unsigned int>(SIZE - 1u, MINES))),
	OFFSET(OFFSET),
	TILE_SIZE(Bumble::SetSize(this->COLS,this->ROWS,OFFSET)),
	GRID_POSITION(OFFSET * 6u, OFFSET * 6u + TILE_SIZE * this->ROWS - 1u,OFFSET, OFFSET + TILE_SIZE * this->COLS - 1u),
	COLOR_START(Color(rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u))),
	COLOR_END(Color(rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u)))
{
	InitialiseTiles();
	InitialiseBackground();
	SetTileValues();
}

void Grid::Update()
{
	for (auto& t : tiles)
	{		
		if (!t.Revealed())
		{
			if (t.Flag())
			{
				t.SetTexture(tile_textures[TILE::FLAG]);
			}
			else if (t.Checked())
			{
				t.SetTexture(tile_textures[0]);
			}
			else
			{				
				t.SetTexture(tile_textures[TILE::BLANK_TILE]);
			}
		}
		else
		{
			t.SetTexture(tile_textures[t.Value()]);
		}
	}
}
void Grid::Draw(Graphics& gfx)
{
	DrawBackground(gfx);
	DrawMouseOverTiles(gfx);
	DrawTiles(gfx);
}

void Grid::InitialiseTiles()
{
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\tile_0.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\tile_1.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\tile_2.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\tile_3.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\tile_4.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\tile_5.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_yellow.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_violet.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\tile_mine.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_blank.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_flag.png"));

	for (unsigned int y = 0u; y < ROWS; y++)     
	{
		for (unsigned int x = 0u; x < COLS; x++)
		{
			const unsigned int TOP		= GRID_POSITION.top + TILE_SIZE * y;
			const unsigned int BOTTOM	= GRID_POSITION.top + TILE_SIZE * y + TILE_SIZE/* - 1u*/;
			const unsigned int LEFT		= GRID_POSITION.left + TILE_SIZE * x;
			const unsigned int RIGHT	= GRID_POSITION.left + TILE_SIZE * x + TILE_SIZE/* - 1u*/;

			const RectUI POSITION = RectUI(TOP, BOTTOM, LEFT, RIGHT);
			
			tiles.emplace_back(POSITION, tile_textures[TILE::BLANK_TILE]);
		}
	}
}
void Grid::InitialiseBackground()
{
	background_textures.emplace_back(std::make_shared<Surface>(Bumble::CreateColorBlendTexture(GRID_POSITION, COLOR_START, COLOR_END)));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.png")));
	
	const size_t MIN = 0;
	const size_t MAX = background_textures.size() - 1;

	background = Block(GRID_POSITION, background_textures[rnd::RandomInt(MIN,MAX)]);
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
				j = 0u;
			}
		}
		index_array.emplace_back(index);

		tiles[index].SetValue(9u);
	}

	unsigned int count = 0u;

	for (int i = 0; i < tiles.size(); i++)
	{		
		if (tiles[i].Value() == 9u)
		{
			count++;
		}
	}

	assert(count == MINES);

	size_t i = 0;
	size_t j = 0;
	for (int y = 0; y < static_cast<int>(ROWS); y++)
	{
		for (int x = 0; x < static_cast<int>(COLS); x++)
		{
			i = static_cast<size_t>(y) * COLS + x;

			if (tiles[i].Value() == 9u)
			{
				for (int yy = -1; yy < 2; yy++)
				{
					for (int xx = -1; xx < 2; xx++)
					{
						if (x + xx >= 0 && x + xx < static_cast<int>(COLS) &&
							y + yy >= 0 && y + yy < static_cast<int>(ROWS))
						{
							j = (static_cast<size_t>(y) +yy) * COLS + (static_cast<size_t>(x) + xx);

							if (tiles[j].Value() != 9)
							{
								SetTileValue(j);
							}
						}
					}
				}
			}
		}
	}
}
void Grid::SetTileValue(const size_t& INDEX)
{
	tiles[INDEX].SetValue(tiles[INDEX].Value() + 1u);
}
void Grid::RevealTiles(const int& X, const int& Y)
{
	for (int yy = -1; yy < 2; yy++)
	{
		for (int xx = -1; xx < 2; xx++)
		{
			if (X + xx >= 0 && X + xx < static_cast<int>(COLS) &&
				Y + yy >= 0 && Y + yy < static_cast<int>(ROWS))
			{
				RevealTile(X + xx, Y + yy);
			}
		}
	}
}
void Grid::RevealTile(const int& X, const int& Y)
{
	const int INDEX = Y * COLS + X;

	if (!tiles[INDEX].Revealed() && !tiles[INDEX].Flag())
	{
		tiles[INDEX].SetIsRevealed(true);

		if (tiles[INDEX].Value() == 0u)
		{
			RevealTiles(X, Y);
		}
	}
}

void Grid::CheckTiles(const int& X, const int& Y)
{
	const unsigned int INDEX = Y * COLS + X;

	size_t i = 0;
	if (!Revealed(INDEX))
	{
		for (int y = -1; y < 2; y++)
		{
			for (int x = -1; x < 2; x++)
			{
				if (X + x >= 0 && X + x < static_cast<int>(COLS) &&
					Y + y >= 0 && Y + y < static_cast<int>(ROWS))
				{
					i = (static_cast<size_t>(Y) + y) * COLS + (static_cast<size_t>(X) + x);
					tiles[i].SetIsChecked(true);
				}
				else
				{
					continue;
				}
			}
		}		
	}
	else
	{
		/*CheckTile(X - 1, Y - 1);
		CheckTile(X + 0, Y - 1);
		CheckTile(X + 1, Y - 1);

		CheckTile(X - 1, Y + 0);
		CheckTile(X + 1, Y + 0);

		CheckTile(X - 1, Y + 1);
		CheckTile(X + 0, Y + 1);
		CheckTile(X + 1, Y + 1);*/
	}
}
void Grid::CheckTile(const int& X, const int& Y)
{
	const unsigned int INDEX = Y * COLS + X;
	if (INDEX >= 0 && INDEX < SIZE)
	{
		if (Y == -1)
		{
			if (X == -1)
			{
				//top left
			}
			else if (X == 0)
			{
				// top middle
			}
			else if (X == 1)
			{
				// top right
			}
		}
		else if (Y == 0)
		{
			if (X == -1)
			{
				//middle left
			}
			else if (X == 0)
			{
				// middle middle
			}
			else if (X == 1)
			{
				// middle right
			}
		}
		else if (Y == 1)
		{
			if (X == -1)
			{
				//bottom left
			}
			else if (X == 0)
			{
				// bottom middle
			}
			else if (X == 1)
			{
				// bottom right
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

RectUI Grid::GetGridRect() const
{
	return GRID_POSITION;
}

unsigned int Grid::Value(const unsigned int& INDEX) const
{
	return tiles[INDEX].Value();
}
bool Grid::Flag(const unsigned int& INDEX) const
{
	return tiles[INDEX].Flag();
}
bool Grid::Revealed(const unsigned int& INDEX) const
{
	return tiles[INDEX].Revealed();
}
bool Grid::Checked(const unsigned int& INDEX) const
{
	return tiles[INDEX].Checked();
}
bool Grid::Mine(const unsigned int& INDEX) const
{
	return tiles[INDEX].Mine();
}
bool Grid::MouseOver(const unsigned int& INDEX) const
{
	return tiles[INDEX].MouseOver();
}

void Grid::SetIsFlag(const unsigned int& INDEX, const bool& IS_FLAG)
{
	tiles[INDEX].SetIsFlag(IS_FLAG);
}
void Grid::SetIsRevealed(const unsigned int& INDEX, const bool& IS_REVEALED)
{
	tiles[INDEX].SetIsRevealed(IS_REVEALED);
}
void Grid::SetIsChecked(const unsigned int& INDEX, const bool& IS_CHECKED)
{
	tiles[INDEX].SetIsChecked(IS_CHECKED);
}
void Grid::SetMouseOver(const unsigned int& INDEX, Mouse& mouse)
{
	tiles[INDEX].SetMouseOver(mouse);
}
void Grid::SetBackground()
{
	const size_t MIN = 0;
	const size_t MAX = background_textures.size() - 1;
	const size_t INDEX = rnd::RandomInt(MIN, MAX);

	if (INDEX == 0u)
	{
		Color color_start = Color(rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u));
		Color color_end = Color(rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u));
		background_textures[0] = std::make_shared<Surface>(Bumble::CreateColorBlendTexture(GRID_POSITION, color_start, color_end));
	}

	background = Block(GRID_POSITION, background_textures[INDEX]);
}

void Grid::DrawBackground(Graphics& gfx)
{
	background.Draw(gfx);
}
void Grid::DrawTiles(Graphics& gfx)
{
	for (auto& t : tiles)
	{
		t.Draw(gfx);
	}
}
void Grid::DrawMouseOverTiles(Graphics& gfx)
{
	for (auto& t : tiles)
	{
		if (t.MouseOver())
		{
			mouseover = Block(t.Position(), std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Tiles\\tile_mouseover.png")));
			mouseover.Draw(gfx);
		}
	}
}