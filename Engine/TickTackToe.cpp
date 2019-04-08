#pragma once

#include "TickTackToe.h"

#include "Random.h"
#include <memory>

/*-----------------------------------------------------*/

TickTackToe::TickTackToe(Keyboard& kbd, Graphics& gfx)
	:
	kbd(kbd),
	gfx(gfx)
{
	//Surface tex_background = Surface::FromFile(L"Textures\\Backgrounds\\BlocksRainbow.jpg");
	/*Surface tex_background2 = Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg");
	Surface tex_grid = Surface::FromFile(L"Textures\\TickTackToe\\grid(16x16).png");
	Surface tex_X = Surface::FromFile(L"Textures\\TickTackToe\\X.png");
	Surface tex_O = Surface::FromFile(L"Textures\\TickTackToe\\O.png");*/

	tex_backgrounds.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksRainbow.jpg"));
	tex_backgrounds.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg"));
	tex_backgrounds.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.jpg"));
	tex_grids.push_back(Surface::FromFile(L"Textures\\TickTackToe\\grid0.png"));
	tex_grids.push_back(Surface::FromFile(L"Textures\\TickTackToe\\grid1.png"));
	tex_Xs.push_back(Surface::FromFile(L"Textures\\TickTackToe\\X.png"));
	tex_Os.push_back(Surface::FromFile(L"Textures\\TickTackToe\\O.png"));

	tex_background = &tex_backgrounds[RND::Random(static_cast<size_t>(0), tex_backgrounds.size() - 1)];
	tex_grid = &tex_grids[RND::Random(static_cast<size_t>(0), tex_grids.size() - 1)];
	tex_X = &tex_Xs[RND::Random(static_cast<size_t>(0), tex_Xs.size() - 1)];
	tex_O = &tex_Os[RND::Random(static_cast<size_t>(0), tex_Os.size() - 1)];
}

/*-----------------------------------------------------*/

void TickTackToe::Setup()
{
	for (int i = 0; i < 9; i++)
	{
		SetState(i, EMPTY);
	}

	SetPlayers();
}
void TickTackToe::Update()
{
	Input();	
}
void TickTackToe::Draw()
{
	DrawBackground();
	DrawCursor();
	DrawGrid();
	DrawXOState();
}

/*-----------------------------------------------------*/

void TickTackToe::SetPlayers()
{
	int randomNum = RND::Random(0, 1);
	current_player_state = (randomNum > 0) ? X : O;
}
void TickTackToe::ChangePlayer()
{
	if (current_player_state == X)
	{
		current_player_state = O;
	}
	else
	{
		current_player_state = X;
	}
}
void TickTackToe::Input()
{
	if (!keyIsPressed)
	{
		if (kbd.KeyIsPressed(VK_LEFT) && current_x > 0)
		{
			current_x--;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_RIGHT) && current_x < 2)
		{
			current_x++;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_UP) && current_y > 0)
		{
			current_y--;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_DOWN) && current_y < 2)
		{
			current_y++;
			keyIsPressed = true;
		}
		if (kbd.KeyIsPressed(VK_SPACE))
		{
			XOState state = GetState(current_x, current_y);
			if ( state == EMPTY )
			{
				SetState(current_x, current_y, current_player_state);
			}

			ChangePlayer();

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

/*-----------------------------------------------------*/

void					TickTackToe::SetState(int i, XOState state)
{
	blocks[i] = state;
}
void					TickTackToe::SetState(int ix, int iy, XOState state)
{
	SetState((iy * cols + ix), state);
}
TickTackToe::XOState	TickTackToe::GetState(int i)
{
	return blocks[i];
}
TickTackToe::XOState	TickTackToe::GetState(int ix, int iy)
{
	return blocks[iy * cols + ix];
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
	
	TexVertex tv0 = { pos0,tc0 };
	TexVertex tv1 = { pos1,tc1 };
	TexVertex tv2 = { pos2,tc2 };
	TexVertex tv3 = { pos3,tc3 };

	gfx.DrawTriangleTex(tv0, tv1, tv2, *tex_background);
	gfx.DrawTriangleTex(tv0, tv2, tv3, *tex_background);
}
void TickTackToe::DrawXOState()
{
	int wi = gfx.ScreenWidth;
	int hi = gfx.ScreenHeight;
	int si = hi / 16;

	Surface* pSurf;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			float left = static_cast<float>(((wi / 2) - (si * 7)) + ((si * 5) * x));
			float top = static_cast<float>(((hi / 2) - (si * 7)) + ((si * 5) * y));
			float right = static_cast<float>(((wi / 2) - (si * 7)) + (((si * 5) * x) + (si * 4)));
			float bottom = static_cast<float>(((hi / 2) - (si * 7)) + (((si * 5) * y) + (si * 4)));

			Vec3 pos0 = { left,top,0.0f };
			Vec3 pos1 = { right,top,0.0f };
			Vec3 pos2 = { right,bottom,0.0f };
			Vec3 pos3 = { left,bottom,0.0f };

			TexVertex tv0 = { pos0,tc0 };
			TexVertex tv1 = { pos1,tc1 };
			TexVertex tv2 = { pos2,tc2 };
			TexVertex tv3 = { pos3,tc3 };

			XOState state = GetState(x, y);

			if ( state == X)
			{
				pSurf = tex_X;
			}
			else if (state == O)
			{
				pSurf = tex_O;
			}            
			else
			{
				continue;
			}

			gfx.DrawTriangleTex(tv0, tv1, tv2, *pSurf);
			gfx.DrawTriangleTex(tv0, tv2, tv3, *pSurf);
		}
	}
}
void TickTackToe::DrawCursor()
{
	int xOut = ((scrW / 2) - (size * 7)) + (((size * 4) * current_x) + (current_x * (size)));
	int yOut = ((scrH / 2) - (size * 7)) + (((size * 4) * current_y) + (current_y * (size)));

	Color c = Color( 100, 255, 255, 255 );

	gfx.DrawRectAlpha(xOut, yOut, size * 4, size * 4, c);
}
void TickTackToe::DrawGrid()
{
	float left		= static_cast<float>((scrW / 2) - (scrH / 2));
	float top		= 0.0f;
	float right		= static_cast<float>((scrW / 2) + (scrH / 2));
	float bottom	= scrH - 1.0f;

	Vec3 top_left		= { left,top,0.0f };
	Vec3 top_right		= {	right,top,0.0f };
	Vec3 bottom_right	= { right,bottom,0.0f };
	Vec3 bottom_left	= { left,bottom,0.0f };

	TexVertex tv0 = { top_left,tc0 };
	TexVertex tv1 = { top_right,tc1 };
	TexVertex tv2 = { bottom_right,tc2 };
	TexVertex tv3 = { bottom_left,tc3 };

	gfx.DrawTriangleTex(tv0, tv1, tv2, *tex_grid);
	gfx.DrawTriangleTex(tv0, tv2, tv3, *tex_grid);

	/*gfx.DrawRect(line0X, line0Y, size * 1, size * 14, Colors::Gray);
	gfx.DrawRect(line1X, line1Y, size * 1, size * 14, Colors::Green);
	gfx.DrawRect(line2X, line2Y, size * 14, size * 1, Colors::White);
	gfx.DrawRect(line3X, line3Y, size * 14, size * 1, Colors::Yellow);*/
}

/*-----------------------------------------------------*/

std::vector<Color>	TickTackToe::ConvertSurfaceToColorVector(const Surface & surface)
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
Surface				TickTackToe::ConvertColorVectorToSurface(int w, int h, const std::vector<Color> & colors)
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

