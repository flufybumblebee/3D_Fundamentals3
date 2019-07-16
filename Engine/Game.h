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

#include "MainWindow.h"
#include "Graphics.h"
#include "Rect.h"
  
#include "Minesweeper.h"

class Game
{
private:
	MainWindow& wnd;
	Graphics gfx;
private:
	Minesweeper ms;

	static constexpr bool IS_COLOR_BLEND = false;
		
	const RectUI RECTANGLE = { 0u,600u,0u,800u };

	const unsigned int SIZE_X = RECTANGLE.GetWidth();
	const unsigned int SIZE_Y = RECTANGLE.GetHeight();

	const Vec2	LINE	= { static_cast<float>(RECTANGLE.bottom),static_cast<float>(RECTANGLE.right) };
	const float LENGTH	= LINE.Len();
	const Vec2	NORMAL	= LINE.GetNormalized();

	Color colorA;
	Color colorB;

	unsigned char start_red = 0u;
	unsigned char start_green = 0u;
	unsigned char start_blue = 0u;

	unsigned char end_red = 0u;
	unsigned char end_green = 0u;
	unsigned char end_blue = 0u;

	unsigned char delta_red = 0u;
	unsigned char delta_green = 0u;
	unsigned char delta_blue = 0u;

	float increment_red = 0.0f;
	float increment_green = 0.0f;
	float increment_blue = 0.0f;

	bool key_pressed = false;

	float angle = 0.0f;	

public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();

private:
	void UpdateModel();
	void ComposeFrame();
};