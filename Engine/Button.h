//#pragma once
//
//#include "Block.h"
//#include "Surface.h"
//#include "Graphics.h"
//
//#include <vector>
//#include <string>
//
//class Button
//{
//private:
//	bool state = false;
//	Block blockA;
//	Block blockB;
//public:
//	Button() = default;
//	Button(const bool& state, const Block& block_a, const Block& block_b)
//		:
//		state(state),
//		blockA(block_a),
//		blockB(block_b)
//	{}
//	Button(const Button& copy)
//		:
//		state(copy.state),
//		blockA(copy.blockA),
//		blockB(copy.blockB)
//	{}
//	Button(Button&& move) noexcept
//		:
//		state(std::move(move.state)),
//		blockA(std::move(move.blockA)),
//		blockB(std::move(move.blockB))		
//	{
//		move.blockA.SetTexture(nullptr);
//		move.blockB.SetTexture(nullptr);
//	}
//	Button& operator = (const Button& rhs)
//	{
//		state = rhs.state;
//		blockA = rhs.blockA;
//		blockB = rhs.blockB;
//
//		return *this;
//	}
//	~Button() = default;
//public:
//	void Set(const Button& button)
//	{
//		state = button.state;
//		blockA = button.blockA;
//		blockB = button.blockB;
//	}
//	void SetState(const bool& new_state)
//	{
//		state = new_state;
//	}
//	void SetTextures(Surface* pTexA, Surface* pTexB)
//	{
//		blockA.SetTexture(pTexA);
//		blockB.SetTexture(pTexB);
//	}
//	bool GetState() const
//	{
//		return state;
//	}
//	void Draw(Graphics& gfx)
//	{
//		if (!state)
//		{
//			blockA.Draw(gfx);
//		}
//		else
//		{
//			blockB.Draw(gfx);
//		}
//	}
//};