#pragma once

#include "TickTackToe.h"

#include <memory>

TickTackToe::TickTackToe(Keyboard& kbd, Graphics& gfx)
	:
	kbd(kbd),
	gfx(gfx)
{}


void TickTackToe::Setup()
{
	for (int i = 0; i < 9; i++)
	{
		SetState(i, EMPTY);
	}
}

void TickTackToe::Update()
{
	Input();
}

void TickTackToe::Draw()
{
	DrawBackground();
	DrawCursor(currentX, currentY);
	DrawGrid();
}

/*----------------------------------------*/



void TickTackToe::Input()
{
	if (!keyIsPressed)
	{
		if (kbd.KeyIsPressed(VK_LEFT) && currentX > 0)
		{
			currentX--;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_RIGHT) && currentX < 2)
		{
			currentX++;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_UP) && currentY > 0)
		{
			currentY--;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_DOWN) && currentY < 2)
		{
			currentY++;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			keyIsPressed = true;
		}
	}
	else
	{
		if (!kbd.KeyIsPressed(VK_LEFT) &&
			!kbd.KeyIsPressed(VK_RIGHT) &&
			!kbd.KeyIsPressed(VK_UP) &&
			!kbd.KeyIsPressed(VK_DOWN) &&
			!kbd.KeyIsPressed(VK_SPACE))
		{
			keyIsPressed = false;
		}
	}
}

void TickTackToe::SetState(int i, XOState state)
{
	blocks[i] = state;
}

void TickTackToe::SetState(int ix, int iy, XOState state)
{
	SetState(ConvertArrayAddress(ix, iy, cols), state);
}

TickTackToe::XOState TickTackToe::GetState(int i)
{
	return blocks[i];
}

TickTackToe::XOState TickTackToe::GetState(int ix, int iy)
{
	return blocks[ConvertArrayAddress(ix, iy, cols)];
}


/*-----------------------------------------------------*/

void TickTackToe::DrawBackground()
{
	float width = scrW - 1.0f;
	float height = scrH - 1.0f;

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

void TickTackToe::DrawCursor(int x, int y)
{
	int w = gfx.ScreenWidth;
	int h = gfx.ScreenHeight;
	int size = h / 16;

	int xOut = ((w / 2) - (size * 7)) + (((size * 4) * x) + (x * (size)));
	int yOut = ((h / 2) - (size * 7)) + (((size * 4) * y) + (y * (size)));

	gfx.DrawRect(xOut, yOut, chunk_size * 4, chunk_size * 4, Colors::Red);
}

//void TickTackToe::DrawCursor(int x, int y)
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

void TickTackToe::DrawGrid()
{
	gfx.DrawRect(line0X, line0Y, chunk_size * 1, chunk_size * 14, Colors::Gray);
	gfx.DrawRect(line1X, line1Y, chunk_size * 1, chunk_size * 14, Colors::Green);
	gfx.DrawRect(line2X, line2Y, chunk_size * 14, chunk_size * 1, Colors::White);
	gfx.DrawRect(line3X, line3Y, chunk_size * 14, chunk_size * 1, Colors::Yellow);
}

/*--------------------------------------------------------------*/

int TickTackToe::ConvertArrayAddress(int ix, int iy, int w)
{
	return iy * w + ix;
}

std::vector<Color> TickTackToe::ConvertSurfaceToColorVector(const Surface & surface)
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

	return std::move(output);
}

Surface TickTackToe::ConvertColorVectorToSurface(int w, int h, const std::vector<Color> & colors)
{
	Surface surface(w, h);

	for (int iy = 0; iy < h; iy++)
	{
		for (int ix = 0; ix < w; ix++)
		{
			surface.PutPixel(ix, iy, colors[iy * w + ix]);
		}
	}

	return std::move(surface);
}

