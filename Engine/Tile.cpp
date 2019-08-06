#include "Tile.h"

Tile::Tile(std::vector<Block> BLOCKS)
	:
	blocks(std::move(BLOCKS))
{
	for (auto& b : BLOCKS)
	{
		b.SetTexture(nullptr);
	}

	BLOCKS.clear();
}

void Tile::SetValue(const unsigned int& VALUE)
{
	value = VALUE;
	if (value == 9u) { is_mine = true; }
}
void Tile::SetRevealed(const bool& IS_REVEALED)
{
	is_revealed = IS_REVEALED;
}
void Tile::SetFlag(const bool& IS_FLAG)
{
	is_flag = IS_FLAG;
}
void Tile::SetChecked(const bool& IS_CHECKED)
{
	is_checked = IS_CHECKED;
}
void Tile::SetExploded(const bool& IS_EXPLODED)
{
	is_exploded = IS_EXPLODED;
}

void Tile::SetMouseOver(Mouse& mouse)
{
	mouseover = mouseover_rect.Contains(mouse.GetPos());
}

void Tile::SetMouseoverRect(const RectUI& MOUSEOVER_RECT)
{
	mouseover_rect = MOUSEOVER_RECT;
}

void Tile::SetGameOver(const bool& IS_GAMEOVER)
{
	gameover = IS_GAMEOVER;
}

void Tile::SetRect(const size_t& INDEX, const RectUI& RECT)
{
	blocks[INDEX].SetRect(RECT);
}

void Tile::SetTexture(const size_t& INDEX, std::shared_ptr<Surface> texture)
{
	blocks[INDEX].SetTexture(texture);
}

unsigned int Tile::Value() const
{
	return value;
}
bool Tile::Revealed() const
{
	return is_revealed;
}
bool Tile::Flag() const
{
	return is_flag;
}
bool Tile::Exploded() const
{
	return is_exploded;
}
bool Tile::Checked() const
{
	return is_checked;
}
bool Tile::Mine() const
{
	return is_mine;
}
bool Tile::MouseOver() const
{
	return mouseover;
}

RectUI Tile::MouseoverRect() const
{
	return mouseover_rect;
}

RectUI Tile::Rect(const size_t INDEX) const
{
	return blocks[INDEX].Rect();
}

void Tile::Reset()
{
	value = 0u;
	is_revealed		= false;
	is_flag			= false;
	is_mine			= false;
	is_checked		= false;
	is_flag_wrong	= false;
	is_exploded		= false;
	mouseover		= false;
	gameover		= false;
}

void Tile::Draw(Graphics& gfx)
{	
	for (size_t i = 1; i < blocks.size(); i++)
	{
		if (!gameover && mouseover)
		{
			blocks[0].Draw(gfx);
		}
		blocks[i].Draw(gfx);
	}

	/*if (is_flag)
	{
		blocks[FLAG].Draw(gfx);
	}
	else if (is_exploded)
	{

	}*/
}