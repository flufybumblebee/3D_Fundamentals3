#include "Minesweeper.h"

#include "Bumble.h"

Minesweeper::Minesweeper()
{	
	InitialiseSounds();
	InitialiseTextures();
	InitialiseHelp();
	InitialiseDisplay();
	InitialiseGameOver();

	level = 0u;

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
	//InitialiseSettingsTextures();
	InitialiseHelpTextures();
	InitialiseGridTextures();
	InitialiseTileTextures();
	InitialiseDigitTextures();
	InitialiseButtonTextures();
	InitialiseGameOverTextures();
}

/*--------------------------------------------*/

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
	button_textures.assign(18, nullptr);
	
	/*  0 */ button_textures[BUTTON::TEXTURE::BACKGROUND]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\background.png") };
	/*  1 */ button_textures[BUTTON::TEXTURE::DIVIDER]			= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\divider.png") };
	 																														 
	/*  2 */ button_textures[BUTTON::TEXTURE::COLS]				= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\cols.png") };
	/*  3 */ button_textures[BUTTON::TEXTURE::ROWS]				= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\rows.png") };
	/*  4 */ button_textures[BUTTON::TEXTURE::MINES]			= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\mines.png") };
																															 
	/*  5 */ button_textures[BUTTON::TEXTURE::LEVEL_EASY]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\level_easy.png") };
	/*  6 */ button_textures[BUTTON::TEXTURE::LEVEL_MEDIUM]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\level_medium.png") };
	/*  7 */ button_textures[BUTTON::TEXTURE::LEVEL_HARD]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\level_hard.png") };
	/*  8 */ button_textures[BUTTON::TEXTURE::LEVEL_CUSTOM]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\level_custom.png") };
																															 
	/*  9 */ button_textures[BUTTON::TEXTURE::HELP]				= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\help.png") };
	/* 10 */ button_textures[BUTTON::TEXTURE::RESTART_SMILE]	= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\smiley_smile.png") };
	/* 11 */ button_textures[BUTTON::TEXTURE::RESTART_WIN]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\smiley_win.png") };
	/* 12 */ button_textures[BUTTON::TEXTURE::RESTART_LOSE]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\smiley_lose.png") };
	/* 13 */ button_textures[BUTTON::TEXTURE::RESTART_GASP]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\smiley_gasp.png") };
	/* 14 */ button_textures[BUTTON::TEXTURE::SETTINGS]			= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\settings.png") };
	 																														 
	/* 15 */ button_textures[BUTTON::TEXTURE::VOLUME]			= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\volume.png") };
	/* 16 */ button_textures[BUTTON::TEXTURE::VOLUME_MUTE]		= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\volume_mute.png") };
	/* 17 */ button_textures[BUTTON::TEXTURE::TIMER]			= { std::make_shared<Surface>(L"Textures\\Minesweeper\\Buttons\\timer.png") };
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

/*--------------------------------------------*/

void Minesweeper::InitialiseDisplay()
{
	InitialiseButtonBackground();
	InitialiseMinesCounter();
	InitialiseTimer();
	InitialiseButtons();
	InitialiseButtonDividers();
}

void Minesweeper::InitialiseButtonBackground()
{
	const unsigned int TOP		= OFFSET / 2u;
	const unsigned int BOTTOM	= TOP + DIGIT_H + OFFSET - 1u;
	const unsigned int LEFT		= SCREEN_RECT.left + OFFSET / 2u;
	const unsigned int RIGHT	= SCREEN_RECT.right - OFFSET / 2u + 1u;

	button_background_rect = { TOP,BOTTOM,LEFT,RIGHT };

	button_background_block = { button_background_rect,button_textures[0] };	
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
void Minesweeper::InitialiseButtons()
{
	button_rects.resize(12);
	button_blocks.resize(9);
	button_pressed.assign(9,false);

	{
		// COLS BUTTON 

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = mine_counter_rect.right + OFFSET + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::COLS] = { TOP, BOTTOM, LEFT, RIGHT };

		button_blocks[BUTTON::COLS] = {
			button_rects[BUTTON::RECT::COLS],
			button_textures[BUTTON::TEXTURE::COLS] };
	}

	{
		// ROWS BUTTON 

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::COLS].right + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::ROWS] = { TOP, BOTTOM, LEFT, RIGHT };

		button_blocks[BUTTON::ROWS] = {
			button_rects[BUTTON::RECT::ROWS],
			button_textures[BUTTON::TEXTURE::ROWS] };
	}
	
	{
		// MINES BUTTON 

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::ROWS].right + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::MINES] = { TOP, BOTTOM, LEFT, RIGHT };

		button_blocks[BUTTON::MINES] = {
			button_rects[BUTTON::RECT::MINES],
			button_textures[BUTTON::TEXTURE::MINES] };
	}

	{
		// LEVELS BUTTON 

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::MINES].right + OFFSET + OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::LEVEL] = { TOP, BOTTOM, LEFT, RIGHT };

		button_blocks[BUTTON::LEVEL] = {
			button_rects[BUTTON::RECT::LEVEL],
			button_textures[BUTTON::TEXTURE::LEVEL_EASY] };
	}

	const unsigned int OFF = 5u;

	{
		// HELP BUTTON

		const unsigned int TOP = OFFSET * 1u;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = SCREEN_W / 2u - OFFSET * 6u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::HELP_RELEASE] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[BUTTON::RECT::HELP_PRESSED] = { 
			TOP + OFF,
			BOTTOM - OFF,
			LEFT + OFF,
			RIGHT - OFF };

		button_blocks[BUTTON::HELP] = {
			button_rects[BUTTON::RECT::HELP_RELEASE],
			button_textures[BUTTON::TEXTURE::HELP] };
	}

	{
		// RESTART BUTTON

		const unsigned int TOP = OFFSET * 1u;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = SCREEN_W / 2u - OFFSET * 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::RESTART_RELEASE] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[BUTTON::RECT::RESTART_PRESSED] = { 
			TOP + OFF,
			BOTTOM - OFF,
			LEFT + OFF,
			RIGHT - OFF };

		button_blocks[BUTTON::RESTART] = {
			button_rects[BUTTON::RECT::RESTART_RELEASE],
			button_textures[BUTTON::TEXTURE::RESTART_SMILE] };
	}

	{
		// SETTINGS BUTTON

		const unsigned int TOP = OFFSET * 1u;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = SCREEN_W / 2u + OFFSET * 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::SETTINGS_RELEASE] = { TOP,BOTTOM,LEFT,RIGHT };
		button_rects[BUTTON::RECT::SETTINGS_PRESSED] = { 
			TOP + OFF,
			BOTTOM - OFF,
			LEFT + OFF,
			RIGHT - OFF };

		button_blocks[BUTTON::SETTINGS] = {
			button_rects[BUTTON::RECT::SETTINGS_RELEASE],
			button_textures[BUTTON::TEXTURE::SETTINGS] };
	}
	
	{
		// TIMER ICON
		
		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = timer_rect.left - DIGIT_W - OFFSET - OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::TIMER] = { TOP, BOTTOM, LEFT, RIGHT };

		button_blocks[BUTTON::TIMER] = {
			button_rects[BUTTON::RECT::TIMER],
			button_textures[BUTTON::TEXTURE::TIMER] };
	}

	{
		// VOLUME BUTTON

		const unsigned int TOP = OFFSET;
		const unsigned int BOTTOM = TOP + DIGIT_H - 1u;
		const unsigned int LEFT = 
			button_rects[BUTTON::RECT::TIMER].left - DIGIT_W - OFFSET - OFFSET / 2u;
		const unsigned int RIGHT = LEFT + DIGIT_W - 1u;

		button_rects[BUTTON::RECT::VOLUME] = { TOP, BOTTOM, LEFT, RIGHT };

		button_blocks[BUTTON::VOLUME] = {
			button_rects[BUTTON::RECT::VOLUME],
			button_textures[BUTTON::TEXTURE::VOLUME] };
	}
}
void Minesweeper::InitialiseButtonDividers()
{
	const unsigned int DIVIDER_W = (DIGIT_W + OFFSET / 2u) / 6u;

	{
		// BETWEEN MINES COUNTER & COLS BUTTON 

		const unsigned int GAP = button_rects[BUTTON::RECT::COLS].left - mine_counter_rect.right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = mine_counter_rect.right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		button_divider_rects.emplace_back( TOP,BOTTOM,LEFT,RIGHT );

		button_divider_blocks.emplace_back( button_divider_rects[0],button_textures[1] );
	}

	{
		// BETWEEN TIMER BUTTON & TIMER

		const unsigned int GAP = timer_rect.left - button_rects[BUTTON::RECT::TIMER].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::TIMER].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		button_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		button_divider_blocks.emplace_back(button_divider_rects[1], button_textures[1]);
	}

	{
		// BETWEEN MINES BUTTON & LEVELS BUTTON

		const unsigned int GAP = 
			button_rects[BUTTON::RECT::LEVEL].left - button_rects[BUTTON::RECT::MINES].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::MINES].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		button_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		button_divider_blocks.emplace_back(button_divider_rects[2], button_textures[1]);
	}

	{
		// BETWEEN LEVELS BUTTON & HELP BUTTON 

		const unsigned int GAP = 
			button_rects[BUTTON::RECT::HELP_RELEASE].left - button_rects[BUTTON::RECT::LEVEL].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::LEVEL].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		button_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		button_divider_blocks.emplace_back(button_divider_rects[3], button_textures[1]);
	}


	{
		// VOLUME BUTTON & TIMER BUTTON

		const unsigned int GAP = 
			button_rects[BUTTON::RECT::TIMER].left - button_rects[BUTTON::RECT::VOLUME].right;

		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::VOLUME].right + GAP / 2u - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;

		button_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);

		button_divider_blocks.emplace_back(button_divider_rects[4], button_textures[1]);
	}

	{
		// VOLUME BUTTON LEFT
	
		const unsigned int TOP = OFFSET / 2u;
		const unsigned int BOTTOM = TOP + DIGIT_H + OFFSET - 1u;
		const unsigned int LEFT = button_rects[BUTTON::RECT::VOLUME].left - DIVIDER_W - DIVIDER_W / 2u;
		const unsigned int RIGHT = LEFT + DIVIDER_W - 1u;
	
		button_divider_rects.emplace_back(TOP, BOTTOM, LEFT, RIGHT);
	
		button_divider_blocks.emplace_back(button_divider_rects[5], button_textures[1]);
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
					if (grid->CheckTiles(x, y, true))
					{
						gameover = true;
						gamewon = false;
					}
				}
			}
		}
	}
}

void Minesweeper::SetSound()
{
	if (volume_mute)
	{
		sound_fx_volume = 0.0f;
		music_volume = 0.0f;
	}
	else
	{
		sound_fx_volume = 1.0f;
		music_volume = 1.0f;
	}
}

void Minesweeper::SetMinesCounter()
{
	Bumble::ExtractDigits(mine_counter_value, mines);
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

	Bumble::ExtractDigits(timer_value, time);

	while (timer_value.size() < 3u)
	{
		timer_value.emplace_back(0u);
	}
}
void Minesweeper::SetButtons(Mouse& mouse)
{
	for (size_t i = 0u; i < button_blocks.size(); i++)
	{
		button_blocks[i].SetMouseOver(mouse);
	}

	for (size_t i = 0u; i < button_blocks.size(); i++)
	{
		if (!mouse_pressed)
		{
			if (button_blocks[i].MouseOver() && mouse.LeftIsPressed())
			{
				sounds[SOUNDS::CLICK_0].Play(1.0f, sound_fx_volume);

				switch (i)
				{
				case BUTTON::LEVEL:
					switch (level)
					{
					case 0:
						level++;
						button_blocks[BUTTON::LEVEL].SetTexture(button_textures[BUTTON::TEXTURE::LEVEL_MEDIUM]);
						break;
					case 1:
						level++;
						button_blocks[BUTTON::LEVEL].SetTexture(button_textures[BUTTON::TEXTURE::LEVEL_HARD]);
						break;
					case 2:
						level++;
						button_blocks[BUTTON::LEVEL].SetTexture(button_textures[BUTTON::TEXTURE::LEVEL_CUSTOM]);
						break;
					case 3:
						level = 0;
						button_blocks[BUTTON::LEVEL].SetTexture(button_textures[BUTTON::TEXTURE::LEVEL_EASY]);
						break;
					default:
						break;
					}
					break;
				case BUTTON::HELP:
					is_help = true;
					break;
				case BUTTON::RESTART:
					Restart();
					break;
				case BUTTON::SETTINGS:
					break;
				case BUTTON::VOLUME:
					if (!volume_mute)
					{
						volume_mute = true;
						button_blocks[BUTTON::VOLUME].SetTexture(button_textures[BUTTON::TEXTURE::VOLUME_MUTE]);
					}
					else
					{
						volume_mute = false;
						button_blocks[BUTTON::VOLUME].SetTexture(button_textures[BUTTON::TEXTURE::VOLUME]);
					}
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

	if (gameover)
	{
		if (gamewon)
		{
			button_blocks[BUTTON::RESTART].SetTexture(button_textures[BUTTON::TEXTURE::RESTART_WIN]);
		}
		else
		{
			button_blocks[BUTTON::RESTART].SetTexture(button_textures[BUTTON::TEXTURE::RESTART_LOSE]);
		}
	}

	const bool OVER_GRID = grid->GridRect().Contains(mouse.GetPos());

	if (!gameover && mouse.IsInWindow())
	{
		if ( OVER_GRID &&
			(mouse.MiddleIsPressed() ||
			(mouse.LeftIsPressed() && mouse.RightIsPressed())))
		{
			button_blocks[BUTTON::RESTART].SetTexture(button_textures[BUTTON::TEXTURE::RESTART_GASP]);

			if (!checked_sound_played)
			{
				sounds[SOUNDS::CLICK_1].Play(1.0f, sound_fx_volume);
				checked_sound_played = true;
			}
		}
		else if ((!mouse.MiddleIsPressed() &&
			!mouse.LeftIsPressed() &&
			!mouse.RightIsPressed()) || !OVER_GRID)
		{
			checked_sound_played = false;

			button_blocks[BUTTON::RESTART].SetTexture(button_textures[BUTTON::TEXTURE::RESTART_SMILE]);
		}
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
		}
		else
		{
			if (!gameover_sound_played)
			{
				sounds[0].Play(1.0f, sound_fx_volume);
				gameover_sound_played = true;
			}

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

void Minesweeper::DrawHelp(Graphics& gfx)
{
	if (is_help)
	{
		gfx.DrawRect(true, SCREEN_RECT, Colors::LightGray);
		help_block.Draw(gfx);
		help_button_block.Draw(gfx);
	}
}

void Minesweeper::DrawButtonDisplay(Graphics& gfx)
{
	DrawButtonBackground(gfx);
	DrawMinesCounter(gfx);
	DrawTimer(gfx);
	DrawButtons(gfx);
	DrawButtonDividers(gfx);
}

void Minesweeper::DrawButtonBackground(Graphics& gfx)
{
	button_background_block.Draw(gfx);
}
void Minesweeper::DrawMinesCounter(Graphics& gfx)
{	
	for (unsigned int i = 0u; i < mine_counter_value.size(); i++)
	{
		mine_counter[mine_counter_value[i]][i].Draw(gfx);
	}	
}
void Minesweeper::DrawTimer(Graphics& gfx)
{
	for (unsigned int i = 0u; i < timer_value.size(); i++)
	{
		timer_blocks[timer_value[i]][i].Draw(gfx);
	}
}
void Minesweeper::DrawButtons(Graphics& gfx)
{
	for (auto& b : button_blocks)
	{
		b.Draw(gfx);
	}
}
void Minesweeper::DrawButtonDividers(Graphics& gfx)
{
	for (auto& d : button_divider_blocks)
	{
		d.Draw(gfx);
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

//void Minesweeper::Setup()
//{	
//	timer_started = false;
//	time = 0u;
//
//	gameover = false;
//	gamewon = false;
//	gameover_sound_played = false;
//
//	is_help = false;
//	is_settings = false;
//
//	flags = 0u;
//	frames = 0u;
//	index0 = 0u;
//	index1 = 0u;
//
//	button_blocks[BUTTON::RESTART].SetTexture(button_textures[BUTTON::TEXTURE::RESTART_SMILE]); 
//
//	if (grid != nullptr)
//	{
//		mines = grid->Mines();
//	}
//}
void Minesweeper::Restart()
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
	
	button_blocks[BUTTON::RESTART].SetTexture(button_textures[BUTTON::TEXTURE::RESTART_SMILE]);

	switch (level)
	{
	case 0:
		grid = std::make_unique<Grid>(
			grid_textures,
			tile_textures,
			LEVEL::EASY::COLS,
			LEVEL::EASY::ROWS,
			LEVEL::EASY::MINES,
			OFFSET);
		break;
	case 1:
		grid = std::make_unique<Grid>(
			grid_textures,
			tile_textures,
			LEVEL::MEDIUM::COLS,
			LEVEL::MEDIUM::ROWS,
			LEVEL::MEDIUM::MINES,
			OFFSET);
		break;
	case 2:
		grid = std::make_unique<Grid>(
			grid_textures,
			tile_textures,
			LEVEL::HARD::COLS,
			LEVEL::HARD::ROWS,
			LEVEL::HARD::MINES,
			OFFSET);
		break;
	case 3:
		grid = std::make_unique<Grid>(
			grid_textures,
			tile_textures,
			custom_cols,
			custom_rows,
			custom_mines,
			OFFSET);
		break;
	default:
		break;
	}
	gameover_sound_played	= false;
	
	//grid->Reset();
	
	if (grid != nullptr)
	{
		mines = grid->Mines();
	}
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
		SetSound();

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
void Minesweeper::Draw(Graphics& gfx)
{
	if (is_help)		
	{
		DrawHelp(gfx);
	}
	else
	{
		DrawButtonDisplay(gfx);

		if (grid != nullptr)
		{
			grid->Draw(gfx);
		}

		DrawGameOver(gfx);
	}
}
