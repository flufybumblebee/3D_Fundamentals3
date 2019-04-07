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
	for (int i = 0; i < 9; i++)
	{
		SetState(i, EMPTY);
	}
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
	Input();
}

void Game::ComposeFrame()
{
	DrawBackground();
	DrawCursor(currentX, currentY);
	DrawGrid();
}

/*-----------------------------------------------------*/

void Game::ClearBlocks()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i] = EMPTY;
	}
}

void Game::Input()
{
	if (!keyIsPressed)
	{
		if (wnd.kbd.KeyIsPressed(VK_LEFT) && currentX > 0)
		{
			currentX--;
			keyIsPressed = true;
		}
		if (wnd.kbd.KeyIsPressed(VK_RIGHT) && currentX < 2)
		{
			currentX++;
			keyIsPressed = true;
		}
		if (wnd.kbd.KeyIsPressed(VK_UP) && currentY > 0)
		{
			currentY--;
			keyIsPressed = true;
		}
		if (wnd.kbd.KeyIsPressed(VK_DOWN) && currentY < 2)
		{
			currentY++;
			keyIsPressed = true;
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			keyIsPressed = true;
		}
	}
	else
	{
		if (!wnd.kbd.KeyIsPressed(VK_LEFT) &&
			!wnd.kbd.KeyIsPressed(VK_RIGHT) &&
			!wnd.kbd.KeyIsPressed(VK_UP) &&
			!wnd.kbd.KeyIsPressed(VK_DOWN) &&
			!wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			keyIsPressed = false;
		}
	}
}


void Game::SetState(int i, XOState state)
{
	blocks[i] = state;
}

void Game::SetState(int ix, int iy, XOState state)
{
	SetState(ConvertArrayAddress(ix, iy,cols), state);
}

Game::XOState Game::GetState(int i)
{
	return blocks[i];
}

Game::XOState Game::GetState(int ix, int iy)
{
	return blocks[ConvertArrayAddress(ix,iy,cols)];
}

int Game::ConvertArrayAddress(int ix, int iy, int w)
{
	return iy * w + ix;
}

/*-----------------------------------------------------*/

void Game::DrawBackground()
{
	float width = gfx.ScreenWidth - 1.0f;
	float height = gfx.ScreenHeight - 1.0f;

	Vec3 pos0 = { 0.0f,0.0f,0.0f };
	Vec3 pos1 = { width,0.0f,0.0f };
	Vec3 pos2 = { width,height,0.0f };
	Vec3 pos3 = { 0.0f,height,0.0f };
	Vec2 tc0 = { 0.0f,0.0f };
	Vec2 tc1 = { 1.0f,0.0f };
	Vec2 tc2 = { 1.0f,1.0f };
	Vec2 tc3 = { 0.0f,1.0f };
	TexVertex tv0 = { pos0,tc0 };
	TexVertex tv1 = { pos1,tc1 };
	TexVertex tv2 = { pos2,tc2 };
	TexVertex tv3 = { pos3,tc3 };

	gfx.DrawTriangleTex(tv0, tv1, tv2, tex_background2);
	gfx.DrawTriangleTex(tv0, tv2, tv3, tex_background2);
}

void Game::DrawCursor(int x, int y)
{
	int w = gfx.ScreenWidth;
	int h = gfx.ScreenHeight;
	int size = h / 16;

	int xOut = ((w/2) - (size*7)) + (((size*4) * x) + (x * (size)));
	int yOut = ((h/2) - (size*7)) + (((size*4) * y) + (y * (size)));

	gfx.DrawRect(xOut, yOut, block_size, block_size, Colors::Red);
}

//void Game::DrawCursor(int x, int y)
//{	
//	//Surface copy = gfx.CopySysBuffer();
//	//int w = copy.GetWidth();
//	//int h = copy.GetHeight();
//
//	//assert(w >= h);
//	
//	//int size = h / 16;
//	int w = gfx.ScreenWidth;
//	int h = gfx.ScreenHeight;
//	int size = h / 16;
//	int xStart	= ((w / 2) - (size * 7)) + (	(size * 5) * x				);
//	int xEnd	= ((w / 2) - (size * 7)) + ((	(size * 5) * x) + (size * 4));
//	int yStart	= ((h / 2) - (size * 7)) + (	(size * 5) * y				);
//	int yEnd	= ((h / 2) - (size * 7)) + ((	(size * 5) * y) + (size * 4));
//		
//	Surface input(size * 4, size * 4);
//
//	for (int iy = yStart; iy < yEnd; iy++)
//	{
//		for (int ix = xStart; ix < xEnd; ix++)
//		{
//			//Color c = copy.GetPixel(ix, iy);
//			//input.PutPixel(ix - xStart, iy - yStart, c);
//			input.PutPixel(ix, iy, Colors::Black);
//		}
//	}
//	
//	Surface output( gfx.Blur(gfx.Blur(gfx.Blur(input))) );
//	
//	//assert(output.GetWidth() == input.GetWidth() && output.GetHeight() == input.GetHeight());
//
//	
//	int wi = gfx.ScreenWidth;
//	int hi = gfx.ScreenHeight;
//	int si = hi / 16;
//
//	float left		= static_cast<float>(((wi / 2) - (si * 7)) + ((si * 5) * x));
//	float top		= static_cast<float>(((hi / 2) - (si * 7)) + ((si * 5) * y));
//	float right		= static_cast<float>(((wi / 2) - (si * 7)) + (((si * 5) * x) + (si * 4)));
//	float bottom	= static_cast<float>(((hi / 2) - (si * 7)) + (((si * 5) * y) + (si * 4)));
//
//	Vec3 pos0 = { left,top,0.0f };
//	Vec3 pos1 = { right,top,0.0f };
//	Vec3 pos2 = { right,bottom,0.0f };
//	Vec3 pos3 = { left,bottom,0.0f };
//
//	Vec2 tc0 = { 0.0f,0.0f }; // top left
//	Vec2 tc1 = { 1.0f,0.0f }; // top right
//	Vec2 tc2 = { 1.0f,1.0f }; // bottom right
//	Vec2 tc3 = { 0.0f,1.0f }; // bottom left
//
//	TexVertex tv0 = { pos0,tc0 };
//	TexVertex tv1 = { pos1,tc1 };
//	TexVertex tv2 = { pos2,tc2 };
//	TexVertex tv3 = { pos3,tc3 };
//
//	gfx.DrawTriangleTex(tv0, tv1, tv2, output);
//	gfx.DrawTriangleTex(tv0, tv2, tv3, output);
//	
//	/*int w = gfx.ScreenWidth;
//	int h = gfx.ScreenHeight;
//	int size = h / 16;
//
//	int xOut = ((w/2) - (size*7)) + (((size*4) * x) + (x * (size)));
//	int yOut = ((h/2) - (size*7)) + (((size*4) * y) + (y * (size)));
//
//	gfx.DrawRect(xOut, yOut, block_size, block_size, Colors::Red);*/
//}

void Game::DrawGrid()
{
	gfx.DrawRect(line0X, line0Y, line_thickness, line_length, Colors::Gray);
	gfx.DrawRect(line1X, line1Y, line_thickness, line_length, Colors::Green);
	gfx.DrawRect(line2X, line2Y, line_length, line_thickness, Colors::White);
	gfx.DrawRect(line3X, line3Y, line_length, line_thickness, Colors::Yellow);
}

/*--------------------------------------------------------------*/

std::vector<Color> Game::ConvertSurfaceToColorVector(const Surface& surface)
{
	std::vector<Color> output;

	for (size_t iy = 0; iy < surface.GetHeight(); iy++)
	{
		for (size_t ix = 0; ix < surface.GetWidth(); ix++)
		{
			unsigned int x = static_cast<unsigned int>(ix);
			unsigned int y = static_cast<unsigned int>(iy);
			output.push_back(surface.GetPixel(x, y));
		}
	}

	return output;
}

Surface Game::ConvertColorVectorToSurface(int width, int height, const std::vector<Color>& colors)
{
	Surface surface(width, height);

	for (int iy = 0; iy < height; iy++)
	{
		for (int ix = 0; ix < width; ix++)
		{
			surface.PutPixel(ix, iy, colors[iy * width + ix]);
		}
	}
	
	return surface;
}

