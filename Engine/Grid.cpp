#include "Grid.h"

#include "Random.h"
#include "Bumble.h"

#include <algorithm>

Grid::Grid(std::vector<std::shared_ptr<Surface>> textures,
	const unsigned int& COLS,
	const unsigned int& ROWS,
	const unsigned int& MINES,
	const unsigned int& OFFSET)
	:
	tile_textures(textures),
	COLS(std::max<unsigned int>(MIN_COLS, COLS)),
	ROWS(std::max<unsigned int>(MIN_ROWS, ROWS)),
	SIZE(static_cast<size_t>(this->COLS) * this->ROWS),
	MINES(std::max<unsigned int>(1u, std::min<unsigned int>(static_cast<unsigned int>(SIZE) - 1u, MINES))),
	OFFSET(OFFSET),
	TILE_SIZE(SetTileSize(this->COLS,this->ROWS,OFFSET)),
	GRID_RECT(SetGridRect(TILE_SIZE,this->COLS,this->ROWS,OFFSET))
{
	InitialiseTiles();
	InitialiseBackground();
	SetTileValues();
}

unsigned int Grid::SetTileSize(const unsigned int& COLS, const unsigned int& ROWS, const unsigned int& OFFSET)
{
	unsigned int size = 0u;

	const unsigned int WIDTH = Graphics::ScreenWidth - OFFSET * 2u;
	const unsigned int HEIGHT = Graphics::ScreenHeight - OFFSET * 7u;

	if (HEIGHT / ROWS <= WIDTH / COLS)
	{
		size = HEIGHT / ROWS;
	}
	else
	{
		size = WIDTH / COLS;
	}

	return size;
}
RectUI Grid::SetGridRect(const unsigned int& TILE_SIZE, const unsigned int& COLS, const unsigned int& ROWS, const unsigned int& OFFSET)
{
	const unsigned int TOP		= OFFSET * 6u + (SCREEN_H - OFFSET * 6u) / 2 - TILE_SIZE * ROWS / 2;
	const unsigned int BOTTOM	= TOP + TILE_SIZE * ROWS - 1u;
	const unsigned int LEFT		= SCREEN_W / 2u - (TILE_SIZE * COLS) / 2u;
	const unsigned int RIGHT	= LEFT + TILE_SIZE * COLS - 1u;

	const RectUI RECT = { TOP, BOTTOM, LEFT, RIGHT };

	return RECT;
}

void Grid::InitialiseTiles()
{
	const unsigned int OFF = TILE_SIZE / 20u;

	for (unsigned int y = 0u; y < ROWS; y++)     
	{
		for (unsigned int x = 0u; x < COLS; x++)
		{
			const unsigned int TOP = GRID_RECT.top + TILE_SIZE * y;
			const unsigned int BOTTOM = GRID_RECT.top + TILE_SIZE * y + TILE_SIZE/* - 1u*/;
			const unsigned int LEFT = GRID_RECT.left + TILE_SIZE * x;
			const unsigned int RIGHT = GRID_RECT.left + TILE_SIZE * x + TILE_SIZE/* - 1u*/;

			std::vector<Block> blocks;

			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), tile_textures[TILE::MOUSEOVER]);
			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), tile_textures[TILE::TILE_DARK]);
			blocks.emplace_back(RectUI(TOP, BOTTOM, LEFT, RIGHT), tile_textures[TILE::BLANK]);

			tiles.emplace_back(blocks);

			const size_t INDEX = static_cast<size_t>(y) * COLS + x;
			tiles[INDEX].SetMouseoverRect(RectUI(TOP+OFF, BOTTOM-OFF, LEFT+OFF, RIGHT-OFF));
		}
	}
}
void Grid::InitialiseBackground()
{
	const Color COLOR_START = Bumble::RandomColor();
	const Color COLOR_END = Bumble::RandomColor();

	background_textures.emplace_back(std::make_shared<Surface>(Bumble::CreateColorBlendTexture(GRID_RECT, COLOR_START, COLOR_END)));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature0.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature2.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature5.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Space1.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\BlocksBlue.png")));
	background_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\BlocksGreen.png")));
	
	const size_t MIN = 0;
	const size_t MAX = background_textures.size() - 1;

	background = { GRID_RECT, background_textures[rnd::RandomInt(MIN,MAX)] };
}

void Grid::SetTileValues()
{
	for (auto& t : tiles)
	{
		t.Reset();
	}

	std::vector<unsigned int> index_array;
	
	const unsigned int MIN = 0u;
	const unsigned int MAX = static_cast<unsigned int>(SIZE) - 1u;
	for (unsigned int i = 0u; i < MINES; i++)
	{
		unsigned int index = rnd::RandomInt(MIN,MAX);

		for (unsigned int j = 0u; j < index_array.size(); j++)
		{
			while (index == index_array[j])
			{
				index = rnd::RandomInt(MIN, MAX);
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
							j = (static_cast<size_t>(y) + yy) * COLS + (static_cast<size_t>(x) + xx);

							if (tiles[j].Value() != 9)
							{
								tiles[j].SetValue(tiles[j].Value() + 1u);
							}
						}
					}
				}
			}
		}
	}
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
		tiles[INDEX].SetRevealed(true);

		if (tiles[INDEX].Value() == 0u)
		{
			RevealTiles(X, Y);
		}
	}
}

bool Grid::CheckTiles(const int& X, const int& Y, const bool& IS_CHECKED)
{
	const size_t INDEX = static_cast<size_t>(Y) * COLS + X;
	
	size_t i = 0;
	size_t j = 0;
	unsigned int mines = 0u;

	std::vector<size_t> indices;
	std::vector<int> ix;
	std::vector<int> iy;

	for (int y = -1; y < 2; y++)
	{
		for (int x = -1; x < 2; x++)
		{
			if (X + x >= 0 && X + x < static_cast<int>(COLS) &&
				Y + y >= 0 && Y + y < static_cast<int>(ROWS))
			{
				i = (static_cast<size_t>(Y) + y) * COLS + (static_cast<size_t>(X) + x);

				if (i != INDEX)
				{
					indices.emplace_back(i);
					ix.emplace_back(x);
					iy.emplace_back(y);
				}
			}
		}
	}

	if(!tiles[INDEX].Revealed())
	{
		tiles[INDEX].SetChecked(IS_CHECKED);

		for (i = 0; i < indices.size(); i++)
		{
			tiles[indices[i]].SetChecked(IS_CHECKED);
		}
	}
	else
	{
		if (tiles[INDEX].Value() >= 1u &&
			tiles[INDEX].Value() < 9u)
		{
			for (i = 0; i < indices.size(); i++)
			{
				tiles[indices[i]].SetChecked(IS_CHECKED);

				if (tiles[indices[i]].Flag() && !tiles[indices[i]].Mine())
				{
					gameover = true;
				}
				else if (tiles[indices[i]].Flag() && tiles[indices[i]].Mine())
				{
					mines++;
				}
			}

			for (i = 0; i < indices.size(); i++)
			{
				if (gameover)
				{
					if (!tiles[indices[i]].Flag() && tiles[indices[i]].Mine())
					{
						tiles[indices[i]].SetExploded(true);
					}
				}
			}

			for (i = 0; i < indices.size(); i++)
			{
				if (tiles[INDEX].Value() == mines)
				{
					if (!tiles[indices[i]].Revealed())
					{
						RevealTile(X + ix[i], Y + iy[i]);
					}
				}
			}
		}
	}

	return gameover;
}

unsigned int	Grid::Cols() const
{
	return COLS;
}
unsigned int	Grid::Rows() const
{
	return ROWS;
}
size_t			Grid::GridSize() const
{
	return SIZE;
}
unsigned int	Grid::Mines() const
{
	return MINES;
}
unsigned int	Grid::TileSize() const
{
	return TILE_SIZE;
}

RectUI Grid::GridRect() const
{
	return GRID_RECT;
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

void Grid::SetFlag(const unsigned int& INDEX, const bool& IS_FLAG)
{
	tiles[INDEX].SetFlag(IS_FLAG);
}
void Grid::SetRevealed(const unsigned int& INDEX, const bool& IS_REVEALED)
{
	tiles[INDEX].SetRevealed(IS_REVEALED);
}
void Grid::SetChecked(const unsigned int& INDEX, const bool& IS_CHECKED)
{
	tiles[INDEX].SetChecked(IS_CHECKED);
}
void Grid::SetExploded(const unsigned int& INDEX, const bool& IS_EXPLODED)
{
	tiles[INDEX].SetExploded(IS_EXPLODED);
}
void Grid::SetMouseOver(const unsigned int& INDEX, Mouse& mouse)
{
	tiles[INDEX].SetMouseOver(mouse);
}
void Grid::SetGameOver(const bool& IS_GAMEOVER)
{
	gameover = IS_GAMEOVER;
}
void Grid::SetBackground()
{
	const size_t MIN = 0;
	const size_t MAX = background_textures.size() - 1;
	const size_t INDEX = rnd::RandomInt(MIN, MAX);

	if (INDEX == 0u)
	{
		Color color_start = Bumble::RandomColor();
		Color color_end = Bumble::RandomColor();
		background_textures[0] = std::make_shared<Surface>(Bumble::CreateColorBlendTexture(GRID_RECT, color_start, color_end));
	}

	background = { GRID_RECT, background_textures[INDEX] };
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

void Grid::Reset()
{
	SetBackground();
	SetTileValues();
	gameover = false;

	for (auto& t : tiles)
	{
		t.SetTexture(TILE_TYPE::TILE,tile_textures[TILE::TILE_DARK]);
		t.SetTexture(TILE_TYPE::IMAGE,tile_textures[TILE::BLANK]);
	}
}

void Grid::Update()
{
	for (auto& t : tiles)
	{		
		if (!t.Revealed())
		{		
			if (gameover)
			{
				if (t.Flag() && !t.Mine())
				{
					t.SetTexture(1,tile_textures[TILE::TILE_DARK]);
					t.SetTexture(2,tile_textures[TILE::FLAG_WRONG]);
				}
				else if (t.Flag() && t.Mine())
				{
					t.SetTexture(1,tile_textures[TILE::TILE_DARK]);
					t.SetTexture(2,tile_textures[TILE::FLAG_CORRECT]);
				}
			}
			else
			{
				if (t.Flag())
				{
					t.SetTexture(1,tile_textures[TILE::TILE_DARK]);
					t.SetTexture(2,tile_textures[TILE::FLAG]);
				}
				else if (t.Checked())
				{
					t.SetTexture(1,tile_textures[TILE::TILE_LIGHT]);
				}
				else
				{
					t.SetTexture(1,tile_textures[TILE::TILE_DARK]);
				}
			}
		}
		else
		{			
			if (t.Exploded())
			{
				t.SetTexture(1,tile_textures[TILE::TILE_LIGHT]);
				t.SetTexture(2,tile_textures[TILE::EXPLODED]);
			}
			else
			{
				t.SetTexture(1,tile_textures[TILE::TILE_LIGHT]);
				t.SetTexture(2,tile_textures[t.Value()]);
			}
		}
	}
}
void Grid::Draw(Graphics& gfx)
{
	DrawBackground(gfx);
	DrawTiles(gfx);
}