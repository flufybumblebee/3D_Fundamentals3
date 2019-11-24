/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Game.cpp																			  *
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
#include "MainWindow.h"
#include "Game.h"

#include "Bumble.h"
#include "Polyshape.h"
#include "LineSegment.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)
{
	/*const float INCREMENT_X = 1.0f / (COLS-1.0f); 
	const float INCREMENT_Y = 1.0f / (ROWS-1.0f) ;
	
	auto myLambda = [](const Ray& r)
	{
		Vec3 unit_direction = r.GetDirection().GetNormalized();
		float t = 0.5f * (unit_direction.y + 1.0f);
		return Vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vec3(0.5f, 0.7f, 1.0f) * t;
	};

	for(int y = 0; y < ROWS; y++)
	{
		for(int x = 0; x < COLS; x++)
		{
			const float X = x * INCREMENT_X;
			const float Y = y * INCREMENT_Y;

			const Vec3 color{
				((1.0f - X) + (1.0f - Y)) / 2.0f,
				((0.0f + X) + (0.0f + Y)) / 2.0f,
				((0.0f + X) + (1.0f - Y)) / 2.0f, };
			 
			colors.emplace_back(static_cast<unsigned char>(color.x * 255),
								static_cast<unsigned char>(color.y * 255),
								static_cast<unsigned char>(color.z * 255));
		}
	}*/
	
	SetWalls();
}

void Game::Go()
{	
	gfx.BeginFrame();
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	/*const float SPEED = 1.0f;

	if (wnd.kbd.KeyIsPressed(VK_LEFT))
	{
		light.x -= SPEED;
	}
	
	if (wnd.kbd.KeyIsPressed(VK_RIGHT))
	{
		light.x += SPEED;
	}
	
	if (wnd.kbd.KeyIsPressed(VK_UP))
	{
		light.y -= SPEED;
	}
	
	if (wnd.kbd.KeyIsPressed(VK_DOWN))
	{
		light.y += SPEED;
	}*/	
	
	if (!key_is_pressed)
	{
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			SetWalls();

			key_is_pressed = true;
		}
	}
	else
	{
		if (!wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			key_is_pressed = false;
		}
	}

	if (wnd.mouse.IsInWindow())
	{
		rays.SetPosition(Vec2(wnd.mouse.GetPos()));
	}
	else
	{
		rays.SetPosition(position);
	}
	rays.Cast(walls);	
}

void Game::ComposeFrame()
{
	/*size_t i = 0;
	for(int y = 0; y < ROWS; y++)
	{
		for(int x = 0; x < COLS; x++)
		{
			i = static_cast<size_t>(y) * COLS + x;
			gfx.PutPixel(x, y, colors[i]);
		}
	}*/

	for (auto& w : walls)
	{
		w.Draw(gfx);
	}

	rays.Draw(gfx);
}

void Game::SetWalls()
{
	walls.clear();

	walls.emplace_back(X0, Y0, X1, Y0);
	walls.emplace_back(X1, Y0, X1, Y1);
	walls.emplace_back(X1, Y1, X0, Y1);
	walls.emplace_back(X0, Y1, X0, Y0);

	for (int i = 0; i < NLINES; i++)
	{
		const LineSegment wall = {
			rnd::RandomFloat(X_MIN, X_MAX),
			rnd::RandomFloat(Y_MIN, Y_MAX),
			rnd::RandomFloat(X_MIN, X_MAX),
			rnd::RandomFloat(Y_MIN, Y_MAX) };
		walls.emplace_back(wall);
	}
}
