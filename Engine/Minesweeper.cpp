#include "Minesweeper.h"

Minesweeper::Minesweeper()
{	
	InitialiseSounds();
	InitialiseTextures();
	InitialiseHelp();
	InitialiseSettings();
}

/*--------------------------------------------*/

void Minesweeper::InitialiseSounds()
{
	sounds.emplace_back(L"Sounds\\explosion.wav");
	sounds.emplace_back(L"Sounds\\fanfare.wav");
	sounds.emplace_back(L"Sounds\\click_0.wav");
	sounds.emplace_back(L"Sounds\\click_1.wav");
	sounds.emplace_back(L"Sounds\\click_2.wav");
	sounds.emplace_back(L"Sounds\\click_3.wav");
	win_sounds.emplace_back(L"Sounds\\cheer.wav");
	win_sounds.emplace_back(L"Sounds\\fanfare.wav");
}
void Minesweeper::InitialiseTextures()
{
	InitialiseSettingsTextures();
	InitialiseHelpTextures();
	InitialiseBorderTextures();
	InitialiseDigitTextures();
	InitialiseButtonTextures();
	InitialiseGameOverTextures();
}

/*--------------------------------------------*/

void Minesweeper::InitialiseSettingsTextures()
{
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\Settings.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\Easy.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\Medium.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\Hard.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\Custom.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\Play.png"));
	settings_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Settings\\button.png"));
}
void Minesweeper::InitialiseHelpTextures()
{
	help_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Help\\mouse.png")));
	help_button_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Help\\button_exit.png")));
	help_button_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Help\\button_exit_mouseover.png")));
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
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_0.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_1.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_2.png"));
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

void Minesweeper::InitialiseHelp()
{
	{
		const unsigned int SIZE = 300u;
		const unsigned int TOP = SCREEN_H / 2u - SIZE / 2u;
		const unsigned int BOTTOM = TOP + SIZE - 1u;
		const unsigned int LEFT = SCREEN_W / 2u - SIZE / 2u;
		const unsigned int RIGHT = LEFT + SIZE - 1u;

		help_block = { {TOP,BOTTOM,LEFT,RIGHT}, help_textures[0] };
	}

	{
		const unsigned int SIZE = 50u;
		const unsigned int TOP = SIZE / 2u;
		const unsigned int BOTTOM = TOP + SIZE - 1u;
		const unsigned int LEFT = SCREEN_W - 1u - SIZE - SIZE / 2u;
		const unsigned int RIGHT = LEFT + SIZE;

		help_button_block = { {TOP,BOTTOM,LEFT,RIGHT}, help_button_textures[0] };
	}
}
void Minesweeper::InitialiseSettings()
{
	const unsigned int TEXT_SIZE = 50u;

	{
		constexpr unsigned int TITLE_W = TEXT_SIZE * 8u;
		constexpr unsigned int TITLE_H = TEXT_SIZE;

		const Veiu2 TITLE_POSITION = { SCREEN_W / 2u,TITLE_H };
		// SETTINGS TEXT

		const unsigned int TOP = TITLE_POSITION.y - TITLE_H / 2u;
		const unsigned int BOTTOM = TOP + TITLE_H;
		const unsigned int LEFT = TITLE_POSITION.x - TITLE_W / 2u;
		const unsigned int RIGHT = LEFT + TITLE_W;

		//const RectUI RECT = { TOP,BOTTOM,LEFT,RIGHT };
		//settings_text_blocks[0] = { RECT, settings_textures[0] };

		settings_text_blocks[0] = { { TOP,BOTTOM,LEFT,RIGHT }, settings_textures[0] };
	}

	constexpr unsigned int TEXT_W = TEXT_SIZE * 6u;
	constexpr unsigned int TEXT_H = TEXT_SIZE;

	const unsigned int BUTTON_W = TEXT_W + TEXT_SIZE / 2u;
	const unsigned int BUTTON_H = TEXT_SIZE + TEXT_SIZE / 2u;

	const unsigned int OFFSET_W = TEXT_SIZE / 2u;
	const unsigned int OFFSET_H = TEXT_SIZE + TEXT_SIZE / 2u + TEXT_SIZE / 4u;

	const Veiu2 BUTTON_POSITION = { SCREEN_W / 2u, 150u };

	for (int i = 1; i < settings_text_blocks.size(); i++)
	{
		const unsigned int TOP = BUTTON_POSITION.y + OFFSET_H * (i - 1u) - TEXT_SIZE / 2u;
		const unsigned int BOTTOM = TOP + TEXT_SIZE - 1u;
		const unsigned int LEFT = BUTTON_POSITION.x - TEXT_SIZE * 3u;
		const unsigned int RIGHT = LEFT + TEXT_SIZE * 6u - 1u;

		//const RectUI RECT = { TOP,BOTTOM,LEFT,RIGHT };

		settings_text_blocks[i] = Block({ TOP,BOTTOM,LEFT,RIGHT }, settings_textures[i]);
	}

	for (int i = 0; i < settings_blocks.size(); i++)
	{
		const unsigned int TOP = BUTTON_POSITION.y + (OFFSET_H * i) - (BUTTON_H / 2u);
		const unsigned int BOTTOM = TOP + BUTTON_H - 1u;
		const unsigned int LEFT = BUTTON_POSITION.x - BUTTON_W / 2u;
		const unsigned int RIGHT = LEFT + BUTTON_W - 1u;

		//const RectUI RECT = { TOP,BOTTOM,LEFT,RIGHT };

		settings_blocks[i] = Block({ TOP,BOTTOM,LEFT,RIGHT }, settings_textures[6]);
	}

	for (size_t i = 0; i < is_selected.size(); i++)
	{
		is_selected[i] = false;
	}
}

/*--------------------------------------------*/

void Minesweeper::InitialiseBorder()
{
	border_blocks.clear();

	{
		const unsigned int TOP = 0u;
		const unsigned int BOTTOM = OFFSET * 6 + grid->TileSize() * grid->Rows() + OFFSET - 1u;
		const unsigned int LEFT = 0u;
		const unsigned int RIGHT = OFFSET + grid->TileSize() * grid->Cols() + OFFSET - 1u;;

		border_rect = RectUI(TOP, BOTTOM, LEFT, RIGHT);
	}

	// Top Horizontal
	RectUI corner_top_left{
		border_rect.top,
		border_rect.top + OFFSET,
		border_rect.left,
		border_rect.left + OFFSET };

	RectUI top{
		border_rect.top,
		border_rect.top + OFFSET,
		border_rect.left + OFFSET,
		border_rect.right - OFFSET };

	RectUI corner_top_right{
		border_rect.top,
		border_rect.top + OFFSET,
		border_rect.right - OFFSET,
		border_rect.right };

	border_blocks.emplace_back(corner_top_left, border_textures[BORDER::CORNER_TOP_LEFT]);
	border_blocks.emplace_back(top, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(corner_top_right, border_textures[BORDER::CORNER_TOP_RIGHT]);

	// Middle Horizontal
	RectUI t_left{
		border_rect.top + OFFSET * 5u,
		border_rect.top + OFFSET * 6u,
		border_rect.left,
		border_rect.left + OFFSET };

	RectUI middle{
		border_rect.top + OFFSET * 5u,
		border_rect.top + OFFSET * 6u,
		border_rect.left + OFFSET,
		border_rect.right - OFFSET };

	RectUI t_right{
		border_rect.top + OFFSET * 5u,
		border_rect.top + OFFSET * 6u,
		border_rect.right - OFFSET,
		border_rect.right };

	border_blocks.emplace_back(t_left, border_textures[BORDER::T_LEFT]);
	border_blocks.emplace_back(middle, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(t_right, border_textures[BORDER::T_RIGHT]);

	// Bottom Horizontal
	RectUI corner_bottom_left{
		border_rect.bottom - OFFSET,
		border_rect.bottom,
		border_rect.left,
		border_rect.left + OFFSET };

	RectUI bottom{
		border_rect.bottom - OFFSET,
		border_rect.bottom,
		border_rect.left + OFFSET,
		border_rect.right - OFFSET };

	RectUI corner_bottom_right{
		border_rect.bottom - OFFSET,
		border_rect.bottom,
		border_rect.right - OFFSET,
		border_rect.right };

	border_blocks.emplace_back(corner_bottom_left, border_textures[BORDER::CORNER_BOTTOM_LEFT]);
	border_blocks.emplace_back(bottom, border_textures[BORDER::HORIZONTAL]);
	border_blocks.emplace_back(corner_bottom_right, border_textures[BORDER::CORNER_BOTTOM_RIGHT]);

	// Verticle Left
	RectUI left_top{
		border_rect.top + OFFSET,
		border_rect.top + OFFSET * 5,
		border_rect.left,
		border_rect.left + OFFSET };

	RectUI left_bottom{
		border_rect.top + OFFSET * 6u,
		border_rect.bottom - OFFSET,
		border_rect.left,
		border_rect.left + OFFSET };

	border_blocks.emplace_back(left_top, border_textures[BORDER::VERTICLE]);
	border_blocks.emplace_back(left_bottom, border_textures[BORDER::VERTICLE]);

	// Verticle Right
	RectUI right_top{
		border_rect.top + OFFSET,
		border_rect.top + OFFSET * 5,
		border_rect.right - OFFSET,
		border_rect.right };

	RectUI right_bottom{
		border_rect.top + OFFSET * 6u,
		border_rect.bottom - OFFSET,
		border_rect.right - OFFSET,
		border_rect.right };

	border_blocks.emplace_back(right_top, border_textures[BORDER::VERTICLE]);
	border_blocks.emplace_back(right_bottom, border_textures[BORDER::VERTICLE]);
}
void Minesweeper::InitialiseMinesCounter()
{
	const unsigned int TOP = border_rect.top + OFFSET;
	const unsigned int RIGHT = OFFSET + OFFSET * 4u * 3u;

	for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
	{
		for (unsigned int x = 0u; x < DIGIT_COLS; x++)
		{
			const RectUI RECT = {
				TOP,
				TOP + OFFSET * 4u - 1u,
				RIGHT - (OFFSET * 4u * x) - OFFSET * 4u,
				RIGHT - (OFFSET * 4u * x) - 1u };

			mines_counter[y][x] = std::move(Block(RECT, digit_textures[y]));
		}
	}
}
void Minesweeper::InitialiseButtons()
{
	{
		const unsigned int TOP = border_rect.top + OFFSET * 1u;
		const unsigned int BOTTOM = TOP + OFFSET * 4u - 1u;
		const unsigned int LEFT = OFFSET + (grid->TileSize() * grid->Cols()) / 2u - OFFSET * 6u;
		const unsigned int RIGHT = LEFT + OFFSET * 4u - 1u;

		button_rects[0] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[1] = { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

		button_blocks[0] = { button_rects[0], button_textures[0] };
		button_blocks[1] = { button_rects[1], button_textures[0] };
	}

	{
		const unsigned int TOP = border_rect.top + OFFSET * 1u;
		const unsigned int BOTTOM = TOP + OFFSET * 4u - 1u;
		const unsigned int LEFT = OFFSET + (grid->TileSize() * grid->Cols()) / 2u + OFFSET * 2u;
		const unsigned int RIGHT = LEFT + OFFSET * 4u - 1u;

		button_rects[2] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[3] = { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

		button_blocks[2] = { button_rects[2], button_textures[1] };
		button_blocks[3] = { button_rects[3], button_textures[1] };
	}

	{
		const unsigned int TOP = border_rect.top + OFFSET * 1u;
		const unsigned int BOTTOM = TOP + OFFSET * 4u - 1u;
		const unsigned int LEFT = OFFSET + (grid->TileSize() * grid->Cols()) / 2u - OFFSET * 2u;
		const unsigned int RIGHT = LEFT + OFFSET * 4u - 1u;

		button_rects[4] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[5] = { TOP + 5u, BOTTOM - 5u, LEFT + 5u, RIGHT - 5u };

		button_blocks[4] = { button_rects[4], button_textures[2] };
		button_blocks[5] = { button_rects[5], button_textures[2] };
	}
}
void Minesweeper::InitialiseTimer()
{
	const unsigned int TOP = border_rect.top + OFFSET * 1u;
	const unsigned int RIGHT = OFFSET + grid->TileSize() * grid->Cols();

	for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
	{
		for (unsigned int x = 0u; x < DIGIT_COLS; x++)
		{
			const RectUI RECT = {
				TOP,
				TOP + OFFSET * 4u - 1u,
				RIGHT - (OFFSET * 4u * x) - OFFSET * 4u,
				RIGHT - (OFFSET * 4u * x) - 1u };

			timer[y][x] = std::move(Block(RECT, digit_textures[y]));
		}
	}
}
void Minesweeper::InitialiseGameOver()
{
	if (grid->Rows() <= grid->Cols())
	{
		const unsigned int SIZE = (grid->TileSize() * grid->Rows()) / 2u;
		const unsigned int TOP = border_rect.top + OFFSET * 6u + (grid->TileSize() * grid->Rows()) / 2u - SIZE;
		const unsigned int BOTTOM = border_rect.top + OFFSET * 6u + (grid->TileSize() * grid->Rows()) / 2u + SIZE;
		const unsigned int LEFT = (OFFSET + grid->TileSize() * grid->Cols() + OFFSET) / 2u - SIZE;
		const unsigned int RIGHT = (OFFSET + grid->TileSize() * grid->Cols() + OFFSET) / 2u + SIZE;

		gameover_rect = { TOP,BOTTOM,LEFT,RIGHT };
	}
	else
	{
		const unsigned int SIZE = (grid->TileSize() * grid->Cols()) / 2u;
		const unsigned int TOP = border_rect.top + OFFSET * 6u + (grid->TileSize() * grid->Rows()) / 2u - SIZE;
		const unsigned int BOTTOM = border_rect.top + OFFSET * 6u + (grid->TileSize() * grid->Rows()) / 2u + SIZE;
		const unsigned int LEFT = (OFFSET + grid->TileSize() * grid->Cols() + OFFSET) / 2u - SIZE;
		const unsigned int RIGHT = (OFFSET + grid->TileSize() * grid->Cols() + OFFSET) / 2u + SIZE;

		gameover_rect = { TOP,BOTTOM,LEFT,RIGHT };
	}

	// EXPLOSION ANIMATION
	for (unsigned int i = 0; i < EXPLOSION_FRAMES; i++)
	{
		explosion_blocks[i] = { gameover_rect, explosion_textures[i] };
	}

	// FLAG ANIMATION
	for (unsigned int i = 0; i < FLAG_FRAMES; i++)
	{
		flag_blocks[i] = { gameover_rect, flag_textures[i] };
	}
}

/*--------------------------------------------*/

void Minesweeper::SetSettings(Mouse& mouse)
{
	for (auto& s : settings_blocks)
	{
		s.SetMouseOver(mouse);
	}

	if (mouse.IsInWindow())
	{
		if (!mouse_pressed)
		{
			if (mouse.LeftIsPressed())
			{
				mouse_pressed = true;
				bool play_sound = false;

				if (settings_blocks[0].MouseOver() && !is_selected[0])
				{
					is_selected[0] = true;
					is_selected[1] = false;
					is_selected[2] = false;
					is_selected[3] = false;

					play_sound = true;
				}
				else if (settings_blocks[1].MouseOver() && !is_selected[1])
				{
					is_selected[0] = false;
					is_selected[1] = true;
					is_selected[2] = false;
					is_selected[3] = false;

					play_sound = true;
				}
				else if (settings_blocks[2].MouseOver() && !is_selected[2])
				{
					is_selected[0] = false;
					is_selected[1] = false;
					is_selected[2] = true;
					is_selected[3] = false;

					play_sound = true;
				}
				else if (settings_blocks[3].MouseOver() && !is_selected[3])
				{
					is_selected[0] = false;
					is_selected[1] = false;
					is_selected[2] = false;
					is_selected[3] = true;

					play_sound = true;
				}
				else if (settings_blocks[4].MouseOver())
				{
					if (is_selected[0])
					{
						grid = std::make_unique<Grid>(EASY::COLS, EASY::ROWS, EASY::MINES, OFFSET);
						Setup();

						play_sound = true;
					}
					else if (is_selected[1])
					{
						grid = std::make_unique<Grid>(MEDIUM::COLS, MEDIUM::ROWS, MEDIUM::MINES, OFFSET);
						Setup();

						play_sound = true;
					}
					else if (is_selected[2])
					{
						grid = std::make_unique<Grid>(HARD::COLS, HARD::ROWS, HARD::MINES, OFFSET);
						Setup();

						play_sound = true;
					}
					else if (is_selected[3])
					{

					}
					else
					{
						play_sound = false;
						sounds[SOUNDS::CLICK_3].Play(1.0f, 1.0f);
					}
				}

				if (play_sound)
				{
					sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);
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
void Minesweeper::SetHelp(Mouse& mouse)
{
	help_button_block.SetMouseOver(mouse);

	if (help_button_block.MouseOver())
	{
		help_button_block.SetTexture(help_button_textures[1]);

		if (mouse.LeftIsPressed())
		{
			is_help = false;
		}
	}
	else
	{
		help_button_block.SetTexture(help_button_textures[0]);
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
			const bool MOUSEOVER = button_blocks[static_cast<size_t>(i) * 2u].MouseOver();

			if (MOUSEOVER && mouse.LeftIsPressed())
			{
				sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);

				switch (i)
				{
				case 0:
					is_help = true;
					break;
				case 1:
					is_settings = true;

					for (size_t i = 0; i < is_selected.size(); i++)
					{
						is_selected[i] = false;
					}

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
void Minesweeper::SetGrid(Mouse& mouse)
{
	if (!gameover && mouse.IsInWindow())
	{
		int i = 0;

		for (; i < grid->Size(); i++)
		{
			grid->SetMouseOver(i, mouse);
			grid->SetChecked(i, false);
		}

		for (unsigned int y = 0u; y < grid->Rows(); y++)
		{
			for (unsigned int x = 0u; x < grid->Cols(); x++)
			{
				i = y * grid->Cols() + x;

				if (grid->MouseOver(i) && !mouse_pressed)
				{
					if (mouse.LeftIsPressed())
					{
						if (!grid->Revealed(i) && !grid->Flag(i))
						{
							sounds[SOUNDS::CLICK_0].Play(1.0f, 1.0f);

							grid->SetRevealed(i, true);

							if (grid->Value(i) == 9u)
							{
								gameover = true;
								gamewon = false;
							}
							else if (grid->Value(i) == 0u)
							{
								grid->RevealTiles(x, y);
							}
						}

						mouse_pressed = true;
					}
					else if (mouse.RightIsPressed())
					{
						if (!grid->Revealed(i))
						{
							if (grid->Flag(i))
							{
								sounds[SOUNDS::CLICK_2].Play(1.0f, 1.0f);
								if (flags > 0) { flags--; }
								assert(flags >= 0u);

								if (mines < grid->Mines())
								{
									mines++;
									grid->SetFlag(i, false);
								}
								assert(mines <= grid->Mines());
							}
							else
							{
								sounds[SOUNDS::CLICK_1].Play(1.0f, 1.0f);
								if (flags < grid->Mines()) { flags++; }
								assert(flags <= grid->Mines());

								if (mines > 0u)
								{
									mines--;
									grid->SetFlag(i, true);
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

		for (unsigned int y = 0u; y < grid->Rows(); y++)
		{
			for (unsigned int x = 0u; x < grid->Cols(); x++)
			{
				i = y * grid->Cols() + x;

				if (grid->MouseOver(i) && mouse.MiddleIsPressed())
				{
					if (grid->CheckTiles(x, y, true))
					{
						gameover = true;
						gamewon = false;
					}

					if (!checked_sound_played)
					{
						sounds[SOUNDS::CLICK_1].Play(1.0f, 1.0f);
						checked_sound_played = true;
					}
				}
				else if (!mouse.MiddleIsPressed())
				{
					checked_sound_played = false;
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

		for (unsigned int i = 0u; i < grid->Size(); i++)
		{
			if (grid->Value(i) == 9u && grid->Flag(i))
			{
				count_mines_and_flags++;
			}

			if (grid->Revealed(i))
			{
				count_revealed_blocks++;
			}
		}		

		if (count_mines_and_flags == grid->Mines() && count_revealed_blocks == grid->Size() - grid->Mines())
		{
			gameover = true;
			gamewon = true;
		}
	}

	if (gameover)
	{
		if (gamewon)
		{
			if (!gameover_sound_played)
			{
				//sounds[1].Play(1.0f, 1.0f);
				const size_t MIN = 0;
				const size_t MAX = win_sounds.size() - 1;
				const size_t INDEX = rnd::RandomInt(MIN, MAX);
				win_sounds[INDEX].Play(1.0f, 1.0f);
				gameover_sound_played = true;
			}
			button_blocks[4].SetTexture(button_textures[3]);
			button_blocks[5].SetTexture(button_textures[3]);
		}
		else
		{
			if (!gameover_sound_played)
			{
				sounds[0].Play(1.0f, 1.0f);
				gameover_sound_played = true;
			}
			button_blocks[4].SetTexture(button_textures[4]);
			button_blocks[5].SetTexture(button_textures[4]);

			for (unsigned int i = 0u; i < grid->Size(); i++)
			{
				grid->SetChecked(i, false);

				if (!grid->Revealed(i) && grid->Value(i) == 9u)
				{
					grid->SetRevealed(i, true);
				}
			}
			
			//grid->Update();
		}
	}
}
void Minesweeper::SetMinesCounter()
{
	ExtractDigits(mines_number, mines);
	while (mines_number.size() < 3u) { mines_number.emplace_back(0u); }
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

void Minesweeper::Update(Mouse& mouse)
{
	if (grid != nullptr)
	{
		if (is_help)
		{
			SetHelp(mouse);
		}
		else
		{
			SetButtons(mouse);

			if (grid != nullptr)
			{
				SetGrid(mouse);

				if (!gameover)
				{
					SetTimer();
					SetMinesCounter();
				}

				SetGameOver();

				grid->Update();
			}
		}		
	}
	
	if(grid == nullptr)
	{
		SetSettings(mouse);
	}
}
void Minesweeper::Draw(Graphics& gfx)
{
	if (grid != nullptr)
	{
		if (is_help)
		{
			DrawHelp(gfx);
		}
		else
		{
			DrawBorder(gfx);
			DrawDisplayBackground(gfx);

			DrawMinesCounter(gfx);
			DrawButtons(gfx);
			DrawTimer(gfx);
			grid->Draw(gfx);
			DrawGameOver(gfx);
		}
	}
	else
	{
		DrawSettings(gfx);
	}
}

/*--------------------------------------------*/

void Minesweeper::Setup()
{
	// custom: columns, rows, mines

	if (grid != nullptr)
	{
		mines = grid->Mines();

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
	gameover_sound_played	= false;
	is_help			= false;
	is_settings		= false;
	mines			= grid->Mines();
	flags			= 0u;
	time			= 0u;
	frames			= 0u;
	index0			= 0u;
	index1			= 0u;
		
	grid->SetTileValues();
	grid->SetBackground();
	
	button_blocks[4].SetTexture(button_textures[2]);
	button_blocks[5].SetTexture(button_textures[2]);
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
	const unsigned int TOP		= border_rect.top + OFFSET;
	const unsigned int BOTTOM	= border_rect.top + OFFSET * 5u - 1u;
	const unsigned int LEFT		= OFFSET;
	const unsigned int RIGHT	= OFFSET + grid->TileSize() * grid->Cols() - 1u;

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
		gfx.DrawRect(true, SCREEN_RECT, Colors::LightGray);
		help_block.Draw(gfx);
		help_button_block.Draw(gfx);
	}
}
void Minesweeper::DrawSettings(Graphics& gfx)
{
	if (is_settings)
	{
		Block background = { SCREEN_RECT, std::make_unique<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Blocks3DRainbow.png")) };
		background.Draw(gfx);

		Block mouseover;
		Block selected;
		selected.SetTexture(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Settings\\button_selected.png")));
		mouseover.SetTexture( std::make_shared<Surface>( Surface::FromFile(L"Textures\\Minesweeper\\Settings\\button_mouseover.png") ) );
		for (auto& s : settings_blocks)
		{
			s.Draw(gfx);

			if (s.MouseOver())
			{
				mouseover.SetPosition(s.Position());
				mouseover.Draw(gfx);
			}
		}

		for (int i = 0; i < is_selected.size(); i++)
		{
			if (is_selected[i])
			{
				selected.SetPosition(settings_blocks[i].Position());
				selected.Draw(gfx);
			}
		}

		for (auto& s : settings_text_blocks)
		{			
			s.Draw(gfx);
		}
	}
}