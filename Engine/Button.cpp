#include "Button.h"

Button::Button(const Block& a, const Block& b, const Block& c, const Block& d)
	:
	block_a(a),
	block_b(b),
	block_c(c),
	block_d(d)
{
	rect = block_a.Rect();
}
Button::Button(const Button& copy)
	:
	block_a(copy.block_a),
	block_b(copy.block_b),
	block_c(copy.block_c),
	block_d(copy.block_d),
	rect(copy.rect)
{}
Button::Button(Button&& move) noexcept
	:
	block_a(std::move(move.block_a)),
	block_b(std::move(move.block_b)),
	block_c(std::move(move.block_c)),
	block_d(std::move(move.block_d)),
	rect(std::move(move.rect))
{}

Button& Button::operator = (const Button& rhs)
{
	block_a = rhs.block_a;
	block_b = rhs.block_b;
	block_c = rhs.block_c;
	block_d = rhs.block_d;

	rect = rhs.rect;

	return *this;
}

void Button::Set(const unsigned int& MOUSE_X, const unsigned int& MOUSE_Y, const bool& LEFT_IS_PRESSED)
{
	const bool FITS_TOP		= MOUSE_Y >=	rect.top;
	const bool FITS_BOTTOM	= MOUSE_Y <		rect.bottom;
	const bool FITS_LEFT	= MOUSE_X >=	rect.left;
	const bool FITS_RIGHT	= MOUSE_X <		rect.right;

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
bool Button::MouseOver() const
{
	return mouseover;
}
bool Button::GetMousePress() const
{
	return mousepress;
}
RectUI Button::Position() const
{
	return rect;
}
void Button::Draw(Graphics& gfx)
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