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


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	
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
	
}

void Game::ComposeFrame()
{	
	DrawGrid();
}

void Game::ClearBlocks()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i] = EMPTY;
	}
}

void Game::DrawGrid()
{
	gfx.DrawRect(line0X, line0Y, lineW, lineH, Colors::Gray);
	gfx.DrawRect(line1X, line1Y, lineW, lineH, Colors::Green);
	gfx.DrawRect(line2X, line2Y, lineH, lineW, Colors::White);
	gfx.DrawRect(line3X, line3Y, lineH, lineW, Colors::Yellow);
}

