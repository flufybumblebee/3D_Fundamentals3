#pragma once

#include "Graphics.h"
#include "Vec2.h"

class Digit
{
private:
	const Vec2 TOP_LEFT		= { -1.0f, -1.0f };
	const Vec2 TOP_RIGHT	= { +1.0f, -1.0f };
	const Vec2 MIDDLE_LEFT	= { -1.0f, +0.0f };
	const Vec2 MIDDLE_RIGHT	= { +1.0f, +0.0f };
	const Vec2 BOTTOM_LEFT	= { -1.0f, +1.0f };
	const Vec2 BOTTOM_RIGHT	= { +1.0f, +1.0f };

	/*
	 _
	|_|
	|_|

	*/

public:
	void Draw(const size_t& DIGIT, const Vec2& POSITION, const float& SIZE, Graphics& gfx)
	{
		const Vec2 TL = TOP_LEFT		* SIZE + POSITION;
		const Vec2 TR = TOP_RIGHT		* SIZE + POSITION;
		const Vec2 ML = MIDDLE_LEFT		* SIZE + POSITION;
		const Vec2 MR = MIDDLE_RIGHT	* SIZE + POSITION;
		const Vec2 BL = BOTTOM_LEFT		* SIZE + POSITION;
		const Vec2 BR = BOTTOM_RIGHT	* SIZE + POSITION;

		const Color C = Colors::White;

		switch (DIGIT)
		{
		case 0:
			gfx.DrawLine(TL, TR, C);
			//gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			gfx.DrawLine(TL, ML, C);
			gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break;
		case 1:
			//gfx.DrawLine(TL, TR, C);
			//gfx.DrawLine(ML, MR, C);
			//gfx.DrawLine(BL, BR, C);

			//gfx.DrawLine(TL, ML, C);
			//gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break;
		case 2:
			gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			//gfx.DrawLine(TL, ML, C);
			gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			//gfx.DrawLine(MR, BR, C);
			break;
		case 3:
			gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			//gfx.DrawLine(TL, ML, C);
			//gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break; 
		case 4:
			//gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			//gfx.DrawLine(BL, BR, C);

			gfx.DrawLine(TL, ML, C);
			//gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break; 
		case 5:
			gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			gfx.DrawLine(TL, ML, C);
			//gfx.DrawLine(ML, BL, C);
			//gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break; 
		case 6:
			gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			gfx.DrawLine(TL, ML, C);
			gfx.DrawLine(ML, BL, C);
			//gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break; 
		case 7:
			gfx.DrawLine(TL, TR, C);
			//gfx.DrawLine(ML, MR, C);
			//gfx.DrawLine(BL, BR, C);

			//gfx.DrawLine(TL, ML, C);
			//gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break; 
		case 8:
			gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			gfx.DrawLine(TL, ML, C);
			gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break; 
		case 9:
			gfx.DrawLine(TL, TR, C);
			gfx.DrawLine(ML, MR, C);
			gfx.DrawLine(BL, BR, C);

			gfx.DrawLine(TL, ML, C);
			//gfx.DrawLine(ML, BL, C);
			gfx.DrawLine(TR, MR, C);
			gfx.DrawLine(MR, BR, C);
			break;
		default:
			break;
		}
	}
};
