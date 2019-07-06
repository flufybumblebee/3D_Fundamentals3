#include "Minesweeper.h"

Minesweeper::Minesweeper()
{	
	Initialise();	
}

/*--------------------------------------------*/

void Minesweeper::Update(Mouse& mouse)
{
	if (grid != nullptr)
	{
		SetButtons(mouse);

		if (grid != nullptr)
		{
			SetBlocks(mouse);

			if (!gameover)
			{
				SetTimer();
				SetMinesCounter();
			}

			SetGameOver();

			grid->Update();
		}
	}
	else
	{
		SetSettings(mouse);
	}
}
void Minesweeper::Draw(Graphics& gfx)
{
	if (grid != nullptr)
	{
		DrawBorder(gfx);
		DrawDisplayBackground(gfx);

		DrawMinesCounter(gfx);
		DrawButtons(gfx);
		DrawTimer(gfx);

		grid->Draw(gfx);

		DrawGameOver(gfx);
	}
	else
	{
		DrawSettings(gfx);
	}
}

/*--------------------------------------------*/

void Minesweeper::Initialise()
{	
	InitialiseTextures();
	InitialiseSounds();
	InitialiseSettings();
}

void Minesweeper::Setup()
{
	// buttons: small, medium, large - DONE
	// custom: columns, rows, mines
	// alternate block size dependent on screen width and height
	// if camera, block size

	if (grid != nullptr)
	{
		mines = grid->GetMines();

		InitialiseBorder();
		InitialiseMinesCounter();
		InitialiseButtons();
		InitialiseTimer();
		InitialiseGameOver();

		Reset();
	}
}
void Minesweeper::Reset()
{
	timer_started	= false;
	gameover		= false;
	gamewon			= false;
	sound_played	= false;
	is_help			= false;
	is_settings		= false;
	mines			= grid->GetMines();
	flags			= 0u;
	time			= 0u;
	frames			= 0u;
	index0			= 0u;
	index1			= 0u;

	settings_blocks[0].SetTexture(settings_textures[0]);
	settings_blocks[1].SetTexture(settings_textures[0]);
	settings_blocks[2].SetTexture(settings_textures[0]);
	
	grid->SetTileValues();
	
	button_blocks[4].SetTexture(button_textures[2]);
	button_blocks[5].SetTexture(button_textures[2]);
}

void Minesweeper::InitialiseTextures()
{
	InitialiseSettingsTextures();
	InitialiseBorderTextures();
	InitialiseDigitTextures();
	InitialiseButtonTextures();
	InitialiseGameOverTextures();
}
void Minesweeper::InitialiseSounds()
{
	sounds.emplace_back(L"Sounds\\explosion.wav");
	sounds.emplace_back(L"Sounds\\fanfare.wav");
	sounds.emplace_back(L"Sounds\\click_0.wav");
	sounds.emplace_back(L"Sounds\\click_1.wav");
	sounds.emplace_back(L"Sounds\\click_2.wav");
}
void Minesweeper::InitialiseHelp()
{

}
void Minesweeper::InitialiseSettings()
{
	for (int i = 0; i < 3; i++)
	{
		settings_blocks.emplace_back(RectUI(50u + i * 40u, 90u + i * 40u, 50u, 90u), settings_textures[0]);
	}

	settings_confirmation = { RectUI{500u,540u, 50u,90u},settings_textures[2] };
	is_settings = true;
}

/*--------------------------------------------*/

void Minesweeper::InitialiseSettingsTextures()
{
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\radio_button_off.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\radio_button_on.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_blank.png"));
}
void Minesweeper::InitialiseBorderTextures()
{
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_horizontal.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_verticle.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_top_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_top_right.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_bottom_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_corner_bottom_right.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_t_left.png"));
	border_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Border\\border_t_right.png"));
}
void Minesweeper::InitialiseDigitTextures()
{
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_0_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_1_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_2_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_3_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_4_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_5_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png"));
	digit_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_9_white.png"));
}
void Minesweeper::InitialiseButtonTextures()
{
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\question_mark.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\cog_white.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_smiley_0.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_smiley_1.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tile\\tile_smiley_2.png"));
}
void Minesweeper::InitialiseGameOverTextures()
{
	{
		const std::wstring filepath = L"Textures\\Minesweeper\\Explosion\\";
		const std::wstring filetype = L".png";

		for (unsigned int i = 0; i < EXPLOSION_FRAMES; i++)
		{
			const std::wstring number = std::to_wstring(i);
			const std::wstring filename =
				filepath + (std::wstring(4 - number.length(), '0') + number) + filetype;
			explosion_textures[i] = std::make_shared<Surface>(filename);
		}
	}

	{
		const std::wstring filepath = L"Textures\\Minesweeper\\Flag\\";
		const std::wstring filetype = L".png";

		for (unsigned int i = 0; i < FLAG_FRAMES; i++)
		{
			const std::wstring number = std::to_wstring(i + 10u);
			const std::wstring filename =
				filepath + (std::wstring(4 - number.length(), '0') + number) + filetype;
			flag_textures[i] = std::make_shared<Surface>(filename);
		}
	}
}

/*--------------------------------------------*/

void Minesweeper::InitialiseBorder()
{
	{
		const unsigned int TOP = 0u;
		const unsigned int BOTTOM = OFFSET * 6 + grid->GetTileSize() * grid->GetRows() + OFFSET - 1u;
		const unsigned int LEFT = 0u;
		const unsigned int RIGHT = OFFSET + grid->GetTileSize() * grid->GetCols() + OFFSET - 1u;;

		border_position = RectUI(TOP, BOTTOM, LEFT, RIGHT);
	}

	// Top Horizontal
	RectUI corner_top_left{ 
		border_position.top,
		border_position.top + OFFSET,
		border_position.left,
		border_position.left + OFFSET };

	RectUI top{
		border_position.top,
		border_position.top + OFFSET,
		border_position.left + OFFSET,
		border_position.right - OFFSET };

	RectUI corner_top_right{
		border_position.top,
		border_position.top + OFFSET,
		border_position.right - OFFSET,
		border_position.right};

	border_blocks.emplace_back(corner_top_left, border_textures[BORDER::CORNER_TOP_LEFT]);
	border_blocks.emplace_back(top, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(corner_top_right, border_textures[BORDER::CORNER_TOP_RIGHT]);
	
	// Middle Horizontal
	RectUI t_left{
		border_position.top + OFFSET * 5u,
		border_position.top + OFFSET * 6u,
		border_position.left,
		border_position.left + OFFSET };

	RectUI middle{
		border_position.top + OFFSET * 5u,
		border_position.top + OFFSET * 6u,
		border_position.left + OFFSET,
		border_position.right - OFFSET };

	RectUI t_right{
		border_position.top + OFFSET * 5u,
		border_position.top + OFFSET * 6u,
		border_position.right - OFFSET,
		border_position.right };

	border_blocks.emplace_back(t_left, border_textures[BORDER::T_LEFT]);
	border_blocks.emplace_back(middle, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(t_right, border_textures[BORDER::T_RIGHT]);

	// Bottom Horizontal
	RectUI corner_bottom_left{
		border_position.bottom - OFFSET,
		border_position.bottom,
		border_position.left,
		border_position.left + OFFSET };

	RectUI bottom{
		border_position.bottom - OFFSET,
		border_position.bottom,
		border_position.left + OFFSET,
		border_position.right - OFFSET };

	RectUI corner_bottom_right{
		border_position.bottom - OFFSET,
		border_position.bottom,
		border_position.right - OFFSET,
		border_position.right };

	border_blocks.emplace_back(corner_bottom_left, border_textures[BORDER::CORNER_BOTTOM_LEFT]);
	border_blocks.emplace_back(bottom, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(corner_bottom_right, border_textures[BORDER::CORNER_BOTTOM_RIGHT]);

	// Verticle Left
	RectUI left_top{
		border_position.top + OFFSET,
		border_position.top + OFFSET * 5,
		border_position.left,
		border_position.left + OFFSET };

	RectUI left_bottom{
		border_position.top + OFFSET * 6u,
		border_position.bottom - OFFSET,
		border_position.left,
		border_position.left + OFFSET };

	border_blocks.emplace_back(left_top, border_textures[BORDER::VERTICLE]);
	border_blocks.emplace_back(left_bottom, border_textures[BORDER::VERTICLE]);

	// Verticle Right
	RectUI right_top{
		border_position.top + OFFSET,
		border_position.top + OFFSET * 5,
		border_position.right - OFFSET,
		border_position.right };

	RectUI right_bottom{
		border_position.top + OFFSET * 6u,
		border_position.bottom - OFFSET,
		border_position.right - OFFSET,
		border_position.right };

	border_blocks.emplace_back(right_top, border_textures[BORDER::VERTICLE]);
	border_blocks.emplace_back(right_bottom, border_textures[BORDER::VERTICLE]);
}
void Minesweeper::InitialiseMinesCounter()
{	
	const unsigned int TOP = border_position.top + OFFSET * 1u + OFFSET * 2u - grid->GetTileSize() / 2u;
	const unsigned int RIGHT = OFFSET + grid->GetTileSize() * 3u;

	for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
	{
		for (unsigned int x = 0u; x < DIGIT_COLS; x++)
		{
			const RectUI POSITION = {
				TOP,
				TOP + grid->GetTileSize() - 1u,
				RIGHT - (grid->GetTileSize() * x) - grid->GetTileSize(),
				RIGHT - (grid->GetTileSize() * x) - 1u };

			mines_counter[y][x] = std::move(Block(POSITION, digit_textures[y]));
		}
	}
}
void Minesweeper::InitialiseButtons()
{
	{		
		const unsigned int TOP		= border_position.top + OFFSET * 1u + OFFSET * 2u - grid->GetTileSize() / 2u;
		const unsigned int BOTTOM	= TOP + grid->GetTileSize() - 1u;
		const unsigned int LEFT		= OFFSET + (grid->GetTileSize() * grid->GetCols()) / 2u - grid->GetTileSize() / 2u - grid->GetTileSize();
		const unsigned int RIGHT	= LEFT + grid->GetTileSize() - 1u;

		button_positions[0] = { TOP,BOTTOM,LEFT,RIGHT };
		button_positions[1] = { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

		button_blocks[0] = { button_positions[0], button_textures[0] };
		button_blocks[1] = { button_positions[1], button_textures[0] };
	}

	{
		const unsigned int TOP		= border_position.top + OFFSET * 1u + OFFSET * 2u - grid->GetTileSize() / 2u;
		const unsigned int BOTTOM	= TOP + grid->GetTileSize() - 1u;
		const unsigned int LEFT		= OFFSET + (grid->GetTileSize() * grid->GetCols()) / 2u + grid->GetTileSize() / 2u;
		const unsigned int RIGHT	= LEFT + grid->GetTileSize() - 1u;

		button_positions[2] = { TOP,BOTTOM,LEFT,RIGHT };
		button_positions[3] = { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

		button_blocks[2] = { button_positions[2], button_textures[1] };
		button_blocks[3] = { button_positions[3], button_textures[1] };
	}

	{
		const unsigned int TOP		= border_position.top + OFFSET * 1u + OFFSET * 2u - grid->GetTileSize() / 2u;
		const unsigned int BOTTOM	= TOP + grid->GetTileSize() - 1u;
		const unsigned int LEFT		= OFFSET + (grid->GetTileSize() * grid->GetCols()) / 2u - grid->GetTileSize() / 2u;
		const unsigned int RIGHT	= LEFT + grid->GetTileSize() - 1u;

		button_positions[4] = { TOP,BOTTOM,LEFT,RIGHT };
		button_positions[5] = { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

		button_blocks[4] = { button_positions[4], button_textures[2] };
		button_blocks[5] = { button_positions[5], button_textures[2] };
	}
}
void Minesweeper::InitialiseTimer()
{
	const unsigned int TOP = border_position.top + OFFSET * 1u + OFFSET * 2u - grid->GetTileSize() / 2u;
	const unsigned int RIGHT = OFFSET + grid->GetTileSize() * grid->GetCols();

	for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
	{
		for (unsigned int x = 0u; x < DIGIT_COLS; x++)
		{
			const RectUI POSITION = {
				TOP,
				TOP + grid->GetTileSize() - 1u,
				RIGHT - (grid->GetTileSize() * x) - grid->GetTileSize(),
				RIGHT - (grid->GetTileSize() * x) - 1u };

			timer[y][x] = std::move(Block(POSITION, digit_textures[y]));
		}
	}
}
void Minesweeper::InitialiseGameOver()
{
	if (grid->GetRows() <= grid->GetCols())
	{
		const unsigned int SIZE		= (grid->GetTileSize() * grid->GetRows()) / 2u;
		const unsigned int TOP		= border_position.top + OFFSET * 6u + (grid->GetTileSize() * grid->GetRows()) / 2u - SIZE;
		const unsigned int BOTTOM	= border_position.top + OFFSET * 6u + (grid->GetTileSize() * grid->GetRows()) / 2u + SIZE;
		const unsigned int LEFT		= (OFFSET + grid->GetTileSize() * grid->GetCols() + OFFSET) / 2u - SIZE;
		const unsigned int RIGHT	= (OFFSET + grid->GetTileSize() * grid->GetCols() + OFFSET) / 2u + SIZE;

		gameover_position = { TOP,BOTTOM,LEFT,RIGHT };
	}
	else
	{
		const unsigned int SIZE		= (grid->GetTileSize() * grid->GetCols()) / 2u;
		const unsigned int TOP		= border_position.top + OFFSET * 6u + (grid->GetTileSize() * grid->GetRows()) / 2u - SIZE;
		const unsigned int BOTTOM	= border_position.top + OFFSET * 6u + (grid->GetTileSize() * grid->GetRows()) / 2u + SIZE;
		const unsigned int LEFT		= (OFFSET + grid->GetTileSize() * grid->GetCols() + OFFSET) / 2u - SIZE;
		const unsigned int RIGHT	= (OFFSET + grid->GetTileSize() * grid->GetCols() + OFFSET) / 2u + SIZE;

		gameover_position = { TOP,BOTTOM,LEFT,RIGHT };
	}

	// EXPLOSION ANIMATION
	for (unsigned int i = 0; i < EXPLOSION_FRAMES; i++)
	{
		explosion_blocks[i] = { gameover_position, explosion_textures[i] };
	}	

	// FLAG ANIMATION
	for (unsigned int i = 0; i < FLAG_FRAMES; i++)
	{
		flag_blocks[i] = { gameover_position, flag_textures[i] };
	}	
}

/*--------------------------------------------*/

void Minesweeper::SetSettings(Mouse& mouse)
{
	for (auto& s : settings_blocks)
	{
		s.SetMouseOver(mouse);
	}

	settings_confirmation.SetMouseOver(mouse);

	if (mouse.IsInWindow())
	{
		if (!mouse_pressed)
		{
			if (settings_blocks[0].GetMouseOver())
			{
				if (mouse.LeftIsPressed())
				{
					is_beginner = true;
					is_intermediate = false;
					is_advanced = false;

					settings_blocks[0].SetTexture(settings_textures[1]);
					settings_blocks[1].SetTexture(settings_textures[0]);
					settings_blocks[2].SetTexture(settings_textures[0]);

					sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
					mouse_pressed = true;
				}
			}
			else if (settings_blocks[1].GetMouseOver())
			{
				if (mouse.LeftIsPressed())
				{
					is_beginner = false;
					is_intermediate = true;
					is_advanced = false;
					
					settings_blocks[0].SetTexture(settings_textures[0]);
					settings_blocks[1].SetTexture(settings_textures[1]);
					settings_blocks[2].SetTexture(settings_textures[0]);

					sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
					mouse_pressed = true;
				}
			}
			else if (settings_blocks[2].GetMouseOver())
			{
				if (mouse.LeftIsPressed())
				{
					is_beginner = false;
					is_intermediate = false;
					is_advanced = true;

					settings_blocks[0].SetTexture(settings_textures[0]);
					settings_blocks[1].SetTexture(settings_textures[0]);
					settings_blocks[2].SetTexture(settings_textures[1]);

					sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
					mouse_pressed = true;
				}
			}
			else
			{
				if (settings_confirmation.GetMouseOver())
				{
					if (mouse.LeftIsPressed())
					{
						if (is_beginner)
						{
							grid = std::make_unique<Grid>(COLS_BEGINNER, ROWS_BEGINNER, MINES_BEGINNER, OFFSET);
							sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
							Setup();
						}
						else if (is_intermediate)
						{
							grid = std::make_unique<Grid>(COLS_INTERMEDIATE, ROWS_INTERMEDIATE, MINES_INTERMEDIATE, OFFSET);
							sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
							Setup();
						}
						else if (is_advanced)
						{
							grid = std::make_unique<Grid>(COLS_ADVANCED, ROWS_ADVANCED, MINES_ADVANCED, OFFSET);
							sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
							Setup();
						}
						else
						{

						}

						mouse_pressed = true;
					}
				}
			}
		}
		else
		{
			if (!mouse.LeftIsPressed() && !mouse.RightIsPressed())
			{
				mouse_pressed = false;
			}
		}
	}
}
void Minesweeper::SetButtons(Mouse& mouse)
{
	button_blocks[BUTTONS::HELP].SetMouseOver(mouse);
	button_blocks[BUTTONS::SETTINGS].SetMouseOver(mouse);
	button_blocks[BUTTONS::RESET].SetMouseOver(mouse);

	for (unsigned int i = 0u; i < BUTTONS_NUM; i++)
	{
		if (!mouse_pressed)
		{
			const bool MOUSEOVER = button_blocks[static_cast<size_t>(i) * 2u].GetMouseOver();

			if (MOUSEOVER && mouse.LeftIsPressed())
			{
				sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);

				switch (i)
				{
				case 0:
					// help
					is_help = true;
					break;
				case 1:
					// settings
					is_settings = true;
					is_beginner = false;
					is_intermediate = false;
					is_advanced = false;
					grid.reset(nullptr);
					break;
				case 2:
					Reset();
					break;
				default:
					break;
				}

				button_pressed[i] = true;
				mouse_pressed = true;
			}
			else
			{
				button_pressed[i] = false;
			}
		}
		else
		{
			if (!mouse.LeftIsPressed() && !mouse.RightIsPressed())
			{
				mouse_pressed = false;
			}
		}
		
	}
}
void Minesweeper::SetBlocks(Mouse& mouse)
{
	if (!gameover && mouse.IsInWindow())
	{
		for (unsigned int i = 0u; i < grid->GetGridSize(); i++)
		{
			grid->SetMouseOver(i,mouse);
		}
		
		int i = 0;

		for (unsigned int y = 0u; y < grid->GetRows(); y++)
		{
			for (unsigned int x = 0u; x < grid->GetCols(); x++)
			{
				i = y * grid->GetCols() + x;

				if (grid->GetMouseOver(i))
				{
					if (!mouse_pressed)
					{
						if (mouse.LeftIsPressed())
						{
							if (!grid->GetIsRevealed(i) && !grid->GetIsFlag(i))
							{
								sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);

								//grid->blocks[i].SetTexture(grid->block_textures[grid->block_values[i]]);
								//grid->block_revealed[i] = true;

								grid->SetIsRevealed(i,true);

								/*if (grid->block_values[i] == 9u)
								{
									gameover = true;
									gamewon = false;
								}
								else if (grid->block_values[i] == 0u)
								{
									grid->RevealTiles(x, y, grid->COLS, grid->ROWS);
								}*/

								if (grid->GetValue(i) == 9u)
								{
									gameover = true;
									gamewon = false;
								}
								else if (grid->GetValue(i) == 0u)
								{
									grid->RevealTiles(x, y);
								}

								mouse_pressed = true;
							}
						}
						else if (mouse.RightIsPressed())
						{
							/*if (!grid->block_revealed[i])
							{
								if (grid->block_flag[i])
								{
									sounds[SOUNDS::CLICK_2].Play(1.0f, 1.0f);
									if (flags > 0) { flags--; }
									assert(flags >= 0u);

									if (mines < grid->MINES)
									{
										mines++;
										grid->blocks[i].SetTexture(grid->block_textures[TILE::TILE]);
										grid->block_flag[i] = false;
									}
									assert(mines <= grid->MINES);
								}
								else
								{
									sounds[SOUNDS::CLICK_1].Play(1.0f, 1.0f);
									if (flags < grid->MINES) { flags++; }
									assert(flags <= grid->MINES);

									if (mines > 0u)
									{
										mines--;
										grid->blocks[i].SetTexture(grid->block_textures[TILE::FLAG]);
										grid->block_flag[i] = true;
									}
									assert(mines >= 0u);
								}
							}*/
							if (!grid->GetIsRevealed(i))
							{
								if (grid->GetIsFlag(i))
								{
									sounds[SOUNDS::CLICK_2].Play(1.0f, 1.0f);
									if (flags > 0) { flags--; }
									assert(flags >= 0u);

									if (mines < grid->GetMines())
									{
										mines++;
										grid->SetIsFlag( i,false );
									}
									assert(mines <= grid->GetMines());
								}
								else
								{
									sounds[SOUNDS::CLICK_1].Play(1.0f, 1.0f);
									if (flags < grid->GetMines()) { flags++; }
									assert(flags <= grid->GetMines());

									if (mines > 0u)
									{
										mines--;
										grid->SetIsFlag( i,true );
									}
									assert(mines >= 0u);
								}
							}
							mouse_pressed = true;
						}

						if (mouse_pressed && !timer_started)
						{
							timer_started = true;
							t1 = std::chrono::high_resolution_clock::now();
						}
					}
					else
					{
						if (!mouse.LeftIsPressed() && !mouse.RightIsPressed())
						{
							mouse_pressed = false;
						}
					}
				}
			}
		}
	}
}
void Minesweeper::SetGameOver()
{
	if (!gameover)
	{
		unsigned int count_mines_and_flags = 0u;
		unsigned int count_revealed_blocks = 0u;

		for (unsigned int i = 0u; i < grid->GetGridSize(); i++)
		{
			if (grid->GetValue(i) == 9u && grid->GetIsFlag(i))
			{
				count_mines_and_flags++;
			}

			if (grid->GetIsRevealed(i))
			{
				count_revealed_blocks++;
			}
		}

		if (count_mines_and_flags == grid->GetMines() && count_revealed_blocks == grid->GetGridSize() - grid->GetMines())
		{
			gameover = true;
			gamewon = true;
		}
	}

	if (gameover)
	{
		if (gamewon)
		{
			if (!sound_played)
			{
				sounds[1].Play(1.0f, 1.0f);
				sound_played = true;
			}
			button_blocks[4].SetTexture(button_textures[3]);
			button_blocks[5].SetTexture(button_textures[3]);
		}
		else
		{
			if (!sound_played)
			{
				sounds[0].Play(1.0f, 1.0f);
				sound_played = true;
			}
			button_blocks[4].SetTexture(button_textures[4]);
			button_blocks[5].SetTexture(button_textures[4]);

			for (unsigned int i = 0u; i < grid->GetGridSize(); i++)
			{
				if (!grid->GetIsRevealed(i) && grid->GetValue(i) == 9u)
				{
					grid->SetIsRevealed(i, true);
				}
			}
		}
	}
}
void Minesweeper::SetMinesCounter()
{
	ExtractDigits(mines_number, mines);
	while (mines_number.size() < 3u){ mines_number.emplace_back(0u);	}
}
void Minesweeper::SetTimer()
{
	if (timer_started)
	{
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
		time = static_cast<unsigned int>(duration.count()) / 1000000u;
	}

	if (time > 999) { time = 0u; t1 = std::chrono::high_resolution_clock::now(); }

	ExtractDigits(timer_number, time);
	
	while (timer_number.size() < 3u)
	{
		timer_number.emplace_back(0u);
	}
}

/*--------------------------------------------*/

void Minesweeper::ExtractDigits(std::vector<unsigned int>& vec, const unsigned int& NUM)
{
	vec.clear();
	unsigned int result = NUM;

	if(result > 0)
	{
		while(result > 0u)
		{
			vec.push_back(result % 10u);
			result /= 10u;
		}
	}
	else
	{
		vec.push_back(0u);
	}
}

/*--------------------------------------------*/

void Minesweeper::DrawBorder(Graphics& gfx)
{
	for(auto b : border_blocks)
	{
		b.Draw(gfx);
	}
}
void Minesweeper::DrawDisplayBackground(Graphics& gfx)
{
	const unsigned int TOP		= border_position.top + OFFSET;
	const unsigned int BOTTOM	= border_position.top + OFFSET * 5u - 1u;
	const unsigned int LEFT		= OFFSET;
	const unsigned int RIGHT	= OFFSET + grid->GetTileSize() * grid->GetCols() - 1u;

	const RectUI POSITION = RectUI(TOP, BOTTOM, LEFT, RIGHT);

	gfx.DrawRect(true, POSITION, Color(155, 155, 155));
}
void Minesweeper::DrawMinesCounter(Graphics& gfx)
{	
	for (unsigned int i = 0u; i < mines_number.size(); i++)
	{
		mines_counter[mines_number[i]][i].Draw(gfx);
	}	
}
void Minesweeper::DrawButtons(Graphics& gfx)
{
	for (unsigned int i = 0u; i < 3u; i++)
	{
		if (button_pressed[i])
		{
			button_blocks[static_cast<size_t>(i) * 2u + 1u].Draw(gfx);
		}
		else
		{
			button_blocks[static_cast<size_t>(i) * 2u].Draw(gfx);
		}
	}
}
void Minesweeper::DrawTimer(Graphics& gfx)
{
	for (unsigned int i = 0u; i < timer_number.size(); i++)
	{
		timer[timer_number[i]][i].Draw(gfx);
	}
}
void Minesweeper::DrawGameOver(Graphics& gfx)
{	
	if (gameover)
	{		
		if (gamewon)
		{
			if (index0 < FLAG_FRAMES)
			{
				flag_blocks[index0].Draw(gfx);
			}
			index0++;
		}
		else
		{
			if (index1 < EXPLOSION_FRAMES)
			{
				explosion_blocks[index1].Draw(gfx);
			}
			index1++;
		}
	}
}
void Minesweeper::DrawHelp(Graphics& gfx)
{
	if (is_help)
	{
		help_block.Draw(gfx);
	}
}
void Minesweeper::DrawSettings(Graphics& gfx)
{
	if (is_settings)
	{
		gfx.DrawRect(true, SCREEN_RECT, Colors::Cyan);
				
		for (auto s : settings_blocks)
		{
			s.Draw(gfx);
		}
		
		settings_confirmation.Draw(gfx);
	}
}



/*--------------------------------------------*/


