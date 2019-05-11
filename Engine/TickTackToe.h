#pragma once

#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Colors.h"
#include "Surface.h"
#include "Vec3.h"

#include <array>
#include <vector>

#include "Cell.h"

class TickTackToe
{
public:
	Graphics& gfx;
	Keyboard& kbd;
	Mouse& mouse;

private:
	const int scrW = gfx.ScreenWidth;
	const int scrH = gfx.ScreenHeight;

	const unsigned int size = scrH / 16;

	//	why 16?
	//
	//	the screen height is divided into 16 equal blocks:
	//	 2 blocks for border of the grid (one on each side)
	//	12 blocks for the 3 mouse_cells in each row or column (4 blocks for each cell)
	//	 2 blocks for the 2 grid lines in each row or column
	
	/* tex = texture */

	const Vec2 tex_coordinate_0 = { 0.0f,0.0f };
	const Vec2 tex_coordinate_1 = { 1.0f,0.0f };
	const Vec2 tex_coordinate_2 = { 1.0f,1.0f };
	const Vec2 tex_coordinate_3 = { 0.0f,1.0f };

	/*------------------------------------------------*/

	const float width = static_cast<float>(scrW / 2);
	const float height = static_cast<float>(scrH / 2);

	const Vec3 grid_tex_pos_0 = { width - (size * 8),height - (size * 8),0.0f };
	const Vec3 grid_tex_pos_1 = { width + (size * 8),height - (size * 8),0.0f };
	const Vec3 grid_tex_pos_2 = { width + (size * 8),height + (size * 8),0.0f };
	const Vec3 grid_tex_pos_3 = { width - (size * 8),height + (size * 8),0.0f };

	const TexVertex grid_tex_vertex_0 = { grid_tex_pos_0,tex_coordinate_0 };
	const TexVertex grid_tex_vertex_1 = { grid_tex_pos_1,tex_coordinate_1 };
	const TexVertex grid_tex_vertex_2 = { grid_tex_pos_2,tex_coordinate_2 };
	const TexVertex grid_tex_vertex_3 = { grid_tex_pos_3,tex_coordinate_3 };
	
	/*------------------------------------------------*/

	const Vec3 background_tex_pos_0 = { 0.0f,			0.0f,			0.0f };
	const Vec3 background_tex_pos_1 = { scrW - 1.0f,	0.0f,			0.0f };
	const Vec3 background_tex_pos_2 = { scrW - 1.0f,	scrH - 1.0f,	0.0f };
	const Vec3 background_tex_pos_3 = { 0.0f,			scrH - 1.0f,	0.0f };

	const TexVertex background_tex_vertex_0 = { background_tex_pos_0,tex_coordinate_0 };
	const TexVertex background_tex_vertex_1 = { background_tex_pos_1,tex_coordinate_1 };
	const TexVertex background_tex_vertex_2 = { background_tex_pos_2,tex_coordinate_2 };
	const TexVertex background_tex_vertex_3 = { background_tex_pos_3,tex_coordinate_3 };
	
	/*------------------------------------------------*/

	enum XOState { EMPTY, X, O };

	static const unsigned int cols = 3;
	static const unsigned int rows = 3;

	std::array<std::array<RectI, cols>, rows> mouse_cells;
	std::array<XOState, (rows*cols)> blocks = { EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY };
	std::array<int, 3> winner = { 0,0,0 };
		
	int		current_x = 1;
	int		current_y = 1;
	XOState current_player_state = EMPTY;

	unsigned int nTurns = 0;

	bool leftIsPressed		= false;
	bool rightIsPressed		= false;
	bool upIsPressed		= false;
	bool downIsPressed		= false;
	bool spaceIsPressed		= false;
	bool returnIsPressed	= false;

	bool mouseLeftIsPressed = false;

	bool gameIsOver			= false;

	/*------------------------------------------------*/

	const Vec2 tc0 = { 0.0f,0.0f };
	const Vec2 tc1 = { 1.0f,0.0f };
	const Vec2 tc2 = { 1.0f,1.0f };
	const Vec2 tc3 = { 0.0f,1.0f };

	std::vector<Surface> background_textures;
	std::vector<Surface> grid_textures;
	std::vector<Surface> x_textures_A;
	std::vector<Surface> o_textures_A;
	std::vector<Surface> x_textures_B;
	std::vector<Surface> o_textures_B;
	
	Surface* background_tex = nullptr;
	Surface* grid_tex		= nullptr;
	Surface* x_tex_black	= nullptr;
	Surface* o_tex_black	= nullptr;
	Surface* x_tex_red		= nullptr;
	Surface* o_tex_red		= nullptr;

	//std::array<std::array<Cell, cols>, rows> cells;
	std::vector<Cell> cells;

public:
	TickTackToe(Keyboard& kbd, Mouse& mouse, Graphics& gfx);

public:
	void	Setup();
	void	Update();
	void	Draw();

public:
	void	EndTurn();
	void	InputA();
	void	InputB();
	void	SetCell();
	XOState	GameOver();

	XOState	GetState(int i);
	XOState GetState(int ix, int iy);
	void	SetState(int i, XOState state);
	void	SetState(int ix, int iy, XOState state);

	void	DrawBackground();
	void	DrawXOState();
	void	DrawCursor();
	void	DrawGrid();
};