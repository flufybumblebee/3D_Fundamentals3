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
	Button(const Block& a, const Block& b, const Block& c, const Block& d);
	Button(const Button& copy);
	Button(Button&& move) noexcept;
	Button& operator = (const Button& rhs);
	~Button() = default;
public:
	void Set(const unsigned int& MOUSE_X, const unsigned int& MOUSE_Y, const bool& LEFT_IS_PRESSED);
	bool GetMouseOver() const;
	bool GetMousePress() const;
	RectUI GetPosition() const;
	void Draw(Graphics& gfx);
};