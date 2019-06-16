#include "Block.h"

Block::Block(const RectUI& position, std::shared_ptr<Surface> pTex)
	:
	position(static_cast<RectF>(position)),
	pTex(pTex)
{}
Block::Block(const Block& copy)
	:
	position(copy.position),
	pTex(copy.pTex)
{}
Block::Block(Block&& block) noexcept
	:
	position(block.position),
	pTex(block.pTex)
{
	block.pTex = nullptr;
}
Block& Block::operator = (const Block& rhs)
{
	position = rhs.position;
	pTex = rhs.pTex;

	return *this;
}
Block& Block::operator = (Block&& rhs) noexcept
{
	if (&rhs != this)
	{
		position = rhs.position;
		pTex = rhs.pTex;

		rhs.pTex = nullptr;
	}

	return *this;
}
void Block::SetMouseOver(Mouse& mouse)
{
	mouse_over = position.Contains(mouse.GetPos());	
}
bool Block::GetMouseOver() const
{
	return mouse_over;
}
RectUI Block::GetPosition() const
{
	return RectUI(position);
}
void Block::SetPosition(const RectUI& new_position)
{
	position = static_cast<RectF>(new_position);
}
void Block::SetTexture(std::shared_ptr<Surface> new_texture)
{
	pTex = new_texture;
}
void Block::Draw(Graphics& gfx)
{
	gfx.DrawTriangleTex(
		{ {position.left,	position.top,		0.0f},{ 0.0f,0.0f } },
		{ {position.right,	position.top,		0.0f},{ 1.0f,0.0f } },
		{ {position.right,	position.bottom,	0.0f},{ 1.0f,1.0f } },
		*pTex);
	gfx.DrawTriangleTex(
		{ {position.left,	position.top,		0.0f},{ 0.0f,0.0f } },
		{ {position.right,	position.bottom,	0.0f},{ 1.0f,1.0f } },
		{ {position.left,	position.bottom,	0.0f},{ 0.0f,1.0f } },
		*pTex);
}