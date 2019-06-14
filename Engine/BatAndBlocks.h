#pragma once

#include "Block.h"
#include "Vec4.h"

#include <vector>
#include <array>

enum class BLOCKS
{
	BASIC
};

class BatAndBlocks
{
private:
	static constexpr unsigned int SCREEN_W = Graphics::ScreenWidth;
	static constexpr unsigned int SCREEN_H = Graphics::ScreenHeight;
	static constexpr unsigned int BLOCKS_COLS = 10u;
	static constexpr unsigned int BLOCKS_ROWS = 10u;
	static constexpr unsigned int BLOCK_W = 10u;
	static constexpr unsigned int BLOCK_H = 10u;
	
	static constexpr unsigned int BAT_W = 50u; 
	static constexpr unsigned int BAT_H = 20u;
	static constexpr unsigned int BAT_X = SCREEN_W / 2u;
	static constexpr unsigned int BAT_Y = SCREEN_H - ((BAT_H / 2u) * 3u);

	Vec2 bat_positon{ BAT_W,BAT_H };
	Block bat_block;
	//std::vector<Block> blocks;

	std::array<std::array<BLOCKS, BLOCKS_COLS>, BLOCKS_ROWS> block_values;
	std::array<std::array<Block, BLOCKS_COLS>, BLOCKS_ROWS> blocks;
public:
	BatAndBlocks() = default;
	~BatAndBlocks() = default;

public:
	void Update();
	void Draw();

public:
	void Initialise();
	void SetBat();
	void SetBlocks();
	void DeleteBlock();

	void Input();
};

