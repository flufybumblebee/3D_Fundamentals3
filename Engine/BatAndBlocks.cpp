#include "BatAndBlocks.h"

void BatAndBlocks::Initialise()
{
	for (int y = 0; y < BLOCKS_ROWS; y++)
	{
		for (int x = 0; x < BLOCKS_COLS; x++)
		{
			block_values[y][x] = BLOCKS::BASIC;
		}
	}
}

void BatAndBlocks::SetBat()
{
	
}
