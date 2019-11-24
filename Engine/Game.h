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
#include "Random.h"
#include "Rays.h"

#include <array>
#include <vector>

//template <typename T>
//T RandomFloat(T min, T max)
//{
//	std::mt19937 rng;
//	rng.seed(std::random_device()());
//	std::uniform_real_distribution<T> dist(min, max);
//
//	return dist(rng);
//}

class Game
{
private:
	MainWindow& wnd;
	Graphics gfx;
private:
	static constexpr unsigned int COLS = Graphics::ScreenWidth;
	static constexpr unsigned int ROWS = Graphics::ScreenHeight;

	std::vector<Color> colors;

public:
	Game(class MainWindow& wnd);
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	void Go();

private:
	void UpdateModel();
	void ComposeFrame();
	void SetWalls();

private:
	const float SCREEN_W = Graphics::ScreenWidth;
	const float SCREEN_H = Graphics::ScreenHeight;

	const float X_MIN = 100.0f;
	const float X_MAX = SCREEN_W - 100.0f;
	const float Y_MIN = 100.0f;
	const float Y_MAX = SCREEN_H - 100.0f;

	const float X0 = 100.0f;
	const float Y0 = 100.0f;
	const float X1 = SCREEN_W - 100.0f;
	const float Y1 = SCREEN_H - 100.0f;

	const size_t NLINES = 5u;

	const Vec2 position = { SCREEN_W / 2, SCREEN_H / 2 };
	const size_t nrays = 1000u;
	Rays rays = { position, nrays };
	std::vector<LineSegment> walls;
	bool key_is_pressed = false;
};