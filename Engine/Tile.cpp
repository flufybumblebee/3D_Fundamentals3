#include "Tile.h"

Tile::Tile(const RectUI& position, std::shared_ptr<Surface> texture)
{
	block = { position,texture };
}

void Tile::SetValue(const unsigned int& new_value)
{
	value = new_value;
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
void Tile::SetPosition(const RectUI& POSITION)
{
	block.SetPosition(POSITION);
}
void Tile::SetTexture(std::shared_ptr<Surface> texture)
{
	block.SetTexture(texture);
}
void Tile::SetMouseOver(Mouse& mouse)
{
	block.SetMouseOver(mouse);
}

unsigned int	Tile::Value() const
{
	return value;
}
bool			Tile::Revealed() const
{
	return is_revealed;
}
bool			Tile::Flag() const
{
	return is_flag;
}
bool Tile::Checked() const
{
	return is_checked;
}
bool			Tile::Mine() const
{
	return is_mine;
}
RectUI			Tile::Position() const
{
	return block.Position();
}
bool			Tile::MouseOver() const
{
	return block.MouseOver();
}

void Tile::Reset()
{
	value = 0;
	is_flag = false;
	is_revealed = false;
	is_checked = false;
	is_mine = false;
}
void Tile::Draw(Graphics& gfx)
{
	block.Draw(gfx);
}