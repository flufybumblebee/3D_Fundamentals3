#pragma once

#include "Graphics.h"
#include "Keyboard.h"
#include "Colors.h"
#include "Surface.h"

#include <array>
#include <vector>

class TickTackToe
{
public:
	Graphics& gfx;
	Keyboard& kbd;

private:
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

	std::array<XOState, 9> blocks = { EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY };

	int cols = 3;
	int rows = 3;
	int current_x = 1;
	int current_y = 1;
	int current_player = 0;
	XOState current_player_state = EMPTY;

	bool keyIsPressed = false;

	/*------------------------------------------------*/

	Surface tex_background = Surface::FromFile(L"Textures\\Backgrounds\\Street0.bmp");
	Surface tex_background2 = Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg");
	Surface tex_X = Surface::FromFile(L"Textures\\TickTackToe\\X.png");
	Surface tex_O = Surface::FromFile(L"Textures\\TickTackToe\\O.png");

public:
	TickTackToe(Keyboard& kbd, Graphics& gfx);

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
	void	SetPlayers();

	/*----------------------------------------*/

	void	DrawBackground();
	void	DrawX();
	void	DrawO();
	void	DrawCursor(int x, int y);
	void	DrawGrid();

	/*----------------------------------------*/

	std::vector<Color>	ConvertSurfaceToColorVector(const Surface& surface);
	Surface				ConvertColorVectorToSurface(int w, int h, const std::vector<Color>& colors);
};