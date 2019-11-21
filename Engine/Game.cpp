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
	const float SPEED = 1.0f;

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
	}
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

	Vec2 v0 = { 100.0f, 100.0f };
	Vec2 v1 = { 500.0f, 100.0f };
	Vec2 v2 = { 500.0f, 500.0f };
	Vec2 v3 = { 100.0f, 500.0f };

	gfx.DrawLine(v0.x, v0.y, v1.x, v1.y, Colors::White);
	gfx.DrawLine(v1.x, v1.y, v2.x, v2.y, Colors::White);
	gfx.DrawLine(v2.x, v2.y, v3.x, v3.y, Colors::White);
	gfx.DrawLine(v3.x, v3.y, v0.x, v0.y, Colors::White);

	/*--------------------------------------------------------*/

	Vec2 p0 = { 150.0f, 150.0f };
	Vec2 p1 = { 200.0f, 150.0f };
	Vec2 p2 = { 200.0f, 200.0f };
	Vec2 p3 = { 150.0f, 200.0f };

	gfx.DrawLine(p0.x, p0.y, p1.x, p1.y, Colors::Green);
	gfx.DrawLine(p1.x, p1.y, p2.x, p2.y, Colors::Green);
	gfx.DrawLine(p2.x, p2.y, p3.x, p3.y, Colors::Green);
	gfx.DrawLine(p3.x, p3.y, p0.x, p0.y, Colors::Green);

	/*--------------------------------------------------------*/

	gfx.DrawLine(light.x, light.y, v0.x, v0.y, Colors::Red);
	gfx.DrawLine(light.x, light.y, v1.x, v1.y, Colors::Red);
	gfx.DrawLine(light.x, light.y, v2.x, v2.y, Colors::Red);
	gfx.DrawLine(light.x, light.y, v3.x, v3.y, Colors::Red);
}