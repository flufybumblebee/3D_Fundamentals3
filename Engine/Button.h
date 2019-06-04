#pragma once

#include "Block.h"
#include "Surface.h"
#include "Graphics.h"
#include "Rect.h"
#include "Mouse.h"

#include <vector>

class Button
{
private:
	bool mousepress = false;
	bool mouseover = false;
	RectUI position;
	Block block_a;
	Block block_b;
	Block block_c; 
	Block block_d;
public:
	Button() = default;
	Button(const Block& block_A, const Block& block_B, const Block& block_C, const Block& block_D )
		:
		block_a(block_A),
		block_b(block_B),
		block_c(block_C),
		block_d(block_D)
	{
		position = block_a.GetPosition();
	}
	Button(const Button& copy)
		:
		block_a(copy.block_a),
		block_b(copy.block_b),
		block_c(copy.block_c),
		block_d(copy.block_d)
	{
		position = copy.position;
	}
	/*Button(Button&& move) noexcept
		:
		blocks(std::move(move.blocks))
	{}
	Button& operator = (const Button& rhs)
	{
		blocks = rhs.blocks;
	
		return *this;
	}*/
	~Button() = default;
public:
	void Set( Mouse& mouse )
	{
		if (mouse.IsInWindow())
		{
			const unsigned int MOUSE_X = mouse.GetPosX();
			const unsigned int MOUSE_Y = mouse.GetPosY();
			const bool LEFT_IS_PRESSED = mouse.LeftIsPressed();

			const bool FITS_TOP		= MOUSE_Y >=	position.top;
			const bool FITS_BOTTOM	= MOUSE_Y <		position.bottom;
			const bool FITS_LEFT	= MOUSE_X >=	position.left;
			const bool FITS_RIGHT	= MOUSE_X <		position.right;

			if (FITS_TOP && FITS_BOTTOM && FITS_LEFT && FITS_RIGHT)
			{
				mouseover = true;

				if (mouseover && LEFT_IS_PRESSED)
				{
					mousepress = true;
				}
				else
				{
					mousepress = false;
				}
			}
			else
			{
				mouseover = false;
			}			
		}
	}
	bool GetMouseOver() const
	{
		return mouseover;
	}
	bool GetMousePress() const
	{
		return mousepress;
	}
	void Draw(Graphics& gfx)
	{
		if (!mousepress)
		{
			if (!mouseover)
			{
				block_a.Draw(gfx);
			}
			else
			{
				block_b.Draw(gfx);
			}
		}
		else
		{
			if (!mouseover)
			{
				block_c.Draw(gfx);
			}
			else
			{
				block_d.Draw(gfx);
			}
		}
	}
};