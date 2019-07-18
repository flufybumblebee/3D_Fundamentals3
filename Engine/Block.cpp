#include "Block.h"

Block::Block(const RectUI& POSITION, std::shared_ptr<Surface> texture)
	:
	position(static_cast<RectF>(POSITION)),
	pTex(texture)
{}
Block::Block(const Block& COPY)
	:
	position(COPY.position),
	pTex(COPY.pTex)
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
bool Block::MouseOver() const
{
	return mouse_over;
}
RectUI Block::Position() const
{
	return static_cast<RectUI>(position);
}
void Block::SetPosition(const RectUI& POSITION)
{
	position = static_cast<RectF>(POSITION);
}
void Block::SetTexture(std::shared_ptr<Surface> texture)
{
	pTex = texture;
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