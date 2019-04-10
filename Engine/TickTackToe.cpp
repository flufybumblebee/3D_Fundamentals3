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
	tex_backgrounds.push_back(Surface::FromFile(L"Textures\\Backgrounds\\BlocksRainbow.jpg"));
	tex_backgrounds.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg"));
	tex_backgrounds.push_back(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.jpg"));
	tex_grids.push_back(Surface::FromFile(L"Textures\\TickTackToe\\grid6.png"));
	//tex_grids.push_back(Surface::FromFile(L"Textures\\TickTackToe\\grid1.png"));
	tex_Xs.push_back(Surface::FromFile(L"Textures\\TickTackToe\\X.png"));
	tex_Os.push_back(Surface::FromFile(L"Textures\\TickTackToe\\O.png"));
	tex_Xs_red.push_back(Surface::FromFile(L"Textures\\TickTackToe\\XRed.png"));
	tex_Os_red.push_back(Surface::FromFile(L"Textures\\TickTackToe\\ORed.png"));

	size_t zero = 0;
	size_t random_backgrounds = RND::Random(zero,tex_backgrounds.size() - 1);
	size_t random_grids = RND::Random(zero,tex_grids.size() - 1);
	size_t random_Xs = RND::Random(zero,tex_Xs.size() - 1);
	size_t random_Os = RND::Random(zero,tex_Os.size() - 1);

	tex_background = &tex_backgrounds[random_backgrounds];
	tex_grid = &tex_grids[random_grids];
	tex_X = &tex_Xs[random_Xs];
	tex_O = &tex_Os[random_Os];
	tex_X_red = &tex_Xs_red[random_Xs];
	tex_O_red = &tex_Os_red[random_Os];
}

/*-----------------------------------------------------*/

void TickTackToe::Setup()
{
	nTurns = 0;

	winner[0] = 0;
	winner[1] = 0;
	winner[2] = 0;

	for (int i = 0; i < blocks.size(); i++)
	{
		SetState(i, EMPTY);
	}

	gameIsOver = false;

	current_x = 1;
	current_y = 1;

	int randomNum = RND::Random(0, 1);
	current_player_state = (randomNum > 0) ? X : O;
}
void TickTackToe::Update()
{	
	GameOver();
	Input();	
}
void TickTackToe::Draw()
{
	DrawBackground();
	if (!gameIsOver)
	{
		DrawCursor();
	}
	DrawGrid();
	DrawXOState();
}

/*-----------------------------------------------------*/

void TickTackToe::EndTurn()
{
	if (current_player_state == X)
	{
		current_player_state = O;
	}
	else
	{
		current_player_state = X;
	}
	nTurns++;
}
void TickTackToe::Input()
{
	if (!gameIsOver)
	{
		if (!leftIsPressed && kbd.KeyIsPressed(VK_LEFT) && current_x > 0)
		{
			current_x--;
			leftIsPressed = true;
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_LEFT))
			{
				leftIsPressed = false;
			}
		}

		if (!rightIsPressed && kbd.KeyIsPressed(VK_RIGHT) && current_x < 2)
		{
			current_x++;
			rightIsPressed = true;
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_RIGHT))
			{
				rightIsPressed = false;
			}
		}

		if (!upIsPressed && kbd.KeyIsPressed(VK_UP) && current_y > 0)
		{
			current_y--;
			upIsPressed = true;
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_UP))
			{
				upIsPressed = false;
			}
		}

		if (!downIsPressed && kbd.KeyIsPressed(VK_DOWN) && current_y < 2)
		{
			current_y++;
			downIsPressed = true;
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_DOWN))
			{
				downIsPressed = false;
			}
		}

		if (!spaceIsPressed && kbd.KeyIsPressed(VK_SPACE))
		{
			spaceIsPressed = true;

			XOState state = GetState(current_x, current_y);

			if (state == EMPTY)
			{
				SetState(current_x, current_y, current_player_state);
			}

			EndTurn();
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_SPACE))
			{
				spaceIsPressed = false;
			}
		}
	}
	else
	{
		if (!returnIsPressed && kbd.KeyIsPressed(VK_RETURN))
		{
			Setup();

			returnIsPressed = true;			
		}
		else
		{
			if (!kbd.KeyIsPressed(VK_RETURN))
			{
				returnIsPressed = false;
			}
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
	size_t i = static_cast<size_t>(iy) * static_cast<size_t>(cols) + ix;
	return blocks[i];
}

TickTackToe::XOState TickTackToe::GameOver()
{
	if (nTurns > 8)
	{
		gameIsOver = true;
	}	

	if (blocks[0] == blocks[1] && blocks[1] == blocks[2] && blocks[0] != EMPTY)
	{
		winner[0] = 0;
		winner[1] = 1;
		winner[2] = 2;

		gameIsOver = true;
		return blocks[0];
	}
	else if (blocks[3] == blocks[4] && blocks[4] == blocks[5] && blocks[3] != EMPTY)
	{
		winner[0] = 3;
		winner[1] = 4;
		winner[2] = 5;

		gameIsOver = true;
		return blocks[3];
	}
	else if (blocks[6] == blocks[7] && blocks[7] == blocks[8] && blocks[6] != EMPTY)
	{
		winner[0] = 6;
		winner[1] = 7;
		winner[2] = 8;

		gameIsOver = true;
		return blocks[6];
	}
	else if (blocks[0] == blocks[3] && blocks[3] == blocks[6] && blocks[0] != EMPTY)
	{
		winner[0] = 0;
		winner[1] = 3;
		winner[2] = 6;

		gameIsOver = true;
		return blocks[0];
	}
	else if (blocks[1] == blocks[4] && blocks[4] == blocks[7] && blocks[1] != EMPTY)
	{
		winner[0] = 1;
		winner[1] = 4;
		winner[2] = 7;

		gameIsOver = true;
		return blocks[1];
	}
	else if (blocks[2] == blocks[5] && blocks[5] == blocks[8] && blocks[2] != EMPTY)
	{
		winner[0] = 2;
		winner[1] = 5;
		winner[2] = 8;

		gameIsOver = true;
		return blocks[2];
	}
	else if (blocks[0] == blocks[4] && blocks[4] == blocks[8] && blocks[0] != EMPTY)
	{
		winner[0] = 0;
		winner[1] = 4;
		winner[2] = 8;

		gameIsOver = true;
		return blocks[0];
	}
	else if (blocks[2] == blocks[4] && blocks[4] == blocks[6] && blocks[2] != EMPTY)
	{
		winner[0] = 2;
		winner[1] = 4;
		winner[2] = 6;

		gameIsOver = true;
		return blocks[2];
	}
	else
	{
		return EMPTY;
	}
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
	Surface* pSurf = nullptr;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			float left		= static_cast<float>(((scrW / 2) - (size * 7)) +  ((size * 5) * x));
			float top		= static_cast<float>(((scrH / 2) - (size * 7)) +  ((size * 5) * y));
			float right		= static_cast<float>(((scrW / 2) - (size * 7)) + (((size * 5) * x) + (size * 4)));
			float bottom	= static_cast<float>(((scrH / 2) - (size * 7)) + (((size * 5) * y) + (size * 4)));

			Vec3 pos0 = { left,top,0.0f };
			Vec3 pos1 = { right,top,0.0f };
			Vec3 pos2 = { right,bottom,0.0f };
			Vec3 pos3 = { left,bottom,0.0f };

			TexVertex tv0 = { pos0,tc0 };
			TexVertex tv1 = { pos1,tc1 };
			TexVertex tv2 = { pos2,tc2 };
			TexVertex tv3 = { pos3,tc3 };

			XOState state = GetState(x, y);
			size_t i = static_cast<size_t>(y) * cols + x;

			if (GameOver() != EMPTY)
			{
				if (
					i == winner[0] ||
					i == winner[1] ||
					i == winner[2])
				{
					if (state == X)
					{
						pSurf = tex_X_red;
					}
					else if (state == O)
					{
						pSurf = tex_O_red;
					}
					else
					{
						continue;
					}
				}
				else
				{
					if (state == X)
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
				}
			}
			else
			{
				if (state == X)
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

	for (size_t iy = 0; iy < h; iy++)
	{
		for (size_t ix = 0; ix < w; ix++)
		{
			size_t i = iy * static_cast<size_t>(w) + ix;
			surface.PutPixel(static_cast<unsigned int>(ix), static_cast<unsigned int>(iy), colors[i]);
		}
	}

	return std::move(surface);
}

