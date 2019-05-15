#pragma once

#include "Block.h"
#include "Surface.h"
#include "Graphics.h"

#include <vector>
#include <string>

class Button
{
private:
	bool state = false;
	Block blockA;
	Block blockB;
public:
	Button() = default;
	Button(const bool& state, const Block& block_a, const Block& block_b)
		:
		state(state),
		blockA(block_a),
		blockB(block_b)
	{}
	Button(const Button& copy)
		:
		state(copy.state),
		blockA(copy.blockA),
		blockB(copy.blockB)
	{}
	Button(Button&& move) noexcept
		:
		state(std::move(move.state)),
		blockA(std::move(move.blockA)),
		blockB(std::move(move.blockB))
	{}
	Button& operator = (const Button& rhs)
	{
		state = rhs.state;
		blockA = rhs.blockA;
		blockB = rhs.blockB;

		return *this;
	}
	~Button() = default;
public:
	void Set(const Button& button)
	{
		state = button.state;
		blockA = button.blockA;
		blockB = button.blockB;
	}
	void SetState(const bool& new_state)
	{
		state = new_state;
	}
	void SetTextures(Surface* pTexA, Surface* pTexB)
	{
		blockA.SetTexture(pTexA);
		blockB.SetTexture(pTexA);
	}
	bool GetState() const
	{
		return state;
	}
	void Draw(Graphics& gfx)
	{
		if (!state)
		{
			blockA.Draw(gfx);
		}
		else
		{
			blockB.Draw(gfx);
		}
	}
};

class Button2
{
private:
	bool state = false;	
	Button buttonA;
	Button buttonB;
public:
	Button2() = default;
	Button2(const bool& state,const Button& button_a, const Button& button_b)
		:
		state(state),
		buttonA(button_a),
		buttonB(button_b)
	{}
	Button2(const Button2& copy)
		:
		state(copy.state),
		buttonA(copy.buttonA),
		buttonB(copy.buttonB)
	{}
	Button2(Button2&& move) noexcept
		:
		state(std::move(move.state)),
		buttonA(std::move(move.buttonA)),
		buttonB(std::move(move.buttonB))
	{}
	Button2& operator = (const Button2& rhs)
	{
		state = rhs.state;
		buttonA = rhs.buttonA;
		buttonB = rhs.buttonB;

		return *this;
	}
	~Button2() = default;
public:
	void Set(const bool& new_state, const Button& button_a, const Button& button_b)
	{
		state = new_state;
		buttonA = button_a;
		buttonB = button_b;
	}
	void SetButtonA(bool state, Surface* pTexA, Surface* pTexB)
	{
		buttonA.SetState(state);
		buttonA.SetTextures(pTexA,pTexB);
	}
	void SetButtonB(bool state, Surface* pTexA, Surface* pTexB)
	{
		buttonB.SetState(state);
		buttonB.SetTextures(pTexA, pTexB);
	}

	void SetState(const bool& new_state)
	{
		state = new_state;
	}
	void SetButtonAState(const bool& new_state)
	{
		buttonA.SetState(new_state);
	}
	void SetButtonBState(const bool& new_state)
	{
		buttonB.SetState(new_state);
	}

	bool GetState() const
	{
		return state;
	}
	bool GetButtonAState() const
	{
		return buttonA.GetState();
	}
	bool GetButtonBState() const
	{
		return buttonB.GetState();
	}

	void Draw(Graphics& gfx)
	{
		if (!state)
		{			
			buttonA.Draw(gfx);
		}
		else
		{
			buttonB.Draw(gfx);
		}
	}
};