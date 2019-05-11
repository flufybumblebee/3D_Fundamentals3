#pragma once

#include "TickTackToe.h"

#include "BumbleLibrary.h"
#include <memory>

using namespace Bumble;

/*-----------------------------------------------------*/

TickTackToe::TickTackToe(Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	:
	kbd(kbd),
	mouse(mouse),
	gfx(gfx)
{
	background_textures.push_back(	Surface::FromFile(L"Textures\\Backgrounds\\BlocksRainbow.jpg"));
	background_textures.push_back(	Surface::FromFile(L"Textures\\Backgrounds\\Nature1.jpg"));
	background_textures.push_back(	Surface::FromFile(L"Textures\\Backgrounds\\Nature3.jpg"));
	grid_textures.push_back(		Surface::FromFile(L"Textures\\TickTackToe\\Grid.png"));
	x_textures_A.push_back(			Surface::FromFile(L"Textures\\TickTackToe\\X.png"));
	o_textures_A.push_back(			Surface::FromFile(L"Textures\\TickTackToe\\O.png"));
	x_textures_B.push_back(		Surface::FromFile(L"Textures\\TickTackToe\\XRed.png"));
	o_textures_B.push_back(		Surface::FromFile(L"Textures\\TickTackToe\\ORed.png"));

	/* why 4.5 and 9.5?	*/
	const int left			= (scrW / 2) - (size * 7);
	const int middle_left	= left + static_cast<int>(size * 4.5f);
	const int middle_right	= left + static_cast<int>(size * 9.5f);
	const int right			= left + (size * 14);

	const int top			= (scrH / 2) - (size * 7);
	const int top_middle	= top + static_cast<int>(size * 4.5f);
	const int bottom_middle	= top + static_cast<int>(size * 9.5);
	const int bottom		= top + (size * 14);
	
	mouse_cells[0][0] = { top,top_middle - 1,left,middle_left - 1 };
	mouse_cells[0][1] = { top,top_middle - 1,middle_left,middle_right };
	mouse_cells[0][2] = { top,top_middle - 1,middle_right + 1,right };

	mouse_cells[1][0] = { top_middle,bottom_middle,left,middle_left - 1 };
	mouse_cells[1][1] = { top_middle,bottom_middle,middle_left,middle_right };
	mouse_cells[1][2] = { top_middle,bottom_middle,middle_right + 1,right };

	mouse_cells[2][0] = { bottom_middle + 1,bottom,left,middle_left - 1 };
	mouse_cells[2][1] = { bottom_middle + 1,bottom,middle_left,middle_right };
	mouse_cells[2][2] = { bottom_middle + 1,bottom,middle_right + 1,right };
	
	const unsigned int TOP = (scrH / 2) - (size * 7);
	const unsigned int LEFT = (scrW / 2) - (size * 7);

	Setup();

	for (unsigned int y = 0; y < rows; y++)
	{
		for (unsigned int x = 0; x < cols; x++)
		{
			const float left	= static_cast<float>(LEFT	+ ((size * 5u) * x));
			const float top		= static_cast<float>(TOP	+ ((size * 5u) * y));
			const float right	= static_cast<float>(LEFT	+ (((size * 5u) * x) + (size * 4u)));
			const float bottom	= static_cast<float>(TOP	+ (((size * 5u) * y) + (size * 4u)));

			const RectF rect = { top,bottom,left,right };
			Cell cell(rect);
			cells.emplace_back( cell );
		}
	}
}

/*-----------------------------------------------------*/

void TickTackToe::Setup()
{
	size_t zero					= 0;
	size_t random_backgrounds	= RandomInt(zero,background_textures.size() - 1);
	size_t random_grids			= RandomInt(zero,grid_textures.size() - 1);
	size_t random_Xs			= RandomInt(zero,x_textures_A.size() - 1);
	size_t random_Os			= RandomInt(zero,o_textures_A.size() - 1);

	background_tex	= &background_textures[random_backgrounds];
	grid_tex		= &grid_textures[random_grids];
	x_tex_black			= &x_textures_A[random_Xs];
	o_tex_black			= &o_textures_A[random_Os];
	x_tex_red		= &x_textures_B[random_Xs];
	o_tex_red		= &o_textures_B[random_Os];

	nTurns = 0;

	winner[0] = -1;
	winner[1] = -1;
	winner[2] = -1;

	for (int i = 0; i < blocks.size(); i++)
	{
		SetState(i, EMPTY);
	}

	cells.clear();

	gameIsOver = false;

	int randomNum = RandomInt(0, 1);
	current_player_state = (randomNum > 0) ? X : O;
}
void TickTackToe::Update()
{	
	InputA();
	SetCell();
	GameOver();

	/*if (current_player_state == X)
	{
		InputA();
	}
	else
	{
		InputB();
	}*/
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
void TickTackToe::InputA()
{
	/*if (!gameIsOver)
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
				EndTurn();
			}

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
	}*/

	if (!gameIsOver)
	{		
		int mouseX = mouse.GetPosX();
		int mouseY = mouse.GetPosY();
		
		for (int y = 0; y < rows; y++)
		{
			for (int x = 0; x < cols; x++)
			{
				if (mouseX > mouse_cells[y][x].left &&
					mouseX < mouse_cells[y][x].right &&
					mouseY > mouse_cells[y][x].top &&
					mouseY < mouse_cells[y][x].bottom)
				{
					current_x = x;
					current_y = y;
				}
			}
		}

		if (!mouseLeftIsPressed && mouse.LeftIsPressed())
		{
			mouseLeftIsPressed = true;

			XOState state = GetState(current_x, current_y);

			if (state == EMPTY)
			{
				SetState(current_x, current_y, current_player_state);

				EndTurn();
			}
		}
		else
		{
			if (!mouse.LeftIsPressed())
			{
				mouseLeftIsPressed = false;
			}
		}
	}
	else
	{
		if (!mouseLeftIsPressed && mouse.LeftIsPressed())
		{
			mouseLeftIsPressed = true;

			Setup();
		}
		else
		{
			if (!mouse.LeftIsPressed())
			{
				mouseLeftIsPressed = false;
			}
		}
	}
}

void TickTackToe::InputB()
{

}

void TickTackToe::SetCell()
{
	Surface* pSurf = nullptr;

	XOState gameOverState = GameOver();

	for (unsigned int y = 0; y < rows; y++)
	{
		for (unsigned int x = 0; x < cols; x++)
		{
			size_t i = static_cast<size_t>(y) * cols + x;

			XOState state = GetState(x, y);

			if (gameOverState != EMPTY)
			{
				if (
					i == winner[0] ||
					i == winner[1] ||
					i == winner[2])
				{
					if (state == X)
					{
						pSurf = x_tex_red;
					}
					else if (state == O)
					{
						pSurf = o_tex_red;
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
						pSurf = x_tex_black;
					}
					else if (state == O)
					{
						pSurf = o_tex_black;
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
					pSurf = x_tex_black;
				}
				else if (state == O)
				{
					pSurf = o_tex_black;
				}
				else
				{
					continue;
				}
			}

			cells[i].SetTexture(o_tex_black);
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

	// Horizontal Lines
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
	// Vertical Lines
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
	// Diagonal Lines
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
	gfx.DrawTriangleTex(background_tex_vertex_0, background_tex_vertex_1, background_tex_vertex_2, *background_tex);
	gfx.DrawTriangleTex(background_tex_vertex_0, background_tex_vertex_2, background_tex_vertex_3, *background_tex);
}
void TickTackToe::DrawXOState()
{	
	/*Surface* pSurf = nullptr;

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
						pSurf = x_tex_red;
					}
					else if (state == O)
					{
						pSurf = o_tex_red;
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
						pSurf = x_tex_black;
					}
					else if (state == O)
					{
						pSurf = o_tex_black;
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
					pSurf = x_tex_black;
				}
				else if (state == O)
				{
					pSurf = o_tex_black;
				}
				else
				{
					continue;
				}
			}

            gfx.DrawTriangleTex(tv0, tv1, tv2, *pSurf);
			gfx.DrawTriangleTex(tv0, tv2, tv3, *pSurf);
		}
	}*/

	for (int i = 0; i < cells.size(); i++)
	{
		cells[i].Draw(gfx);
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
	if(false)
	{
		const Color color = Color(170, 255, 255, 255);

		{
			// left vertical - top

			int left = (scrW / 2) - (size * 3);
			int right = (scrW / 2) - (size * 2);
			int top = (scrH / 2) - (size * 7);
			int bottom = (scrH / 2) - (size * 3);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// left vertical - middle

			int left = (scrW / 2) - (size * 3);
			int right = (scrW / 2) - (size * 2);
			int top = (scrH / 2) - (size * 2);
			int bottom = (scrH / 2) + (size * 2);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// left vertical - bottom

			int left = (scrW / 2) - (size * 3);
			int right = (scrW / 2) - (size * 2);
			int top = (scrH / 2) + (size * 3);
			int bottom = (scrH / 2) + (size * 7);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// right vertical - top

			int left = (scrW / 2) + (size * 2);
			int right = (scrW / 2) + (size * 3);
			int top = (scrH / 2) - (size * 7);
			int bottom = (scrH / 2) - (size * 3);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// right vertical - middle

			int left = (scrW / 2) + (size * 2);
			int right = (scrW / 2) + (size * 3);
			int top = (scrH / 2) - (size * 2);
			int bottom = (scrH / 2) + (size * 2);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// right vertical - bottom

			int left = (scrW / 2) + (size * 2);
			int right = (scrW / 2) + (size * 3);
			int top = (scrH / 2) + (size * 3);
			int bottom = (scrH / 2) + (size * 7);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// top horizontal

			int left = (scrW / 2) - (size * 7);
			int right = (scrW / 2) + (size * 7);
			int top = (scrH / 2) - (size * 3);
			int bottom = (scrH / 2) - (size * 2);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}

		{
			// bottom horizontal

			int left = (scrW / 2) - (size * 7);
			int right = (scrW / 2) + (size * 7);
			int top = (scrH / 2) + (size * 2);
			int bottom = (scrH / 2) + (size * 3);

			gfx.DrawRectAlpha(left, top, right - left, bottom - top, color);
		}
	}
	else
	{
		gfx.DrawTriangleTex(grid_tex_vertex_0, grid_tex_vertex_1, grid_tex_vertex_2, *grid_tex);
		gfx.DrawTriangleTex(grid_tex_vertex_0, grid_tex_vertex_2, grid_tex_vertex_3, *grid_tex);
	}
}