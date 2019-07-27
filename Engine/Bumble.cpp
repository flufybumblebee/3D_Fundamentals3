
#include "Bumble.h"

std::vector<Color> Bumble::Blur(const int& WIDTH, const int& HEIGHT, const std::vector<Color>& INPUT)
{
	assert(INPUT.size() == size_t(WIDTH) * HEIGHT);

	constexpr float val = 1.0f / 9.0f;

	std::array<std::array<float, 3u>, 3u> box = {	val,val,val,
													val,val,val,
													val,val,val };

	/*
	0 1 2  c  c  c
	3 4 5  o  o  o
	6 7 8  l  l  l
		   0  1  2
	row 0 00 01 02
	row 1 10 11 12
	row 2 20 21 22
	*/

	std::vector<Color> output;

	Vec3 pixel_out;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			pixel_out = { 0.0f,0.0f,0.0f };

			for (int row = -1; row <= 1; row++)
			{
				for (int col = -1; col <= 1; col++)
				{
					const int X = x + col;
					const int Y = y + row;

					if (X > 0 && X < WIDTH && Y > 0 && Y < HEIGHT)
					{
						const int INDEX = Y * WIDTH + X;
						const int ROW = row + 1;
						const int COL = col + 1;

						pixel_out.x += (INPUT[INDEX].GetR() * box[ROW][COL]);
						pixel_out.y += (INPUT[INDEX].GetG() * box[ROW][COL]);
						pixel_out.z += (INPUT[INDEX].GetB() * box[ROW][COL]);
					}
				}
			}

			output.push_back({
				static_cast<unsigned char>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.x))),
				static_cast<unsigned char>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.y))),
				static_cast<unsigned char>(std::min<float>(255.0f, std::max<float>(0.0f, pixel_out.z))) });
		}
	}

	assert(output.size() == INPUT.size());

	return output;
}

std::vector<Color> Bumble::ConvertSurfaceToColorVector(const Surface& surface)
{
	std::vector<Color> output;

	for (unsigned int y = 0u; y < surface.GetHeight(); y++)
	{
		for (unsigned int x = 0u; x < surface.GetWidth(); x++)
		{
			output.emplace_back(surface.GetPixel(x, y));
		}
	}

	return output;
}

Surface Bumble::CreateColorBlendTexture(const RectUI& RECTANGLE, const Color& color_start, const Color& color_end)
{
	Surface temp(RECTANGLE.GetWidth(), RECTANGLE.GetHeight());

	const Vec2	LINE = { static_cast<float>(RECTANGLE.GetWidth()),static_cast<float>(RECTANGLE.GetHeight()) };
	const float	LENGTH = LINE.Len();
	const Vec2	NORMAL = LINE.GetNormalized();

	float delta_red = static_cast<float>(color_end.GetR() - color_start.GetR());
	float delta_green = static_cast<float>(color_end.GetG() - color_start.GetG());
	float delta_blue = static_cast<float>(color_end.GetB() - color_start.GetB());

	Vec2 pixel;
	float length = 0.0f;
	Color c;

	for (unsigned int y = 0u; y < RECTANGLE.GetHeight(); y++)
	{
		for (unsigned int x = 0u; x < RECTANGLE.GetWidth(); x++)
		{
			pixel = { static_cast<float>(x),static_cast<float>(y) };
			length = NORMAL.DotProduct(pixel);

			c = Color(255,
				static_cast<unsigned char>(color_start.GetR() + delta_red * length / LENGTH),
				static_cast<unsigned char>(color_start.GetG() + delta_green * length / LENGTH),
				static_cast<unsigned char>(color_start.GetB() + delta_blue * length / LENGTH));

			temp.PutPixel(x, y, c);
		}
	}

	return temp;
}

Color Bumble::RandomColor()
{
	return Color(rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u), rnd::RandomInt(0u, 255u));
}
