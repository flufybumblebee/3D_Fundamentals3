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
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_0_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_1_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_2_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_3_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_4_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_5_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_6_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_7_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_8_white.png")));
	textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Digits\\digit_9_white.png")));

	const unsigned int OFFSET   = 10u;
	
	const unsigned int DIGIT_W = OFFSET * 4u;
	const unsigned int DIGIT_H = OFFSET * 4u;

	const unsigned int TOP		= OFFSET;
	const unsigned int BOTTOM	= TOP + DIGIT_H - 1u;
	const unsigned int LEFT		= Graphics::ScreenWidth / 2u - DIGIT_W / 2u;
	const unsigned int RIGHT	= LEFT + DIGIT_W - 1u;

	rect = { TOP, BOTTOM, LEFT, RIGHT };

	const unsigned int WIDTH = textures[0]->GetWidth();
	const unsigned int HEIGHT = textures[0]->GetHeight();

	width = WIDTH;
	height = HEIGHT;
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
	//ms.Update(wnd.mouse);

	/*if (!key_pressed)
	{
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{		

			key_pressed = true;
		}
	}
	else
	{
		if (!wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			key_pressed = false;
		}
	}*/

	const unsigned int VAL = 25u;
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		switch (e.GetType())
		{
		case Mouse::Event::Type::WheelUp:
			i+=VAL;
			if (i > height - 2u)
			{
				i = 0;

				if (indexA < 9u)
				{
					indexA++;
				}
				else
				{
					indexA = 0u;
				}

				if (indexB < 9u)
				{
					indexB++;
				}
				else
				{
					indexB = 0u;
				}
			}
			break;
		case Mouse::Event::Type::WheelDown:
			i-=VAL;
			if (i < 0)
			{
				i = height - 2u;

				if (indexA > 0u)
				{
					indexA--;
				}
				else
				{
					indexA = 9u;
				}

				if (indexB > 0u)
				{
					indexB--;
				}
				else
				{
					indexB = 9u;
				}
			}
			break;
		}
	}
}

void Game::ComposeFrame()
{
	//ms.Draw(gfx);

	const unsigned int WIDTH = textures[0]->GetWidth();
	const unsigned int HEIGHT = textures[0]->GetHeight();

	Surface surface(WIDTH, HEIGHT);
	
	for (unsigned int y = HEIGHT - 1u - i, yy = 0; y < HEIGHT && yy < i; y++, yy++)
	{
		for (unsigned int x = 0u; x < WIDTH; x++)
		{
			Color c = textures[indexB]->GetPixel(x,y);
												
			surface.PutPixel(x, yy, c);
		}
	}		
		
	for (unsigned int y = 0, yy = i; y < HEIGHT - i && yy < HEIGHT; y++, yy++)
	{
		for (unsigned int x = 0u; x < WIDTH; x++)
		{
			Color c = textures[indexA]->GetPixel(x, y);

			surface.PutPixel(x, yy, c);
		}
	}

	const Vec2 TC0 = { 0.0f,0.0f };
	const Vec2 TC1 = { 1.0f,0.0f };
	const Vec2 TC2 = { 1.0f,1.0f }; 
	const Vec2 TC3 = { 0.0f,1.0f };
	
	const float SIZE = 300.0f;

	const Vec3 P0 = {  0.0f, 0.0f,0.0f };
	const Vec3 P1 = { SIZE, 0.0f,0.0f };
	const Vec3 P2 = { SIZE,SIZE,0.0f };
	const Vec3 P3 = {  0.0f,SIZE,0.0f };

	TexVertex tv0 = { P0,TC0 };
	TexVertex tv1 = { P1,TC1 };
	TexVertex tv2 = { P2,TC2 };
	TexVertex tv3 = { P3,TC3 };

	gfx.DrawTriangleTex(tv0, tv1, tv2, surface);
	gfx.DrawTriangleTex(tv0, tv2, tv3, surface);

	/*
	NOTES:

	also would be worth looking into how to do this using
	two seperate textures and cropping them to the rectangle 
	because it may be less expensive than this method that
	creates a surface then copies lines of pixels from the
	two seperate textures into it.

	could also try using a single large texture with all the 
	numbers combined in it and then cropping that. though i 
	still prefer using seperate textures for each digit.

	need to look into changing motion speed and type
	as well as more complex motions such as spring-like
	changes.

	also could look into how textures would look on different
	sized and shaped objects that rotate such as on cylinders
	and square or hex prisms.

	could also look into different transitions from last to first
	such as speedily scrolling back through all the past numbers
	back to the start.

	*/
}