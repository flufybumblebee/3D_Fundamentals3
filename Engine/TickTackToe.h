#pragma once

#include "Graphics.h"
#include "Colors.h"
#include "Surface.h"

#include <array>
#include <vector>

class TickTackToe
{
private:
	Graphics& gfx;

	int scrW = 800;
	int scrH = 600;

	int chunk_size = scrH / 16;

	int line0X = (scrW / 2) - (chunk_size * 3);
	int line0Y = (scrH / 2) - (chunk_size * 7);
	int line1X = (scrW / 2) + (chunk_size * 2);
	int line1Y = (scrH / 2) - (chunk_size * 7);
	int line2X = (scrW / 2) - (chunk_size * 7);
	int line2Y = (scrH / 2) - (chunk_size * 3);
	int line3X = (scrW / 2) - (chunk_size * 7);
	int line3Y = (scrH / 2) + (chunk_size * 2);

	enum XOState { EMPTY, X, O };

	std::array<XOState, 9> blocks;

	int cols = 3;
	int rows = 3;
	int currentX = 1;
	int currentY = 1;

	bool keyIsPressed = false;

	/*------------------------------------------------*/

	Surface tex_background = Surface::FromFile(L"Textures\\Backgrounds\\Street0.bmp");
	Surface tex_background2 = Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg");
public:
	TickTackToe(Graphics& gfx)
		:
		gfx(gfx)
	{}

public:
	void Setup();
	void Update();
	void Draw();

public:
	void	Input();
	void	SetState(int i, XOState state);
	void	SetState(int ix, int iy, XOState state);
	XOState	GetState(int i);
	XOState GetState(int ix, int iy);

	/*----------------------------------------*/

	void	DrawBackground();
	void	DrawX();
	void	DrawO();
	void	DrawCursor(int x, int y);
	void	DrawGrid();

	/*----------------------------------------*/

	int					ConvertArrayAddress(int x, int y, int w);
	std::vector<Color>	ConvertSurfaceToColorVector(const Surface& surface);
	Surface				ConvertColorVectorToSurface(int w, int h, const std::vector<Color>& colors);
};