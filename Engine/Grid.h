#pragma once

#include "Block.h"
#include "Tile.h"

#include <vector>
#include <array>

namespace TILE
{
	static constexpr size_t EMPTY		= 0;
	static constexpr size_t ONE			= 1;
	static constexpr size_t TWO			= 2;
	static constexpr size_t THREE		= 3;
	static constexpr size_t FOUR		= 4;
	static constexpr size_t FIVE		= 5;
	static constexpr size_t SIX			= 6;
	static constexpr size_t SEVEN		= 7;
	static constexpr size_t EIGHT		= 8;
	static constexpr size_t MINE		= 9;
	static constexpr size_t BLANK_TILE	= 10;
	static constexpr size_t FLAG		= 11;
};

class Grid
{
private:
	static constexpr unsigned int SCREEN_W = Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H = Graphics::ScreenHeight;
	static constexpr unsigned int MIN_COLS = 9u;
	static constexpr unsigned int MIN_ROWS = 9u;
		
	const unsigned int COLS;
	const unsigned int ROWS;
	const unsigned int SIZE;
	const unsigned int MINES;
	const unsigned int OFFSET;
	const unsigned int TILE_SIZE;


	const RectUI							GRID_POSITION;
	const Color								COLOR_START;
	const Color								COLOR_END;
	std::vector<std::shared_ptr<Surface>>	tile_textures;
	std::vector<std::shared_ptr<Surface>>	background_textures;
	std::vector<Tile>						tiles;
	Block									background;
	Block									mouseover;

public:
	Grid(const unsigned int& COLS,
		const unsigned int& ROWS,
		const unsigned int& MINES,
		const unsigned int& OFFSET);

public:
	void InitialiseTiles();
	void InitialiseBackground();

	void SetTileValues();
	void SetTileValue(const size_t& INDEX);
	void RevealTiles(const int& X, const int& Y);
	void RevealTile(const int& X, const int& Y);
	void CheckTiles(const int& X, const int& Y);
	void CheckTile(const int& X, const int& Y);

	unsigned int GetCols() const;
	unsigned int GetRows() const;
	unsigned int GetGridSize() const;
	unsigned int GetMines() const;
	unsigned int GetTileSize() const;
	RectUI GetGridRect() const;
	
	unsigned int Value(const unsigned int& INDEX) const;
	bool Flag(const unsigned int& INDEX) const;
	bool Revealed(const unsigned int& INDEX) const;
	bool Checked(const unsigned int& INDEX) const;
	bool Mine(const unsigned int& INDEX) const;
	bool MouseOver(const unsigned int& INDEX) const;

	void SetIsFlag(const unsigned int& INDEX, const bool& IS_FLAG);
	void SetIsRevealed(const unsigned int& INDEX, const bool& IS_REVEALED);
	void SetIsChecked(const unsigned int& INDEX, const bool& IS_CHECKED);
	void SetMouseOver(const unsigned int& INDEX, Mouse& mouse);
	void SetBackground();

	void DrawBackground(Graphics& gfx);
	void DrawTiles(Graphics& gfx);
	void DrawMouseOverTiles(Graphics& gfx);

public:
	void Update();
	void Draw(Graphics& gfx);
};

