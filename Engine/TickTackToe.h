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
	std::array<int, 3> winner = { 0,0,0 };
		
	int current_x = 1;
	int current_y = 1;
	XOState current_player_state = EMPTY;

	bool leftIsPressed		= false;
	bool rightIsPressed		= false;
	bool upIsPressed		= false;
	bool downIsPressed		= false;
	bool spaceIsPressed		= false;
	bool returnIsPressed	= false;
	bool gameIsOver			= false;

	unsigned int nTurns = 0;
	/*------------------------------------------------*/

	const Vec2 tc0 = { 0.0f,0.0f };
	const Vec2 tc1 = { 1.0f,0.0f };
	const Vec2 tc2 = { 1.0f,1.0f };
	const Vec2 tc3 = { 0.0f,1.0f };

	std::vector<Surface> tex_backgrounds;
	std::vector<Surface> tex_grids;
	std::vector<Surface> tex_Xs;
	std::vector<Surface> tex_Os;
	std::vector<Surface> tex_Xs_red;
	std::vector<Surface> tex_Os_red;
	
	Surface* tex_background = nullptr;
	Surface* tex_grid		= nullptr;
	Surface* tex_X			= nullptr;
	Surface* tex_O			= nullptr;
	Surface* tex_X_red		= nullptr;
	Surface* tex_O_red		= nullptr;

public:
	TickTackToe(Keyboard& kbd, Graphics& gfx);

public:
	void	Setup();
	void	Update();
	void	Draw();

public:
	void	EndTurn();
	void	Input();
	XOState	GameOver();

	XOState	GetState(int i);
	XOState GetState(int ix, int iy);
	void	SetState(int i, XOState state);
	void	SetState(int ix, int iy, XOState state);

	void	DrawBackground();
	void	DrawXOState();
	void	DrawCursor();
	void	DrawGrid();

	/*----------------------------------------*/

	std::vector<Color>	ConvertSurfaceToColorVector(const Surface& surface);
	Surface				ConvertColorVectorToSurface(int w, int h, const std::vector<Color>& colors);
};