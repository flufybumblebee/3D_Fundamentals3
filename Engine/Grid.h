#pragma once

#include "Block.h"
#include "Tile.h"

#include <vector>
#include <array>

namespace TILE
{
	static constexpr size_t BLANK			= 0;
	static constexpr size_t ONE				= 1;
	static constexpr size_t TWO				= 2;
	static constexpr size_t THREE			= 3;
	static constexpr size_t FOUR			= 4;
	static constexpr size_t FIVE			= 5;
	static constexpr size_t SIX				= 6;
	static constexpr size_t SEVEN			= 7;
	static constexpr size_t EIGHT			= 8;
	static constexpr size_t MINE			= 9;
	static constexpr size_t EXPLODED		= 10;
	static constexpr size_t TILE_LIGHT		= 11;
	static constexpr size_t TILE_DARK		= 12;
	static constexpr size_t FLAG			= 13;
	static constexpr size_t FLAG_WRONG		= 14;
	static constexpr size_t FLAG_CORRECT	= 15;
	static constexpr size_t MOUSEOVER		= 16;
};

namespace TILE_TYPE
{
	static constexpr size_t MOUSEOVER	= 0;
	static constexpr size_t TILE		= 1;
	static constexpr size_t IMAGE		= 2;
}

class Grid
{
private:
	static constexpr unsigned int SCREEN_W = Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H = Graphics::ScreenHeight;
	static constexpr unsigned int MIN_COLS = 9u;
	static constexpr unsigned int MIN_ROWS = 9u;

	const unsigned int	COLS;
	const unsigned int	ROWS;
	const size_t		SIZE;
	const unsigned int	MINES;
	const unsigned int	OFFSET;
	const unsigned int	TILE_SIZE;
	const RectUI		GRID_RECT;

	std::vector<std::shared_ptr<Surface>>	tile_textures;
	std::vector<std::shared_ptr<Surface>>	background_textures;
	std::vector<Tile>						tiles;

	Block									background;
	
	bool gameover = false;

public:
	Grid(std::vector<std::shared_ptr<Surface>> textures,
		const unsigned int& COLS,
		const unsigned int& ROWS,
		const unsigned int& MINES,
		const unsigned int& OFFSET);

private:
	static unsigned int SetTileSize(
		const unsigned int& COLS,
		const unsigned int& ROWS,
		const unsigned int& OFFSET);

	static RectUI SetGridRect(
		const unsigned int& TILE_SIZE,
		const unsigned int& COLS,
		const unsigned int& ROWS,
		const unsigned int& OFFSET);
	
private:
	//void InitialiseTextures();
	void InitialiseTiles();
	void InitialiseBackground();

public:
	void SetBackground();
	void SetTileValues();
	void RevealTiles(const int& X, const int& Y);
	void RevealTile(const int& X, const int& Y);
	bool CheckTiles(const int& X, const int& Y, const bool& IS_CHECKED);

	unsigned int	Cols() const; 
	unsigned int	Rows() const;
	size_t			GridSize() const;
	unsigned int	Mines() const;
	unsigned int	TileSize() const;
	RectUI			GridRect() const;

	unsigned int	Value(const unsigned int& INDEX) const;

	bool Flag(const unsigned int& INDEX) const;
	bool Revealed(const unsigned int& INDEX) const;
	bool Checked(const unsigned int& INDEX) const; 
	bool Mine(const unsigned int& INDEX) const;
	bool MouseOver(const unsigned int& INDEX) const;

	void SetFlag(const unsigned int& INDEX, const bool& IS_FLAG);
	void SetRevealed(const unsigned int& INDEX, const bool& IS_REVEALED);
	void SetChecked(const unsigned int& INDEX, const bool& IS_CHECKED);
	void SetExploded(const unsigned int& INDEX, const bool& IS_EXPLODED);
	void SetMouseOver(const unsigned int& INDEX, Mouse& mouse);

	void SetGameOver(const bool& IS_GAMEOVER);

	void DrawBackground(Graphics& gfx);
	void DrawTiles(Graphics& gfx);

public:
	void Reset();
	void Update();
	void Draw(Graphics& gfx);
};

