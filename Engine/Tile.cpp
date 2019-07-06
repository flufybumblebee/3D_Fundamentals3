#include "Tile.h"

Tile::Tile(const RectUI& position, std::shared_ptr<Surface> texture)
{
	block = { position,texture };
}

void Tile::SetValue(const unsigned int& new_value)
{
	value = new_value;
}
void Tile::SetIsRevealed(const bool& IS_REVEALED)
{
	is_revealed = IS_REVEALED;
}
void Tile::SetIsFlag(const bool& IS_FLAG)
{
	is_flag = IS_FLAG;
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

unsigned int Tile::GetValue() const
{
	return value;
}
bool Tile::GetIsRevealed() const
{
	return is_revealed;
}
bool Tile::GetIsFlag() const
{
	return is_flag;
}
RectUI Tile::GetPosition() const
{
	return block.GetPosition();
}
bool Tile::GetMouseOver() const
{
	return block.GetMouseOver();
}
void Tile::Reset()
{
	value = 0;
	is_flag = false;
	is_revealed = false;
}
void Tile::Draw(Graphics& gfx)
{
	block.Draw(gfx);
}