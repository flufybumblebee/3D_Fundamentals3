#pragma once

#include "Graphics.h"
#include "Keyboard.h"
#include "Colors.h"
#include "Surface.h"
#include "Vec3.h"

#include <array>
#include <vector>

class TickTackToe
{
public:
	Graphics& gfx;
	Keyboard& kbd;

private:
	int scrW = gfx.ScreenWidth;
	int scrH = gfx.ScreenHeight;

	int size = scrH / 16;
	
	/*------------------------------------------------*/

	enum XOState { EMPTY, X, O };

	static const int cols = 3;
	static const int rows = 3;

	std::array<XOState, (rows*cols)> blocks = { EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY };
		
	int current_x = 1;
	int current_y = 1;
	XOState current_player_state = EMPTY;

	bool keyIsPressed = false;

	/*------------------------------------------------*/

	const Vec2 tc0 = { 0.0f,0.0f };
	const Vec2 tc1 = { 1.0f,0.0f };
	const Vec2 tc2 = { 1.0f,1.0f };
	const Vec2 tc3 = { 0.0f,1.0f };

	std::vector<Surface> tex_backgrounds;
	std::vector<Surface> tex_grids;
	std::vector<Surface> tex_Xs;
	std::vector<Surface> tex_Os;
	
	Surface* tex_background;
	Surface* tex_grid;
	Surface* tex_X;
	Surface* tex_O;

	bool gameIsOver = false;

public:
	TickTackToe(Keyboard& kbd, Graphics& gfx);

public:
	void Setup();
	void Update();
	void Draw();

public:
	void	SetPlayers();
	void	ChangePlayer();
	void	Input();
	void	SetState(int i, XOState state);
	void	SetState(int ix, int iy, XOState state);
	XOState	GetState(int i);
	XOState GetState(int ix, int iy);
	bool	GameOver();

	/*----------------------------------------*/

	void	DrawBackground();
	void	DrawXOState();
	void	DrawCursor();
	void	DrawGrid();

	/*----------------------------------------*/

	std::vector<Color>	ConvertSurfaceToColorVector(const Surface& surface);
	Surface				ConvertColorVectorToSurface(int w, int h, const std::vector<Color>& colors);
};