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
#include "Block.h"
#include "Mat.h"
#include "Bumble.h"

#include <vector>
#include <chrono>
  
#include "Minesweeper.h"

class Game
{
private:
	MainWindow& wnd;
	Graphics gfx;
private:
	Minesweeper ms;

	std::vector<std::shared_ptr<Surface>> textures;
	std::vector<std::shared_ptr<Surface>> temp_tex;
	RectUI rect;
	Block block;

	int i = 0;
	unsigned int number = 0;
	unsigned int indexA = 0;
	unsigned int indexB = 1;

	bool key_pressed = false;

	std::chrono::high_resolution_clock::time_point t1;
	unsigned int time = 0;
	bool timer_started = false;

	unsigned int width = 0u;
	unsigned int height = 0u;

public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();

private:
	void UpdateModel();
	void ComposeFrame();
};