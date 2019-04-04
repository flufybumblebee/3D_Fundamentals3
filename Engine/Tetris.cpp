#include "Tetris.h"

typedef unsigned int uint;
typedef unsigned char uchar;

Tetris::Tetris(Keyboard& kbd, Graphics& gfx)
	:
	kbd(kbd),
	gfx(gfx)
{		
	file_Background.emplace_back( L"Textures\\Backgrounds\\Nature0.jpg" );
	file_Background.emplace_back(L"Textures\\Backgrounds\\Nature1.jpg");
	file_Background.emplace_back(L"Textures\\Backgrounds\\Nature2.png");
	file_Background.emplace_back(L"Textures\\Backgrounds\\Nature3.jpg");
	file_Background.emplace_back(L"Textures\\Backgrounds\\Nature4.jpg");
	file_Background.emplace_back(L"Textures\\Backgrounds\\Nature5.jpg");
	file_Background.emplace_back( L"Textures\\Backgrounds\\BlocksBlue.jpg" );
	file_Background.emplace_back(L"Textures\\Backgrounds\\BlocksGreen.jpg");
	file_Background.emplace_back(L"Textures\\Backgrounds\\BlocksRainbow.jpg");
	file_Background.emplace_back( L"Textures\\Backgrounds\\Street0.bmp" );
	file_Background.emplace_back(L"Textures\\Backgrounds\\Space0.jpg");
	file_Background.emplace_back(L"Textures\\Backgrounds\\Space1.jpg");

	int size = (int)file_Background.size();
	tex_Background = new Surface(Surface::FromFile(file_Background[Random(0,size)]));
	InitialiseBackground();
	InitialiseTetrominos();
	InitialiseScore();
	InitialisePause();
	InitialiseGameOver();
}
Tetris::~Tetris()
{
	delete[] tex_Black;
	delete[] tex_Orange;
	delete[] tex_Cyan;
	delete[] tex_Green;
	delete[] tex_Red;
	delete[] tex_Blue;
	delete[] tex_Magenta;
	delete[] tex_Yellow;
	delete[] tex_Grey;
	
	delete[] tex_digit_0;
	delete[] tex_digit_1;
	delete[] tex_digit_2;
	delete[] tex_digit_3;
	delete[] tex_digit_4;
	delete[] tex_digit_5;
	delete[] tex_digit_6;
	delete[] tex_digit_7;
	delete[] tex_digit_8;
	delete[] tex_digit_9;
	
	delete[] tex_Background;
	delete[] tex_Pause;
	delete[] tex_GameOver;

	tex_Black	= nullptr;
	tex_Orange	= nullptr;
	tex_Cyan	= nullptr;
	tex_Green	= nullptr;
	tex_Red		= nullptr;
	tex_Blue	= nullptr;
	tex_Magenta = nullptr;
	tex_Yellow	= nullptr;
	tex_Grey	= nullptr;

	tex_digit_0 = nullptr;
	tex_digit_1 = nullptr;
	tex_digit_2 = nullptr;
	tex_digit_3 = nullptr;
	tex_digit_4 = nullptr;
	tex_digit_5 = nullptr;
	tex_digit_6 = nullptr;
	tex_digit_7 = nullptr;
	tex_digit_8 = nullptr;
	tex_digit_9 = nullptr;

	tex_Background	= nullptr;
	tex_Pause		= nullptr;
	tex_GameOver	= nullptr;
}

/*-------------------------------------------*/

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

	blockBuffer_Score.clear();

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
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
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
					level++;
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
								uint i = (currentY + y) * fieldW + (currentX + x);
								blockBuffer_Fixed[i] = tetrominoCurrent + 1;
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
								uint i = (currentY + y) * fieldW + x;
								isLine &= (blockBuffer_Fixed[i]) != 0;
							}

							if (isLine)
							{
								// remove line
								for (int x = 1; x < fieldW - 1; x++)
								{
									uint i = (currentY + y) * fieldW + x;
									blockBuffer_Fixed[i] = 8;
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
					uint i = y * fieldW + x;
					blockBuffer_Shown[i] = " ABCDEFG=#"[blockBuffer_Fixed[i]];
				}
			}

			// add current tetris piece to display buffer
			for (int y = 0; y < tetroH; y++)			
			{
				for (int x = 0; x < tetroW; x++)
				{
					// set to the ascii table value
					if (tetromino[tetrominoCurrent][Rotate(x, y, currentRotation)] == 'X')
					{
						uint i = (currentY + y) * fieldW + (currentX + x);
						blockBuffer_Shown[i] = " ABCDEFG=#"[tetrominoCurrent + 1];
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
							uint i = y * fieldW + x;
							uint j = (y - 1) * fieldW + x;
							blockBuffer_Fixed[i] = blockBuffer_Fixed[j];
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
	DrawBlur();
	DrawTextPause();
	DrawTextGameOver();
	DrawScore();
}

/*-------------------------------------------*/

void Tetris::InitialiseBackground()
{
	const RectI rect = { 0,(int)scrH,0,(int)scrW };
	block_Background = Block(rect, tex_Background);
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
void Tetris::InitialiseScore()
{	
	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < cols; x++)
		{
			RectI rect = RectI(
				(digitH / 5),
				(digitH / 5) + digitH,
				(scrW - 1) - (digitW * (x)) - (digitW / 5 * (x + 1)) - digitW,
				(scrW - 1) - (digitW * (x)) - (digitW / 5 * (x + 1)));

			blocks_Digits[y][x] = Block(rect, number_Textures[y]);
		}
	}	
}
void Tetris::InitialisePause()
{
	RectI rect = RectI(
		(scrH / 2) - (pauseH / 2),
		(scrH / 2) + (pauseH / 2),
		(scrW / 2) - (pauseW / 2),
		(scrW / 2) + (pauseW / 2));

	block_Pause = Block(rect, tex_Pause);
}
void Tetris::InitialiseGameOver()
{	
	RectI rect = RectI(
		(scrH / 2) - (gameOverH / 2),
		(scrH / 2) + (gameOverH / 2),
		(scrW / 2) - (gameOverW / 2),
		(scrW / 2) + (gameOverW / 2));

	block_GameOver = Block(rect, tex_GameOver);
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
			uint i = y * fieldW + x;
			blockBuffer_Shown.push_back(" ABCDEFG=#"[blockBuffer_Fixed[i]]);
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
			int i = y * fieldW + x;
			int j = ConvertCharToInt(blockBuffer_Shown[i]);
			blocks.push_back(Block(rect, block_Textures[j]));
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
			int i = y * tetroW + x;
			int j = (tetromino[tetrominoNext][i] != '.') ? tetrominoNext + 1 : 0;

			blocks_Next.push_back(Block(rect,block_Textures[j]));
		}
	}
}
void Tetris::SetScore()
{
	ExtractDigits(blockBuffer_Score, score);	
}

/*-------------------------------------------*/

void Tetris::DrawBackground()
{
	block_Background.Draw(gfx);
}
void Tetris::DrawField()
{
	for (int y = 0; y < fieldH; y++)
	{
		for (int x = 0; x < fieldW; x++)
		{
			uint i = y * fieldW + x;
			if (blockBuffer_Shown[i] == ' ')
			{
				continue;
			}
			else
			{
				blocks[i].Draw(gfx);
			}
		}
	}
}
void Tetris::DrawNextTetromino()
{
	for (int y = 0; y < tetroH; y++)
	{
		for (uint x = 0; x < tetroW; x++)
		{
			uint i = y * tetroW + x;
			if (tetromino[tetrominoNext][i] == '.')
			{
				continue;
			}
			else
			{
				blocks_Next[i].Draw(gfx);
			}
		}
	}
}
void Tetris::DrawScore()
{
	for (size_t i = 0; i < blockBuffer_Score.size(); i++)
	{
		blocks_Digits[blockBuffer_Score[i]][i].Draw(gfx);
	}
}
void Tetris::DrawTextPause()
{
	if (gameIsPaused)
	{		
		block_Pause.Draw(gfx);
	}
}
void Tetris::DrawTextGameOver()
{
	if (gameIsOver)
	{		
		block_GameOver.Draw(gfx);
	}
}
void Tetris::DrawBlur()
{
	if (gameIsPaused || gameIsOver) 
	{
		if (!isTesting)
		{
			Surface copy = gfx.CopySysBuffer();
			int width = copy.GetWidth();
			int height = copy.GetHeight();

			std::vector<Color> input;

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					input.push_back(copy.GetPixel(x, y));
				}
			}

			std::vector<uchar> in_red;
			std::vector<uchar> in_green;
			std::vector<uchar> in_blue;

			for (int i = 0; i < input.size(); i++)
			{
				in_red.push_back(input[i].GetR());
				in_green.push_back(input[i].GetG());
				in_blue.push_back(input[i].GetB());
			}

			std::vector<uchar> out_red(in_red.size());
			std::vector<uchar> out_green(in_green.size());
			std::vector<uchar> out_blue(in_blue.size());

			//gaussBlur_4(in_red, out_red, width, height, 1);
			//gaussBlur_4(in_green, out_green, width, height, 1);
			//gaussBlur_4(in_blue, out_blue, width, height, 1);

			for (int y = 0; y < height; y++)
			{
				for (int x = 0; x < width; x++)
				{
					uint i = y * width + x;

					const uchar r = out_red[i];
					const uchar g = out_green[i];
					const uchar b = out_blue[i];

					gfx.PutPixel(x, y, Color(r, g, b));
				}
			}
		}
		else
		{
			Surface* surface = new Surface(gfx.CopySysBuffer());
			int w = surface->GetWidth();
			int h = surface->GetHeight();

			std::vector<Color> input;

			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					input.push_back(surface->GetPixel(x, y));
				}
			}             

			delete surface;

			std::vector<Color> output;
				
			output = Blur(w, h, Blur(w, h, Blur(w, h, Blur(w, h, Blur(w,h,Blur(w, h, Blur(w, h, input)))))));

			for (int y = 0; y < h; y++)
			{
				for (int x = 0; x < w; x++)
				{
					uint i = y * w + x;
					gfx.PutPixel(x, y, output[i]);
				}
			}			
		}
		
	}
}

/*-------------------------------------------*/

int		Tetris::Rotate(int px, int py, int r)
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
bool	Tetris::DoesTetrisFit(int tetrisID, int rotation, int posX, int posY)
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
int		Tetris::Random(const int min, const int max)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);
}
void	Tetris::ExtractDigits(std::vector<unsigned int>& ints, const unsigned int num)
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
Color	Tetris::ConvertCharToColor(const char value)
{
	/*Color color;
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
	}*/

	return Colors::Black;
}
uint	Tetris::ConvertCharToInt(const char value)
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
void	Tetris::Benchmark(void* pFunction)
{
	std::ofstream file;
	file.open("Benchmark.txt"); // creates file and names it and opens for writing to

	const unsigned int number = 100;

	for (int i = 0; i < number; i++)
	{
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

		&pFunction;

		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

		file << i << " " << duration.count() << " microseconds" << std::endl;
	}
}
void	Tetris::BoxBlur(const Surface & input, std::vector<Color>& output)
{
	output.clear();

	const uint height = input.GetHeight();
	const uint width = input.GetWidth();

	for (uint y = 0; y < height; y++)
	{
		for (uint x = 0; x < width; x++)
		{
			uchar rTotal = 0;
			uchar gTotal = 0;
			uchar bTotal = 0;

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int currentX = x + col;
					int currentY = y + row;

					if (currentX > 0 && currentX < (int)width && currentY > 0 && currentY < (int)height)
					{
						Color c = input.GetPixel(currentX, currentY);

						uchar r = c.GetR();
						uchar g = c.GetG();
						uchar b = c.GetB();

						rTotal += r;
						gTotal += g;
						bTotal += b;
					}
				}
			}

			uint div = 9;

			uchar rFinal = rTotal / div;
			uchar gFinal = gTotal / div;
			uchar bFinal = bTotal / div;

			output.push_back( Color(rFinal, gFinal, bFinal) );
			
		}
	}
	assert(output.size() == (input.GetWidth() * input.GetHeight()));
}

const std::vector<Color> Tetris::Blur(const int w,const int h,const std::vector<Color>& input)
{
	const size_t sizeA = size_t(w) * h;
	const size_t sizeB = input.size();
	assert( sizeA == sizeB );

	Mat3 m;
	
	const float val = 1.0f / 9.0f;

	m.elements[0][0] = val;	// 0
	m.elements[0][1] = val;	// 1
	m.elements[0][2] = val;	// 2
	m.elements[1][0] = val;	// 3
	m.elements[1][1] = val;	// 4 center pixel
	m.elements[1][2] = val;	// 5
	m.elements[2][0] = val;	// 6
	m.elements[2][1] = val;	// 7
	m.elements[2][2] = val;	// 8

	//m.elements[0][0] = 0.1;	// 0
	//m.elements[0][1] = 0.4;	// 1
	//m.elements[0][2] = 0.1;	// 2
	//m.elements[1][0] = 0.4;	// 3
	//m.elements[1][1] = -2.0;	// 4 center pixel
	//m.elements[1][2] = 0.4;	// 5
	//m.elements[2][0] = 0.1;	// 6
	//m.elements[2][1] = 0.4;	// 7
	//m.elements[2][2] = 0.1;	// 8

	/*
	0 1 2 - 00 01 02
	3 4 5 - 10 11 12
	6 7 8 - 20 21 22
	*/

	std::vector<Color> output(sizeA, Colors::Black);
	
	Vec3 color = { 0.0f,0.0f,0.0f };

	typedef unsigned char uchar;

	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;

    for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			float rTotal = 0;
			float gTotal = 0;
			float bTotal = 0;

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					int cx = x + col;
					int cy = y + row;

					if (cx > 0 && cx < w && cy > 0 && cy < h)
					{
						uint i = cy * w + cx;
						Color c = input[i];

						float r = c.GetR();
						float g = c.GetG();
						float b = c.GetB();

						rTotal += (r *= m.elements[row + 1][col + 1]);
						gTotal += (g *= m.elements[row + 1][col + 1]);
						bTotal += (b *= m.elements[row + 1][col + 1]);
					}
				}
			}

			color = { rTotal,gTotal,bTotal };

			color /= 1.0f;

			red		= std::min<float>(255.0f, std::max<float>(0.0f, color.x));
			green	= std::min<float>(255.0f, std::max<float>(0.0f, color.y));
			blue	= std::min<float>(255.0f, std::max<float>(0.0f, color.z));

			/*redB	= (c.x > 255.0f) ? 255.0f : c.x;
			greenB	= (c.y > 255.0f) ? 255.0f : c.y;
			blueB	= (c.z > 255.0f) ? 255.0f : c.z;
			redB	= (c.x < 0.0f) ? 0.0f : c.x;
			greenB	= (c.y < 0.0f) ? 0.0f : c.y;
			blueB	= (c.z < 0.0f) ? 0.0f : c.z;*/

			uint i = y * w + x;

			output[i] = { uchar(red), uchar(green), uchar(blue) };			
		}
	}

	assert(input.size() == output.size());

	return output;
}


//auto Tetris::boxesForGauss(int sigma, int n)  // standard deviation, number of boxes
//{
//	float wIdeal = std::sqrt((12 * sigma*sigma / (float)n) + 1);  // Ideal averaging filter width 
//	int wl = (int)std::floor(wIdeal);  if (wl % 2 == 0) wl--;
//	int wu = wl + 2;
//
//	float mIdeal = (12 * sigma*sigma - (float)n * wl*wl - 4 * (float)n*wl - 3 * (float)n) / (-4 * wl - 4);
//	int m = (int)std::round(mIdeal);
//	// float sigmaActual = std::sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );
//
//	std::vector<int> sizes;  
//	
//	for (int i = 0; i < n; i++) 
//		sizes.push_back(i < m ? wl : wu);
//
//	return sizes;
//}
//
//void Tetris::gaussBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	auto bxs = boxesForGauss(r, 3);
//
//	boxBlur_4(scl, tcl, w, h, (bxs[0] - 1) / 2);
//	boxBlur_4(tcl, scl, w, h, (bxs[1] - 1) / 2);
//	boxBlur_4(scl, tcl, w, h, (bxs[2] - 1) / 2);
//}
//void Tetris::boxBlur_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	for (auto i = 0; i < scl.size(); i++) tcl[i] = scl[i];
//
//	boxBlurH_4(tcl, scl, w, h, r);
//	boxBlurT_4(scl, tcl, w, h, r);
//}
//void Tetris::boxBlurH_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	auto iarr = 1 / (r + r + 1);
//
//	for (auto i = 0; i < h; i++) 
//	{
//		auto ti = i * w;
//		auto li = ti;
//		auto ri = ti + r;
//		auto fv = scl[ti];
//		auto lv = scl[ti + w - 1];
//		auto val = (r + 1)*fv;
//
//		for (auto j = 0; j < r; j++)
//		{
//			val += scl[ti + j];
//		}
//
//		for (auto j = 0; j <= r; j++) 
//		{ 
//			val += scl[ri++] - fv;
//			tcl[ti++] = (int)std::round(val*iarr);
//		}
//
//		for (auto j = r + 1; j < w - r; j++)
//		{ 
//			val += scl[ri++] - scl[li++];
//			tcl[ti++] = (int)std::round(val*iarr);
//		}
//
//		for (auto j = w - r; j < w; j++) 
//		{ 
//			val += lv - scl[li++];
//			tcl[ti++] = (int)std::round(val*iarr);
//		}
//	}
//}
//void Tetris::boxBlurT_4(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	auto iarr = 1 / (r + r + 1);
//
//	for (auto i = 0; i < w; i++) 
//	{
//		auto ti = i, li = ti, ri = ti + r * w;
//		auto fv = scl[ti];
//		auto lv = scl[ti + w * (h - 1)];
//		auto val = (r + 1)*fv;
//
//		for (auto j = 0; j < r; j++)
//		{
//			val += scl[ti + j * w];
//		}
//
//		for (auto j = 0; j <= r; j++)
//		{ 
//			val += scl[ri] - fv;
//			tcl[ti] = (int)std::round(val*iarr);
//			ri += w; ti += w;
//		}
//
//		for (auto j = r + 1; j < h - r; j++)
//		{ 
//			val += scl[ri] - scl[li];
//			tcl[ti] = (int)std::round(val*iarr);
//			li += w; ri += w; ti += w; 
//		}
//
//		for (auto j = h - r; j < h; j++)
//		{ 
//			val += lv - scl[li];  
//			tcl[ti] = (int)std::round(val*iarr);
//			li += w; ti += w;
//		}
//	}
//}
//
//void Tetris::gaussBlur_1(std::vector<uchar> scl, std::vector<uchar> tcl, int w, int h, int r)
//{
//	// scl = source channel, tcl = target channel, width, height, radius
//
//	int rs = (int)std::ceil(r * 2.57);     // significant radius
//
//	for (int i = 0; i < h; i++)
//	{
//		for (int j = 0; j < w; j++)
//		{
//			int val = 0, wsum = 0;
//
//			for (int iy = i - rs; iy < i + rs + 1; iy++)
//			{
//				for (int ix = j - rs; ix < j + rs + 1; ix++)
//				{
//					int x = std::min<int>(w - 1, std::max<int>(0, ix));
//					int y = std::min<int>(h - 1, std::max<int>(0, iy));
//					int dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i);
//					int wght = (int)(std::exp(-dsq / (2 * r*r)) / (PI * 2 * r*r));
//					val += scl[y*w + x] * wght;  wsum += wght;
//				}
//			}
//			tcl[i*w + j] = (uchar)std::round(val / wsum);
//		}
//	}
//}