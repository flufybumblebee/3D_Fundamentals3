#include "Block.h"

Block::Block(const RectUI& position, std::shared_ptr<Surface> pTex)
	:
	pos(static_cast<RectF>(position)),
	pTex(pTex)
{}
Block::Block(const Block& copy)
	:
	pos(copy.pos),
	pTex(copy.pTex)
{}
Block::Block(Block&& block) noexcept
	:
	pos(block.pos),
	pTex(block.pTex)
{
	block.pTex = nullptr;
}
Block& Block::operator = (const Block& rhs)
{
	pos = rhs.pos;
	pTex = rhs.pTex;

	return *this;
}
Block& Block::operator = (Block&& rhs) noexcept
{
	if (&rhs != this)
	{
		pos = rhs.pos;
		pTex = rhs.pTex;

		rhs.pTex = nullptr;
	}

	return *this;
}
void Block::SetMouseOver(Mouse& mouse)
{
	mouse_over = pos.Contains(mouse.GetPos());	
}
bool Block::GetMouseOver() const
{
	return mouse_over;
}
RectUI Block::GetPosition() const
{
	return RectUI(pos);
}
void Block::SetPosition(const RectUI& new_position)
{
	pos = static_cast<RectF>(new_position);
}
void Block::SetTexture(std::shared_ptr<Surface> new_texture)
{
	pTex = new_texture;
}
void Block::Draw(Graphics& gfx)
{
	gfx.DrawTriangleTex(
		{ {pos.left,	pos.top,	0.0f},{ 0.0f,0.0f } },
		{ {pos.right,	pos.top,	0.0f},{ 1.0f,0.0f } },
		{ {pos.right,	pos.bottom,	0.0f},{ 1.0f,1.0f } },
		*pTex);
	gfx.DrawTriangleTex(
		{ {pos.left,	pos.top,	0.0f},{ 0.0f,0.0f } },
		{ {pos.right,	pos.bottom,	0.0f},{ 1.0f,1.0f } },
		{ {pos.left,	pos.bottom,	0.0f},{ 0.0f,1.0f } },
		*pTex);
}