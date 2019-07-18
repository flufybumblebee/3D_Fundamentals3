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

#include "Block.h"
#include "Mat.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd)/*,
	texture(RECTANGLE.GetWidth(),RECTANGLE.GetHeight())*/
{
	/*Color color_start = Color(rnd::RandomInt(0, 255), rnd::RandomInt(0, 255), rnd::RandomInt(0, 255));
	Color color_end = Color(rnd::RandomInt(0, 255), rnd::RandomInt(0, 255), rnd::RandomInt(0, 255));
	texture = CreateColorBlendTexture(RECTANGLE,color_start, color_end);*/
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
	ms.Update(wnd.mouse);

	/*if (!key_pressed)
	{
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			Color color_start = Color(rnd::RandomInt(0, 255), rnd::RandomInt(0, 255), rnd::RandomInt(0, 255));
			Color color_end = Color(rnd::RandomInt(0, 255), rnd::RandomInt(0, 255), rnd::RandomInt(0, 255));

			texture = CreateColorBlendTexture(RECTANGLE, color_start, color_end);

			key_pressed = true;
		}
	}
	else
	{
		if (!wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			key_pressed = false;
		}
	}	*/
}

void Game::ComposeFrame()
{
	ms.Draw(gfx);
	
	/*std::vector<Color> colors;

	colors.emplace_back(Colors::Red);
	colors.emplace_back(Colors::Green);
	colors.emplace_back(Colors::Blue);
	colors.emplace_back(Colors::Cyan);
	colors.emplace_back(Colors::Yellow);
	colors.emplace_back(Colors::Magenta);
	
	const float TRANS_X = Graphics::ScreenWidth / 2.0f;
	const float TRANS_Y = Graphics::ScreenHeight / 2.0f;

	const Vec4 TRANS = { TRANS_X,TRANS_Y, 0.0f, 1.0f };

	const Mat4 TRANSLATE = Mat4::Translation(TRANS);
	const Mat4 ROTATE = Mat4::RotationZ(angle);
	
	// RECTANGLE

	const float X0 = 75.0f;
	const float X1 = 200.0f;
	const float Y0 = 150.0f;
	const float Y1 = 100.0f;

	const std::vector<Vec4> RECTANGLE{
		{ -X0,-Y0, 0.0f, 1.0f },
		{  X1,-Y0, 0.0f, 1.0f },
		{  X1, Y1, 0.0f, 1.0f },
		{ -X0, Y1, 0.0f, 1.0f } };

	std::vector<Vec2> rectangle;

	for (int i = 0; i < RECTANGLE.size(); i++)
	{
		rectangle.emplace_back(RECTANGLE[i] * ROTATE * TRANSLATE);
	}

	{
		if (true)
		{
			gfx.DrawPolygon2D(false, rectangle, Colors::Green);
		}

		if (false)
		{
			const Vec4 ORIGIN = { 0.0f,0.0f,0.0f,1.0f };

			for (int i = 0; i < rectangle.size(); i++)
			{
				gfx.DrawLine({ ORIGIN * TRANSLATE }, { rectangle[i].x,rectangle[i].y }, colors[i]);
			}
		}

		if (false)
		{
			for (int i = 0; i < rectangle.size(); i++)
			{
				const RectF rect = {
					rectangle[i].y - 3,
					rectangle[i].y + 3,
					rectangle[i].x - 3,
					rectangle[i].x + 3 };
				gfx.DrawRect(true, rect, colors[i]);
			}
		}

		if (false)
		{
			int height = texture.GetHeight();
			int width = texture.GetWidth();

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					gfx.PutPixel(x, y, texture.GetPixel(x, y));
				}
			}
		}

		Vec2 tc0 = { 0.0f,0.0f };
		Vec2 tc1 = { 0.0f,1.0f };
		Vec2 tc2 = { 1.0f,1.0f }; 
		Vec2 tc3 = { 1.0f,0.0f };

		Vec3 pos0 = Vec3(rectangle[0].x, rectangle[0].y, 0.0f);
		Vec3 pos1 = Vec3(rectangle[1].x, rectangle[1].y, 0.0f);
		Vec3 pos2 = Vec3(rectangle[2].x, rectangle[2].y, 0.0f);
		Vec3 pos3 = Vec3(rectangle[3].x, rectangle[3].y, 0.0f);

		TexVertex tv0 = { pos0, tc0 };
		TexVertex tv1 = { pos1, tc1 };
		TexVertex tv2 = { pos2, tc2 };
		TexVertex tv3 = { pos3, tc3 };

		gfx.DrawTriangleTex(tv0, tv1, tv2, texture);
		gfx.DrawTriangleTex(tv0, tv2, tv3, texture);
	}

	// arrow
	
	const Vec4 VEC0 = RECTANGLE[0];
	const Vec4 VEC1 = RECTANGLE[2];
		
	const Vec3 ARROW = VEC1 - VEC0;
	const Vec3 NORMAL = ARROW.GetNormalized();
	const float LENGTH = ARROW.Len();

	const Vec4 VEC2 = Vec3(VEC0) + (NORMAL * Mat3::Scaling(LENGTH - LENGTH / 30.0f));
	const Vec4 VEC3 = Vec4(NORMAL * Mat3::Scaling(LENGTH / 100.0f) * Mat3::RotationZ(-PI / 2.0f)) * Mat4::Translation(VEC2);
	const Vec4 VEC4 = Vec4(NORMAL * Mat3::Scaling(LENGTH / 100.0f) * Mat3::RotationZ(PI / 2.0f)) * Mat4::Translation(VEC2);

	std::vector<Vec2> arrow;		

	arrow.emplace_back(VEC0 * ROTATE * TRANSLATE);
	arrow.emplace_back(VEC2 * ROTATE * TRANSLATE);
	arrow.emplace_back(VEC3 * ROTATE * TRANSLATE);
	arrow.emplace_back(VEC1 * ROTATE * TRANSLATE);
	arrow.emplace_back(VEC4 * ROTATE * TRANSLATE);
	arrow.emplace_back(VEC2 * ROTATE * TRANSLATE);
		
	gfx.DrawPolygon2D(false, arrow, Colors::Red);

	if (false)
	{
		for (int i = 0; i < arrow.size(); i++)
		{
			const RectF rect = { arrow[i].y - 3,arrow[i].y + 3, arrow[i].x - 3, arrow[i].x + 3 };
			gfx.DrawRect(true, rect, colors[i]);
		}
	}

	if(true)
	{
		// CROSSHAIR

		const float OFFSET = 25.0f;

		const std::vector<Vec4> ORIGIN = {
			{ -OFFSET,  0.0f,0.0f,1.0f },
			{  OFFSET,  0.0f,0.0f,1.0f },
			{   0.0f,-OFFSET,0.0f,1.0f },
			{   0.0f,OFFSET,0.0f,1.0f } };


		std::vector<Vec2> origin;

		for (int i = 0; i < ORIGIN.size(); i++)
		{
			origin.emplace_back(ORIGIN[i] * TRANSLATE);
		}

		gfx.DrawLine(origin[0], origin[1], Colors::LightGray);
		gfx.DrawLine(origin[2], origin[3], Colors::LightGray);
	}

	if (angle >= PI * 2)
	{
		angle = 0.0f;
	}	
	else
	{
		angle += 0.01f;
	}*/
}

Surface Game::CreateColorBlendTexture(const RectUI& RECTANGLE, const Color& color_start, const Color& color_end)
{
	Surface temp(RECTANGLE.GetWidth(), RECTANGLE.GetHeight());

	const Vec2	LINE = { static_cast<float>(RECTANGLE.GetWidth()),static_cast<float>(RECTANGLE.GetHeight()) };
	const float	LENGTH = LINE.Len();
	const Vec2	NORMAL = LINE.GetNormalized();

	float delta_red = static_cast<float>(color_end.GetR() - color_start.GetR());
	float delta_green = static_cast<float>(color_end.GetG() - color_start.GetG());
	float delta_blue = static_cast<float>(color_end.GetB() - color_start.GetB());

	Vec2 pixel;
	float length = 0.0f;
	Color c;

	for (unsigned int y = 0u; y < RECTANGLE.GetHeight(); y++)
	{
		for (unsigned int x = 0u; x < RECTANGLE.GetWidth(); x++)
		{
			pixel = { static_cast<float>(x),static_cast<float>(y) };
			length = NORMAL.DotProduct(pixel);

			c = Color(255,
				static_cast<unsigned char>(color_start.GetR() + delta_red * length / LENGTH),
				static_cast<unsigned char>(color_start.GetG() + delta_green * length / LENGTH),
				static_cast<unsigned char>(color_start.GetB() + delta_blue * length / LENGTH));

			temp.PutPixel(x, y, c);
		}
	}

	return temp;
}