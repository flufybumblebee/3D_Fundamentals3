/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.h																				  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "Graphics.h"

#include <array>
#include <vector>

class Game
{
private:
	MainWindow& wnd;
	Graphics gfx;

private:
	int chunk_size = 600 / 16;
	int line_thickness = chunk_size;
	int line_length = chunk_size * 14;
	int block_size = chunk_size * 4;

	int origin_x = (800 / 2) - (block_size / 2) - line_thickness - block_size;
	int origin_y = line_thickness;

	Color c = Colors::White;

	int line0X = (gfx.ScreenWidth / 2) - (block_size / 2) - line_thickness;
	int line0Y = (gfx.ScreenHeight / 2) - (block_size / 2) - line_thickness - block_size;
	int line1X = (gfx.ScreenWidth / 2) + (block_size / 2);
	int line1Y = (gfx.ScreenHeight / 2) - (block_size / 2) - line_thickness - block_size;
	int line2X = (gfx.ScreenWidth / 2) - (block_size / 2) - line_thickness - block_size;
	int line2Y = (gfx.ScreenHeight / 2) - (block_size / 2) - line_thickness;
	int line3X = (gfx.ScreenWidth / 2) - (block_size / 2) - line_thickness - block_size;
	int line3Y = (gfx.ScreenHeight / 2) + (block_size / 2);

	enum XOState
	{
		EMPTY = 0,
		X = 1,
		O = 2
	};

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
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();

private:
	void UpdateModel();
	void ComposeFrame();

private:
	void	ClearBlocks();
	void	Input();
	void	DrawGrid();
	void	SetState(int i, XOState state);
	void	SetState(int ix, int iy,XOState state);
	XOState	GetState(int i);
	XOState GetState(int ix, int iy);
	int		ConvertArrayAddress(int x, int y, int w);
	std::vector<Color> ConvertSurfaceToColorVector(const Surface& surface);
	Surface ConvertColorVectorToSurface(int width, int height, const std::vector<Color>& colors);

	void	DrawBackground();
	void	DrawX();
	void	DrawO();
	void	DrawCursor(int x, int y);
};