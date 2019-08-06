#include "Minesweeper.h"

Minesweeper::Minesweeper()
{	
	InitialiseSounds();
	InitialiseTextures();
	InitialiseHelp();
	InitialiseDisplay();
	InitialiseGameOver();

	is_selected[0] = true;
	is_selected[1] = false;
	is_selected[2] = false;
	is_selected[3] = false;

	grid = std::make_unique<Grid>(
		grid_textures,
		tile_textures,
		LEVEL::EASY::COLS,
		LEVEL::EASY::ROWS,
		LEVEL::EASY::MINES,
		OFFSET);
	
	if (grid != nullptr)
	{
		mines = grid->Mines();
	}
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
	InitialiseGridTextures();
	InitialiseTileTextures();
	InitialiseDisplayTextures();
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
void Minesweeper::InitialiseGridTextures()
{
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature0.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature1.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature2.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature3.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature4.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Nature5.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Space1.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\BlocksBlue.png")));
	grid_textures.emplace_back(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\BlocksGreen.png")));
}
void Minesweeper::InitialiseTileTextures()
{
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_blank.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_1_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_2_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_3_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_4_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_5_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_mine.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_skull.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_flag.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_flag_wrong.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_flag_correct.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_mouseover.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_light.png"));
	tile_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_dark.png"));
}
void Minesweeper::InitialiseDisplayTextures()
{
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_background.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_divider.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_cols.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_rows.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_mines.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_easy.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_medium.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_hard.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_custom.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_timer.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_volume.png"));
	display_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\display_volume_mute.png"));
}
void Minesweeper::InitialiseDigitTextures()
{
	digit_textures[0] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_0_white.png");
	digit_textures[1] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_1_white.png");
	digit_textures[2] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_2_white.png");
	digit_textures[3] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_3_white.png");
	digit_textures[4] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_4_white.png");
	digit_textures[5] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_5_white.png");
	digit_textures[6] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_6_white.png");
	digit_textures[7] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_7_white.png");
	digit_textures[8] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_8_white.png");
	digit_textures[9] = std::make_shared<Surface>(L"Textures\\Minesweeper\\Digits\\digit_9_white.png");
}
void Minesweeper::InitialiseButtonTextures()
{
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\help.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\settings.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_smile.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_win.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_lose.png"));
	button_textures.emplace_back(std::make_shared<Surface>(L"Textures\\Minesweeper\\Tiles\\tile_smiley_gasp.png"));
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
		
		settings_text_blocks[i] = Block({ TOP,BOTTOM,LEFT,RIGHT }, settings_textures[i]);
	}

	for (int i = 0; i < settings_blocks.size(); i++)
	{
		const unsigned int TOP = BUTTON_POSITION.y + (OFFSET_H * i) - (BUTTON_H / 2u);
		const unsigned int BOTTOM = TOP + BUTTON_H - 1u;
		const unsigned int LEFT = BUTTON_POSITION.x - BUTTON_W / 2u;
		const unsigned int RIGHT = LEFT + BUTTON_W - 1u;

		settings_blocks[i] = Block({ TOP,BOTTOM,LEFT,RIGHT }, settings_textures[6]);
	}

	for (size_t i = 0; i < is_selected.size(); i++)
	{
		is_selected[i] = false;
	}
}

/*--------------------------------------------*/

void Minesweeper::InitialiseDisplay()
{
	InitialiseDisplayBackground();
	InitialiseButtons();
	InitialiseMinesCounter();
	InitialiseTimer();
	InitialiseDisplayButtons();
	InitialiseDisplayDividers();
}

void Minesweeper::InitialiseDisplayBackground()
{
	const unsigned int TOP		= OFFSET / 2u;
	const unsigned int BOTTOM	= TOP + DIGIT_H + OFFSET - 1u;
	const unsigned int LEFT		= SCREEN_RECT.left + OFFSET / 2u;
	const unsigned int RIGHT	= SCREEN_RECT.right - OFFSET / 2u + 1u;

	display_rect = { TOP,BOTTOM,LEFT,RIGHT };

	display_background_block = { display_rect,display_textures[0] };	
}
void Minesweeper::InitialiseMinesCounter()
{
	const unsigned int TOP = OFFSET;
	const unsigned int LEFT = OFFSET + DIGIT_W * 2u;

	RectUI rect;

	for (unsigned int x = 0u; x < DIGIT_COLS; x++)
	{
		for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
		{
			rect = {
				TOP,
				TOP + DIGIT_H - 1u,
				LEFT - (DIGIT_W * x),
				LEFT - (DIGIT_W * x) + DIGIT_W - 1u };

			mine_counter[y][x] = { rect, digit_textures[y] };
		}
		mine_counter_rects[x] = rect;
	}

	mine_counter_rect = { mine_counter_rects[0].top,mine_counter_rects[0].bottom,mine_counter_rects[2].left,mine_counter_rects[0].right };
}
void Minesweeper::InitialiseButtons()
{
	const unsigned int OFF = 5u;

	{
		// HELP BUTTON

		const unsigned int TOP = OFFSET * 1u;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = SCREEN_W / 2u - OFFSET * 6u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[0] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[1] = { TOP + OFF, BOTTOM - OFF, LEFT + OFF, RIGHT - OFF };

		button_blocks[0] = { button_rects[0], button_textures[0] };
		button_blocks[1] = { button_rects[1], button_textures[1] };
	}

	{
		// SETTINGS BUTTON

		const unsigned int TOP = OFFSET * 1u;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = SCREEN_W / 2u + OFFSET * 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[2] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[3] = { TOP + OFF, BOTTOM - OFF, LEFT + OFF, RIGHT - OFF };

		button_blocks[BUTTON::SETTINGS::RELEASE] = 
			{ button_rects[BUTTON::SETTINGS::RELEASE], button_textures[BUTTON::SETTINGS::TEXTURE] };
		button_blocks[BUTTON::SETTINGS::PRESSED] = 
			{ button_rects[BUTTON::SETTINGS::PRESSED], button_textures[BUTTON::SETTINGS::TEXTURE] };
	}

	{
		// RESET BUTTON

		const unsigned int TOP = OFFSET * 1u;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = SCREEN_W / 2u - OFFSET * 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[4] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[5] = { TOP + OFF, BOTTOM - OFF, LEFT + OFF, RIGHT - OFF };

		button_blocks[4] = { button_rects[4], button_textures[2] };
		button_blocks[5] = { button_rects[5], button_textures[2] };
	}

	//const unsigned int TOP		= OFFSET * 1u;
	//const unsigned int BOTTOM	= TOP + DIGIT_H - 1u;
	//const unsigned int LEFT		= SCREEN_W / 2u - DIGIT_W / 2u - DIGIT_W;
	//const unsigned int RIGHT	= LEFT + DIGIT_W - 1u;
	//
	//unsigned int j = 0;
	//size_t k = 0;
	//
	//for (unsigned int i = 0u; i < button_blocks.size()/2u; i++)
	//{
	//	j = i * 2u;
	//	k = static_cast<size_t>(j) + 1u;
	//
	//	button_rects[j]	= { TOP,
	//						BOTTOM,
	//						LEFT + DIGIT_W * i,
	//						RIGHT + DIGIT_W * i };
	//
	//	button_rects[k]	= { TOP		+ OFF,
	//						BOTTOM	- OFF,
	//						LEFT	+ OFF + DIGIT_W * i,
	//						RIGHT	- OFF + DIGIT_W * i };
	//
	//	button_blocks[j] = { button_rects[j], button_textures[i] };
	//	button_blocks[k] = { button_rects[k], button_textures[i] };
	//}
}
void Minesweeper::InitialiseTimer()
{
	const unsigned int TOP = OFFSET;
	const unsigned int LEFT = SCREEN_RECT.right - OFFSET - DIGIT_W;

	RectUI rect;

	for (unsigned int x = 0u; x < DIGIT_COLS; x++)
	{
		for (unsigned int y = 0u; y < DIGIT_ROWS; y++)
		{
			rect = {
				TOP,
				TOP + DIGIT_H - 1u,
				LEFT - (DIGIT_W * x),
				LEFT - (DIGIT_W * x) + DIGIT_W - 1u	};

			timer_blocks[y][x] = { rect, digit_textures[y] };
		}
		timer_rects[x] = rect;
	}

	timer_rect = { timer_rects[0].top,timer_rects[0].bottom,timer_rects[2].left,timer_rects[0].right };
}
void Minesweeper::InitialiseDisplayButtons()
{
	{
		// DISPLAY BUTTON COLS

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = mine_counter_rect.right + OFFSET + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		display_button_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_button_blocks.emplace_back(display_button_rects[0], display_textures[2]);
	}

	{
		// DISPLAY BUTTON ROWS

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = display_button_rects[0].right + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		display_button_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_button_blocks.emplace_back(display_button_rects[1], display_textures[3]);
	}
	
	{
		// DISPLAY BUTTON MINES

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = display_button_rects[1].right + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		display_button_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_button_blocks.emplace_back(display_button_rects[2], display_textures[4]);
	}

	{
		// DISPLAY BUTTON LEVELS

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = display_button_rects[2].right + OFFSET + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		display_button_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_button_blocks.emplace_back(display_button_rects[3], display_textures[5]);
		display_button_blocks.emplace_back(display_button_rects[3], display_textures[6]);
		display_button_blocks.emplace_back(display_button_rects[3], display_textures[7]);
		display_button_blocks.emplace_back(display_button_rects[3], display_textures[8]);
	}

	{
		// TIMER ICON
		
		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = timer_rect.left - DIGIT_W - OFFSET - OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		display_button_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_button_blocks.emplace_back(display_button_rects[4], display_textures[9]);
	}

	{
		// VOLUME BUTTON

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = display_button_rects[4].left - DIGIT_W - OFFSET - OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		display_button_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_button_blocks.emplace_back(display_button_rects[5], display_textures[10]);
		display_button_blocks.emplace_back(display_button_rects[5], display_textures[11]);
	}
}
void Minesweeper::InitialiseDisplayDividers()
{
	const unsigned int DIVIDER_W = (DIGIT_W + OFFSET / 2u) / 6u;

	{
		// BETWEEN MINES COUNTER & COLS BUTTON 

		const unsigned int GAP = display_button_rects[DISPLAY::COLS].left - mine_counter_rect.right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = mine_counter_rect.right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		display_divider_rects.emplace_back( TOP,BOTTOM,LEFT,RIGHT );

		display_divider_blocks.emplace_back( display_divider_rects[0],display_textures[1] );
	}

	{
		// BETWEEN TIMER BUTTON & TIMER

		const unsigned int GAP = timer_rect.left - display_button_rects[DISPLAY::TIMER].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = display_button_rects[DISPLAY::TIMER].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		display_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_divider_blocks.emplace_back(display_divider_rects[1], display_textures[1]);
	}

	{
		// BETWEEN MINES BUTTON & LEVELS BUTTON

		const unsigned int GAP = display_button_rects[DISPLAY::LEVEL].left - display_button_rects[DISPLAY::MINES].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = display_button_rects[DISPLAY::MINES].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		display_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_divider_blocks.emplace_back(display_divider_rects[2], display_textures[1]);
	}

	{
		// BETWEEN LEVELS BUTTON & HELP BUTTON 

		const unsigned int GAP = button_rects[BUTTON::HELP::RELEASE].left - display_button_rects[DISPLAY::LEVEL].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = display_button_rects[DISPLAY::LEVEL].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		display_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_divider_blocks.emplace_back(display_divider_rects[3], display_textures[1]);
	}


	{
		// VOLUME BUTTON & TIMER BUTTON

		const unsigned int GAP = display_button_rects[DISPLAY::TIMER].left - display_button_rects[DISPLAY::VOLUME].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = display_button_rects[DISPLAY::VOLUME].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		display_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		display_divider_blocks.emplace_back(display_divider_rects[4], display_textures[1]);
	}

	{
		// VOLUME BUTTON LEFT
	
		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = display_button_rects[DISPLAY::VOLUME].left - DIVIDER_W - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;
	
		display_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);
	
		display_divider_blocks.emplace_back(display_divider_rects[5], display_textures[1]);
	}
}

/*--------------------------------------------*/

void Minesweeper::InitialiseGameOver()
{
	const unsigned int TOP = OFFSET * 7u;
	const unsigned int BOTTOM = SCREEN_H - OFFSET - 1u;

	const unsigned int SIZE = BOTTOM - TOP;

	const unsigned int LEFT = SCREEN_W / 2u - SIZE / 2u;
	const unsigned int RIGHT = LEFT + SIZE - 1u;

	gameover_rect = { TOP,BOTTOM,LEFT,RIGHT };
	
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
						grid = std::make_unique<Grid>(
							grid_textures,
							tile_textures,
							LEVEL::EASY::COLS,
							LEVEL::EASY::ROWS,
							LEVEL::EASY::MINES,
							OFFSET);
						Setup();

						play_sound = true;
					}
					else if (is_selected[1])
					{
						grid = std::make_unique<Grid>(
							grid_textures,
							tile_textures,
							LEVEL::MEDIUM::COLS,
							LEVEL::MEDIUM::ROWS,
							LEVEL::MEDIUM::MINES,
							OFFSET);
						Setup();

						play_sound = true;
					}
					else if (is_selected[2])
					{
						grid = std::make_unique<Grid>(
							grid_textures,
							tile_textures,
							LEVEL::HARD::COLS,
							LEVEL::HARD::ROWS,
							LEVEL::HARD::MINES,
							OFFSET);
						Setup();

						play_sound = true;
					}
					else if (is_selected[3])
					{

					}
					else
					{
						play_sound = false;
						sounds[SOUNDS::CLICK_3].Play(1.0f, sound_fx_volume);
					}
				}

				if (play_sound)
				{
					sounds[SOUNDS::CLICK_0].Play(1.0f, sound_fx_volume);
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

void Minesweeper::SetGrid(Mouse& mouse)
{
	if (!gameover && mouse.IsInWindow())
	{
		size_t i = 0u;

		for (; i < grid->GridSize(); i++)
		{
			grid->SetMouseOver(i, mouse);
			grid->SetChecked(i, false);
		}

		for (unsigned int y = 0u; y < grid->Rows(); y++)
		{
			for (unsigned int x = 0u; x < grid->Cols(); x++)
			{
				i = static_cast<size_t>(y) * grid->Cols() + x;

				if (grid->MouseOver(i) && !mouse_pressed)
				{
					if (mouse.LeftIsPressed())
					{
						if (!grid->Revealed(i) && !grid->Flag(i))
						{
							sounds[SOUNDS::CLICK_0].Play(1.0f, sound_fx_volume);
							
							grid->SetRevealed(i, true);

							if (grid->Value(i) == 9u)
							{
								grid->SetExploded(i, true);
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
								sounds[SOUNDS::CLICK_2].Play(1.0f, sound_fx_volume);
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
								sounds[SOUNDS::CLICK_1].Play(1.0f, sound_fx_volume);
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
				i = static_cast<size_t>(y) * grid->Cols() + x;

				if (grid->MouseOver(i) && 
					(mouse.MiddleIsPressed() || 
					(mouse.LeftIsPressed() && mouse.RightIsPressed())))
				{

					button_blocks[4].SetTexture(button_textures[5]);
					button_blocks[5].SetTexture(button_textures[5]);

					if (grid->CheckTiles(x, y, true))
					{
						gameover = true;
						gamewon = false;
					}

					if (!checked_sound_played)
					{
						sounds[SOUNDS::CLICK_1].Play(1.0f, sound_fx_volume);
						checked_sound_played = true;
					}
				}
				else if (!mouse.MiddleIsPressed() && 
					!mouse.LeftIsPressed() &&
					!mouse.RightIsPressed())
				{
					checked_sound_played = false;
					
					button_blocks[4].SetTexture(button_textures[2]);
					button_blocks[5].SetTexture(button_textures[2]);
				}
			}
		}
	}
}

void Minesweeper::SetButtons(Mouse& mouse)
{
	for (size_t i = 0u; i < button_blocks.size(); i+=2)
	{
		button_blocks[i].SetMouseOver(mouse);
	}
	/*button_blocks[BUTTON::HELP::RELEASE].SetMouseOver(mouse);
	button_blocks[BUTTON::SETTINGS::RELEASE].SetMouseOver(mouse);
	button_blocks[BUTTON::RESET::RELEASE].SetMouseOver(mouse);*/

	for (unsigned int i = 0u; i < BUTTONS_NUM; i++)
	{
		if (!mouse_pressed)
		{
			const bool MOUSEOVER = button_blocks[static_cast<size_t>(i) * 2u].MouseOver();

			if (MOUSEOVER && mouse.LeftIsPressed())
			{
				sounds[SOUNDS::CLICK_0].Play(1.0f, sound_fx_volume);

				switch (i)
				{
				case 0:
					is_help = true;
					break;
				case 1:
					/*is_settings = true;

					for (size_t i = 0; i < is_selected.size(); i++)
					{
						is_selected[i] = false;
					}

					grid.reset(nullptr);*/
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
void Minesweeper::SetMinesCounter()
{
	ExtractDigits(mine_counter_value, mines);
	while (mine_counter_value.size() < 3u) { mine_counter_value.emplace_back(0u); }
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

	ExtractDigits(timer_value, time);

	while (timer_value.size() < 3u)
	{
		timer_value.emplace_back(0u);
	}
}

void Minesweeper::SetGameOver()
{
	if (!gameover)
	{
		unsigned int count_mines_and_flags = 0u;
		unsigned int count_revealed_blocks = 0u;

		for (size_t i = 0u; i < grid->GridSize(); i++)
		{
			if (grid->Mine(i) && grid->Flag(i))
			{
				count_mines_and_flags++;
			}

			if (grid->Revealed(i))
			{
				count_revealed_blocks++;
			}
		}		

		if (count_mines_and_flags == grid->Mines() &&
			count_revealed_blocks == grid->GridSize() - grid->Mines())
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
				const size_t MIN = 0;
				const size_t MAX = win_sounds.size() - 1;
				const size_t INDEX = rnd::RandomInt(MIN, MAX);
				win_sounds[INDEX].Play(1.0f, sound_fx_volume);
				gameover_sound_played = true;
			}
			button_blocks[4].SetTexture(button_textures[3]);
			button_blocks[5].SetTexture(button_textures[3]);
		}
		else
		{
			if (!gameover_sound_played)
			{
				sounds[0].Play(1.0f, sound_fx_volume);
				gameover_sound_played = true;
			}
			button_blocks[4].SetTexture(button_textures[4]);
			button_blocks[5].SetTexture(button_textures[4]);

			for (size_t i = 0u; i < grid->GridSize(); i++)
			{
				grid->SetChecked(i, false);

				if (grid->Mine(i) && !grid->Flag(i))
				{
					grid->SetRevealed(i, true);
				}
			}
		}
	}

	grid->SetGameOver(gameover);
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

void Minesweeper::DrawHelp(Graphics& gfx)
{
	gfx.DrawRect(true, SCREEN_RECT, Colors::LightGray);
	help_block.Draw(gfx);
	help_button_block.Draw(gfx);	
}
void Minesweeper::DrawSettings(Graphics& gfx)
{
	if (is_settings)
	{
		/*Block background = { SCREEN_RECT, std::make_unique<Surface>(Surface::FromFile(L"Textures\\Backgrounds\\Blocks3DRainbow.png")) };
		background.Draw(gfx);*/

		Block mouseover;
		Block selected;
		selected.SetTexture(std::make_shared<Surface>(Surface::FromFile(L"Textures\\Minesweeper\\Settings\\button_selected.png")));
		mouseover.SetTexture( std::make_shared<Surface>( Surface::FromFile(L"Textures\\Minesweeper\\Settings\\button_mouseover.png") ) );
		
		for (auto& s : settings_blocks)
		{
			s.Draw(gfx);

			if (s.MouseOver())
			{
				mouseover.SetRect(s.Rect());
				mouseover.Draw(gfx);
			}
		}

		for (int i = 0; i < is_selected.size(); i++)
		{
			if (is_selected[i])
			{
				selected.SetRect(settings_blocks[i].Rect());
				selected.Draw(gfx);
			}
		}

		for (auto& s : settings_text_blocks)
		{			
			s.Draw(gfx);
		}
	}
}

void Minesweeper::DrawDisplay(Graphics& gfx)
{
	DrawDisplayBackground(gfx);
	DrawMinesCounter(gfx);
	DrawButtons(gfx);
	DrawTimer(gfx);
	DrawDisplayButtons(gfx);
}

void Minesweeper::DrawDisplayBackground(Graphics& gfx)
{
	display_background_block.Draw(gfx);

	for (auto& d : display_divider_blocks)
	{
		d.Draw(gfx);
	}
}
void Minesweeper::DrawMinesCounter(Graphics& gfx)
{	
	for (unsigned int i = 0u; i < mine_counter_value.size(); i++)
	{
		mine_counter[mine_counter_value[i]][i].Draw(gfx);
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
	for (unsigned int i = 0u; i < timer_value.size(); i++)
	{
		timer_blocks[timer_value[i]][i].Draw(gfx);
	}
}
void Minesweeper::DrawDisplayButtons(Graphics& gfx)
{
	display_button_blocks[0].Draw(gfx);
	display_button_blocks[1].Draw(gfx);
	display_button_blocks[2].Draw(gfx);
	display_button_blocks[7].Draw(gfx);
	display_button_blocks[8].Draw(gfx);

	if (is_selected[0])
	{
		display_button_blocks[3].Draw(gfx);
	}
	else if (is_selected[1])
	{
		display_button_blocks[4].Draw(gfx);
	}
	else if (is_selected[2])
	{
		display_button_blocks[5].Draw(gfx);
	}
	else if (is_selected[3])
	{
		display_button_blocks[6].Draw(gfx);
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

/*--------------------------------------------*/

void Minesweeper::Setup()
{	
	InitialiseDisplay();
	InitialiseGameOver();

	timer_started = false;
	time = 0u;

	gameover = false;
	gamewon = false;
	gameover_sound_played = false;

	is_help = false;
	is_settings = false;

	flags = 0u;
	frames = 0u;
	index0 = 0u;
	index1 = 0u;

	button_blocks[4].SetTexture(button_textures[2]);
	button_blocks[5].SetTexture(button_textures[2]);

	if (grid != nullptr)
	{
		mines = grid->Mines();
	}
}
void Minesweeper::Reset()
{
	timer_started	= false;
	gameover		= false;
	gamewon			= false;
	is_help			= false;
	is_settings		= false;
	flags			= 0u;
	time			= 0u;
	frames			= 0u;
	index0			= 0u;
	index1			= 0u;
	
	gameover_sound_played	= false;

	button_blocks[4].SetTexture(button_textures[2]);
	button_blocks[5].SetTexture(button_textures[2]);

	if (grid != nullptr)
	{
		mines = grid->Mines();
	}

	grid->Reset();
}
/*--------------------------------------------*/

void Minesweeper::Update(Mouse& mouse)
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

	/*if (grid != nullptr)
	{
		if (is_help)
		{
			
		}
		else
		{
			

			
		}		
	}*/
	
	/*if(grid == nullptr)
	{
		SetSettings(mouse);
	}*/
}
void Minesweeper::Draw(Graphics& gfx)
{
	if (is_help)		
	{
		DrawHelp(gfx);
	}
	else
	{
		DrawDisplay(gfx);

		if (grid != nullptr)
		{
			grid->Draw(gfx);
		}

		DrawGameOver(gfx);
	}

	/*if (grid != nullptr)
	{
		
	}
	else
	{
		if (is_help)
		{
			DrawHelp(gfx);
		}
		else
		{
			DrawSettings(gfx);
		}
	}*/
}
