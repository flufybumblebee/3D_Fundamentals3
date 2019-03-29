#include "Tetris.h"

Tetris::Tetris(Keyboard& kbd, Graphics& gfx)
	:
	kbd(kbd),
	gfx(gfx)
{
	block_Textures = {
		&tileBlack,
		&tileOrange,
		&tileCyan,
		&tileGreen,
		&tileRed,
		&tileBlue,
		&tileMagenta,
		&tileYellow,
		&tileRed,
		&tileGrey };

	InitialiseTetrominos();
	InitialiseDigits();
	InitialiseTextScore();
	//InitialiseTextLevel();
	InitialiseTextPause();
	InitialiseTextGameOver();			
	InitialiseTextDigits();	
}

Tetris::~Tetris()
{
}

void Tetris::Setup()
{
	gameIsOver			= false;
	tetrominoNext		= Random(0, 6);
	tetrominoCurrent	= Random(0, 6);
	currentX			= fieldW / 2 - 2;
	currentY			= 0;
	currentRotation		= 0;
	speed				= 20;
	score				= 0;
	level				= 0;
	tetrominoCounter	= 0;
	speedCounter		= 0;

	blockBuffer_Digits.clear();

	SetFieldBuffer();
	SetFieldBlocks();
	SetNextTetromino();
	SetScore();
}

void Tetris::Update()
{
	if (!spaceIsPressed)
	{
		if (kbd.KeyIsPressed(VK_SPACE) && !gameIsPaused)
		{
			if (gameIsOver)
			{
				Setup();
			}
			else
			{
				gameIsPaused = true;
			}
			spaceIsPressed = true;
		}
		else if (kbd.KeyIsPressed(VK_SPACE) && gameIsPaused)
		{
			gameIsPaused = false;
			spaceIsPressed = true;
		}
	}
	else
	{
		if (!kbd.KeyIsPressed(VK_SPACE))
		{
			spaceIsPressed = false;
		}
	}

	if (!gameIsPaused)
	{
		if (!gameIsOver)
		{
			// game timing
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			speedCounter++;
			forceDown = (speedCounter == speed);

			// input
			currentX -= (kbd.KeyIsPressed(VK_LEFT) && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX - 1, currentY + 0)) ? 1 : 0;
			currentX += (kbd.KeyIsPressed(VK_RIGHT) && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX + 1, currentY + 0)) ? 1 : 0;
			currentY += (kbd.KeyIsPressed(VK_DOWN) && DoesTetrisFit(tetrominoCurrent, currentRotation, currentX + 0, currentY + 1)) ? 1 : 0;

			// block up/rotate key from being pressed every frame
			if (kbd.KeyIsPressed(VK_UP))
			{
				currentRotation += (!keyIsPressed && DoesTetrisFit(tetrominoCurrent, currentRotation + 1, currentX, currentY)) ? 1 : 0;
				keyIsPressed = true;
			}
			else
			{
				keyIsPressed = false;
			}

			if (forceDown)
			{
				// update difficulty/speed every 50 tetris played
				speedCounter = 0;
				tetrominoCounter++;
				if (tetrominoCounter % 50 == 0)
				{
					if (speed >= 10) speed--;
				}

				if (DoesTetrisFit(tetrominoCurrent, currentRotation, currentX, currentY + 1))
				{
					currentY++; // force tetris down
				}
				else
				{
					// add tetris to fixed field buffer
					for (int y = 0; y < tetroH; y++)					
					{
							for (int x = 0; x < tetroW; x++)
						{
							if (tetromino[tetrominoCurrent][Rotate(x, y, currentRotation)] == 'X')
							{
								blockBuffer_Fixed[(currentY + y) * fieldW + (currentX + x)] =
									tetrominoCurrent + 1;
							}
						}
					}

					// check for lines
					for (int y = 0; y < 4; y++)
					{
						if (currentY + y < fieldH - 1)
						{
							bool isLine = true;
							for (int x = 1; x < fieldW - 1; x++)
							{
								isLine &= (blockBuffer_Fixed[(currentY + y) * fieldW + x]) != 0;
							}

							if (isLine)
							{
								// remove line, set to =
								for (int x = 1; x < fieldW - 1; x++)
								{
									blockBuffer_Fixed[(currentY + y) * fieldW + x] = 8;
								}
								lines.push_back(currentY + y);
							}
						}
					}

					// calculate score
					score += 25;
					if (!lines.empty()) score += (1 << lines.size()) * 100;

					SetScore();

					// randomly assign new tetris

					currentX = fieldW / 2 - 2;
					currentY = 0;
					currentRotation = 0;

					tetrominoCurrent = tetrominoNext;
					tetrominoNext = Random(0, 6);

					SetNextTetromino();

					// if tetris doesn't fit...
					gameIsOver = !DoesTetrisFit(tetrominoCurrent, currentRotation, currentX, currentY);
				}
			}

			// add all fixed blocks to displayBuffer from fixedBuffer
			for (int y = 0; y < fieldH; y++)			
			{
				for (int x = 0; x < fieldW; x++)
				{
					blockBuffer_Shown[y*fieldW + x] =
						" ABCDEFG=#"[blockBuffer_Fixed[y*fieldW + x]];
				}
			}

			// add current tetris piece to display buffer
			for (int y = 0; y < tetroH; y++)			
			{
				for (int x = 0; x < tetroW; x++)
				{
					if (tetromino[tetrominoCurrent][Rotate(x, y, currentRotation)] == 'X')
					{
						blockBuffer_Shown[(currentY + y) * fieldW + (currentX + x)] =
							" ABCDEFG=#"[tetrominoCurrent + 1];	// set to the ascii table value
					}
				}
			}

			// Animate Line Completion
			if (!lines.empty())
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(800)); // delay

				for (auto &v : lines)
				{
					for (int x = 1; x < fieldW - 1; x++)
					{
						for (int y = v; y > 0; y--)
						{
							blockBuffer_Fixed[y * fieldW + x] = blockBuffer_Fixed[(y - 1) * fieldW + x];
						}
						blockBuffer_Fixed[x] = 0;
					}
				}

				lines.clear();
			}

			SetFieldBlocks();
		}
	}
}

void Tetris::Draw()
{
	DrawBackground();
	DrawField();
	DrawNextTetromino();
	DrawTextScore();
	DrawTextPause();
	DrawTextGameOver();
	DrawTextDigits();
}

/*-------------------------------------------*/

void Tetris::InitialiseDigits()
{
	text_Digits[0].append("......");
	text_Digits[0].append("XXXXX.");
	text_Digits[0].append("X...X.");
	text_Digits[0].append("X...X.");
	text_Digits[0].append("X...X.");
	text_Digits[0].append("XXXXX.");
	text_Digits[0].append("......");

	text_Digits[1].append("......");
	text_Digits[1].append(".XX...");
	text_Digits[1].append("..X...");
	text_Digits[1].append("..X...");
	text_Digits[1].append("..X...");
	text_Digits[1].append("XXXXX.");
	text_Digits[1].append("......");

	text_Digits[2].append("......");
	text_Digits[2].append("XXXXX.");
	text_Digits[2].append("....X.");
	text_Digits[2].append("XXXXX.");
	text_Digits[2].append("X.....");
	text_Digits[2].append("XXXXX.");
	text_Digits[2].append("......");

	text_Digits[3].append("......");
	text_Digits[3].append("XXXXX.");
	text_Digits[3].append("....X.");
	text_Digits[3].append("XXXXX.");
	text_Digits[3].append("....X.");
	text_Digits[3].append("XXXXX.");
	text_Digits[3].append("......");

	text_Digits[4].append("......");
	text_Digits[4].append("X.....");
	text_Digits[4].append("X..X..");
	text_Digits[4].append("X..X..");
	text_Digits[4].append("XXXXX.");
	text_Digits[4].append("...X..");
	text_Digits[4].append("......");

	text_Digits[5].append("......");
	text_Digits[5].append("XXXXX.");
	text_Digits[5].append("X.....");
	text_Digits[5].append("XXXXX.");
	text_Digits[5].append("....X.");
	text_Digits[5].append("XXXXX.");
	text_Digits[5].append("......");

	text_Digits[6].append("......");
	text_Digits[6].append("X.....");
	text_Digits[6].append("X.....");
	text_Digits[6].append("XXXXX.");
	text_Digits[6].append("X...X.");
	text_Digits[6].append("XXXXX.");
	text_Digits[6].append("......");

	text_Digits[7].append("......");
	text_Digits[7].append("XXXXX.");
	text_Digits[7].append("....X.");
	text_Digits[7].append("....X.");
	text_Digits[7].append("....X.");
	text_Digits[7].append("....X.");
	text_Digits[7].append("......");

	text_Digits[8].append("......");
	text_Digits[8].append("XXXXX.");
	text_Digits[8].append("X...X.");
	text_Digits[8].append("XXXXX.");
	text_Digits[8].append("X...X.");
	text_Digits[8].append("XXXXX.");
	text_Digits[8].append("......");

	text_Digits[9].append("......");
	text_Digits[9].append("XXXXX.");
	text_Digits[9].append("X...X.");
	text_Digits[9].append("XXXXX.");
	text_Digits[9].append("....X.");
	text_Digits[9].append("XXXXX.");
	text_Digits[9].append("......");
}

void Tetris::InitialiseTetrominos()
{
	tetromino[0].append("..X.");
	tetromino[0].append("..X.");
	tetromino[0].append("..X.");
	tetromino[0].append("..X.");

	tetromino[1].append("..X.");
	tetromino[1].append(".XX.");
	tetromino[1].append(".X..");
	tetromino[1].append("....");

	tetromino[2].append(".X..");
	tetromino[2].append(".XX.");
	tetromino[2].append("..X.");
	tetromino[2].append("....");

	tetromino[3].append("....");
	tetromino[3].append(".XX.");
	tetromino[3].append(".XX.");
	tetromino[3].append("....");

	tetromino[4].append("..X.");
	tetromino[4].append(".XX.");
	tetromino[4].append("..X.");
	tetromino[4].append("....");

	tetromino[5].append(".XX.");
	tetromino[5].append("..X.");
	tetromino[5].append("..X.");
	tetromino[5].append("....");

	tetromino[6].append(".XX.");
	tetromino[6].append(".X..");
	tetromino[6].append(".X..");
	tetromino[6].append("....");
}

void Tetris::InitialiseTextLevel()
{
	text_Level.append(".....................................");
	text_Level.append("...X.....XXXXX.X...X.XXXXX.X.........");
	text_Level.append("...X.....X.....X...X.X.....X.....X...");
	text_Level.append("...X.....XXXXX.X...X.XXXXX.X.........");
	text_Level.append("...X.....X......X.X..X.....X.....X...");
	text_Level.append("...XXXXX.XXXXX...X...XXXXX.XXXXX.....");
	text_Level.append(".....................................");

	const int bloW = 5;
	const int bloH = 5;

	for (int y = 0; y < levelH; y++)
	{
		for (int x = 0; x < levelW; x++)
		{
			uint i = (text_Level[y * levelW + x] != '.') ? 9 : 0;
		
			RectI rect = RectI(
				(y * bloH),
				(y * bloH) + bloH,
				(x * bloW),
				(x * bloW) + bloW);

			blocks_Text_Level[y * scoreW + x] = Block(rect, block_Textures[i]);
		}
	}
}

void Tetris::InitialiseTextScore()
{
	text_Score.append(".....................................");
	text_Score.append("...XXXXX.XXXXX.XXXXX.XXXX..XXXXX.....");
	text_Score.append("...X.....X.....X...X.X...X.X.....X...");
	text_Score.append("...XXXXX.X.....X...X.X...X.XXXXX.....");
	text_Score.append(".......X.X.....X...X.XXXX..X.....X...");
	text_Score.append("...XXXXX.XXXXX.XXXXX.X...X.XXXXX.....");
	text_Score.append(".....................................");

	const int bloW = 7;
	const int bloH = 7;

	int color;
	for (int y = 0; y < scoreH; y++)
	{
		for (int x = 0; x < scoreW; x++)
		{
			if (text_Score[y * scoreW + x] == '.')
			{
				color = 0;
			}
			else
			{
				color = 9;
			}
			RectI rect = RectI(
				(y * bloH),
				(y * bloH) + bloH,
				(x * bloW) + (bloW * 10),
				(x * bloW) + (bloW * 10) + bloW	);
			blocks_Text_Score[y * scoreW + x] =	Block(rect,	block_Textures[color]);
		}
	}
}

void Tetris::InitialiseTextDigits()
{
	const int bloW = 7;
	const int bloH = 7;

	for (int i = 0; i < 10; i++)
	{
		for (int x = 0; x < digitW; x++)
		{
			for (int y = 0; y < digitH; y++)
			{
				RectI rect = RectI(
					(y * bloH),
					(y * bloH) + bloH,
					(x * bloW) + (scoreW*bloW) + (digitW*bloW * (9 - i)) + (bloW * 10),
					(x * bloW) + (scoreW*bloW) + (digitW*bloW * (9 - i)) + (bloW * 10) + bloW);

				uint j = (text_Digits[i][y * digitW + x] == 'X') ? 9 : 0;

				blocks_Digits[i][y * digitW + x] = Block(rect,block_Textures[j]);
			}
		}
	}
}

void Tetris::InitialiseTextPause()
{
	text_Pause.append("...................................");
	text_Pause.append("...XXXXX.XXXXX.X...X.XXXXX.XXXXX...");
	text_Pause.append("...X...X.X...X.X...X.X.....X.......");
	text_Pause.append("...X...X.X...X.X...X.XXXXX.XXXXX...");
	text_Pause.append("...XXXXX.XXXXX.X...X.....X.X.......");
	text_Pause.append("...X.....X...X.XXXXX.XXXXX.XXXXX...");
	text_Pause.append("...................................");

	for (int x = 0; x < pauseW; x++)
	{
		for (int y = 0; y < pauseH; y++)
		{
			uint i = (text_Pause[y * pauseW + x] != '.') ? Random(1, 7) : 0;
			
			RectI rect = RectI(
				(gfx.ScreenHeight / 2) - ((pauseH / 2)*blocksH) + (y * blocksH),
				(gfx.ScreenHeight / 2) - ((pauseH / 2)*blocksH) + (y * blocksH) + blocksH,
				(gfx.ScreenWidth / 2) - ((pauseW / 2)*blocksW) + (x * blocksW),
				(gfx.ScreenWidth / 2) - ((pauseW / 2)*blocksW) + (x * blocksW) + blocksW);

			blocks_Text_Pause[y * pauseW + x] = Block(rect, block_Textures[i]);
		}
	}
}

void Tetris::InitialiseTextGameOver()
{
	text_GameOver.append(".............................");
	text_GameOver.append("...XXXXX.XXXXX.XXXXX.XXXXX...");
	text_GameOver.append("...X.....X...X.X.X.X.X.......");
	text_GameOver.append("...X..XX.X...X.X.X.X.XXXXX...");
	text_GameOver.append("...X...X.XXXXX.X.X.X.X.......");
	text_GameOver.append("...XXXXX.X...X.X.X.X.XXXXX...");
	text_GameOver.append(".............................");
	text_GameOver.append("...XXXXX.X...X.XXXXX.XXXX....");
	text_GameOver.append("...X...X.X...X.X.....X...X...");
	text_GameOver.append("...X...X.X...X.XXXXX.X...X...");
	text_GameOver.append("...X...X..X.X..X.....XXXX....");
	text_GameOver.append("...XXXXX...X...XXXXX.X...X...");
	text_GameOver.append(".............................");

	for (int x = 0; x < gameOverW; x++)
	{
		for (int y = 0; y < gameOverH; y++)
		{
			uint i = (text_GameOver[y * gameOverW + x] != '.') ? Random(1, 7) : 0;
			
			RectI rect = RectI(
				(gfx.ScreenHeight / 2) - ((gameOverH / 2)*blocksH) + (y * blocksH),
				(gfx.ScreenHeight / 2) - ((gameOverH / 2)*blocksH) + (y * blocksH) + blocksH,
				(gfx.ScreenWidth / 2) - ((gameOverW / 2)*blocksW) + (x * blocksW),
				(gfx.ScreenWidth / 2) - ((gameOverW / 2)*blocksW) + (x * blocksW) + blocksW);

			blocks_Text_GameOver[y * gameOverW + x] = Block(rect, block_Textures[i]);
		}
	}
}

/*-------------------------------------------*/

void Tetris::SetFieldBuffer()
{
	blockBuffer_Fixed.clear();
	blockBuffer_Shown.clear();

	// set playing field border
	for (int y = 0; y < fieldH; y++)	
	{
		for (int x = 0; x < fieldW; x++)
		{
			blockBuffer_Fixed.push_back((x == 0 || x == fieldW - 1 || y == fieldH - 1) ? 9 : 0);

			//if (x == 0 || x == fieldW - 1) field[y*fieldW + x] = 9;
			//else if(y == fieldH - 1) field[y*fieldW + x] = 9;
			//else field[y*fieldW + x] = 0;
		}
	}

	// set shown blocks buffer with fixed block buffer
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)		
		{
			blockBuffer_Shown.push_back(" ABCDEFG=#"[blockBuffer_Fixed[y * fieldW + x]]);
		}
	}
}

void Tetris::SetFieldBlocks()
{
	blocks.clear();

	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			RectI rect = RectI(
				offsetHeight + (y * blocksH),
				offsetHeight + (y * blocksH) + blocksH,
				offsetWidth + (x * blocksW),
				offsetWidth + (x * blocksW) + blocksW);

			uint i = ConvertCharToInt(blockBuffer_Shown[y * fieldW + x]);
			blocks.push_back(Block(rect, block_Textures[i]));
		}
	}
}

void Tetris::SetNextTetromino()
{
	blocks_Next.clear();

	for (int y = 0; y < tetroH; y++)
	{
		for (int x = 0; x < tetroW; x++)
		{			
			RectI rect = RectI(
				offsetHeight + (y * blocksH),
				offsetHeight + (y * blocksH) + blocksH,
				offsetWidth + (blocksW * fieldW) + (x * blocksW),
				offsetWidth + (blocksW * fieldW) + (x * blocksW) + blocksW);
			
			uint i = (tetromino[tetrominoNext][y * tetroW + x] != '.') ? tetrominoNext + 1 : 0;

			blocks_Next.push_back(Block(rect,block_Textures[i]));
		}
	}
}

void Tetris::SetScore()
{
	ExtractDigits(blockBuffer_Digits, score);	
}

/*-------------------------------------------*/

void Tetris::DrawBackground()
{
	for (uint y = 0; y < background.GetHeight(); y++)
	{
		for (uint x = 0; x < background.GetWidth(); x++)
		{
			gfx.PutPixel(x, y, background.GetPixel(x, y));
		}
	}
}

void Tetris::DrawField()
{
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			if (blockBuffer_Shown[y * fieldW + x] == ' ')
			{
				continue;
			}
			else
			{
				blocks[y * fieldW + x].Draw(gfx);
			}
		}
	}
}

void Tetris::DrawNextTetromino()
{
	for (int y = 0; y < tetroH; y++)
	{
		for (int x = 0; x < tetroW; x++)
		{
			if (tetromino[tetrominoNext][y * tetroW + x] == '.')
			{
				continue;
			}
			else
			{
				blocks_Next[y * tetroW + x].Draw(gfx);
			}
		}
	}
}

void Tetris::DrawTextScore()
{
	// Draw Score Text
	for (int y = 0; y < scoreH; y++)
	{
		for (int x = 0; x < scoreW; x++)
		{
			if (text_Score[y * scoreW + x] == '.')
			{
				continue;
			}
			else
			{
				blocks_Text_Score[y * scoreW + x].Draw(gfx);
			}
		}
	}	
}

void Tetris::DrawLevel()
{
	
}

void Tetris::DrawTextDigits()
{
	/*for (int i = 0; i < blockBuffer_Digits.size(); i++)
	{
		for (int y = 0; y < digitH; y++)
		{
			for (int x = 0; x < digitW; x++)
			{
				for (int j = 0; j < 10; j++)
				{
					if (text_Digits[i][y * digitW + x] == '.')
					{
						continue;
					}
					else
					{
						blocks_Digits[blockBuffer_Digits[i]][y * digitW + x].Draw(gfx);
					}
				}				
			}
		}
	}*/

	
	blockBuffer_Digits.clear();
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	blockBuffer_Digits.push_back(count);
	
	for (int y = 0; y < digitH; y++)
	{
		for (int x = 0; x < digitW; x++)
		{
			blocks_Digits[blockBuffer_Digits[count]][y * digitW + x].Draw(gfx);
		}
	}
	count++;
}

void Tetris::DrawTextPause()
{
	if (gameIsPaused)
	{
		for (int y = 0; y < pauseH; y++)
		{
			for (int x = 0; x < pauseW; x++)
			{
				if (text_Pause[y * pauseW + x] == '.')
				{
					continue;
				}
				else
				{
					blocks_Text_Pause[y * pauseW + x].Draw(gfx);
				}
			}
		}
	}
}

void Tetris::DrawTextGameOver()
{
	if (gameIsOver)
	{
		for (int y = 0; y < gameOverH; y++)
		{
			for (int x = 0; x < gameOverW; x++)
			{
				if (text_GameOver[y * gameOverW + x] == '.')
				{
					continue;
				}
				else
				{
					blocks_Text_GameOver[y * gameOverW + x].Draw(gfx);
				}
			}
		}
	}
}

/*-------------------------------------------*/

int Tetris::Rotate(int px, int py, int r)
{
	int pi = 0;

	switch (r % 4)
	{
	case 0: //   0 degrees 
		pi = 0 + (py * 4) + px;			//  0  1  2  3
		break;							//  4  5  6  7
										//  8  9 10 11
										// 12 13 14 15

	case 1: //  90 degrees
		pi = 12 + py - (px * 4);		// 12  8  4  0
		break;							// 13  9  5  1
										// 14 10  6  2
										// 15 11  7  3

	case 2: // 120 degrees
		pi = 15 - (py * 4) - px;		// 15 14 13 12
		break;							// 11 10  9  8
										//  7  6  5  4
										//  3  2  1  0

	case 3: // 270 degrees
		pi = 3 - py + (px * 4);			//  3  7 11 15
		break;							//  2  6 10 14
										//  1  5  9 13
										//  0  4  8 12
	}

	return pi;
}

bool Tetris::DoesTetrisFit(int tetrisID, int rotation, int posX, int posY)
{
	for (int px = 0; px < 4; px++)
	{
		for (int py = 0; py < 4; py++)
		{
			// get index into tetris
			int pi = Rotate(px, py, rotation);

			// get index into field
			int fi = (posY + py) * fieldW + (posX + px);

			if (posX + px >= 0 && posX + px < fieldW &&
				posY + py >= 0 && posY + py < fieldH)
			{
				if (tetromino[tetrisID][pi] != L'.' && blockBuffer_Fixed[fi] != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

int Tetris::Random(const int min, const int max)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);
}

void Tetris::UpdateDigits(std::array<Block,digitW*digitH>& blocks_Digit, const std::string& text_Digit)
{
	/*for (int x = 0; x < digitW; x++)
	{
		for (int y = 0; y < digitH; y++)
		{

			if (text_Digit[y * digitW + x] != '.')
			{
				
			}
			else
			{
				
			}

			blocks_Digit
		}
	}*/
}

void Tetris::ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num)
{
	ints.clear();
	unsigned int result = num;

	if (result > 0)
	{
		while (result > 0)
		{
			ints.push_back(result % 10);
			result /= 10;
		}
	}
	else
	{
		ints.push_back(0);
	}
}

Color Tetris::ConvertCharToColor(const char value)
{
	Color color;
	for (int i = 0; i < 10; i++)
	{
		if (value == " ABCDEFG=#"[i])
		{
			color = block_Colors[i];
			return color;
		}
		else
		{
			continue;
		}
	}

	return Colors::Black;
}

uint Tetris::ConvertCharToInt(const char value)
{
	for (int i = 0; i < 10; i++)
	{
		if (value == " ABCDEFG=#"[i])
		{
			return i;
		}
		else
		{
			continue;
		}
	}

	return 0;
}
