#include "Block.h"

Block::Block(const RectUI& RECT, std::shared_ptr<Surface> texture)
	:
	rect(static_cast<RectF>(RECT)),
	pTex(texture)
{}
Block::Block(const Block& COPY)
	:
	rect(COPY.rect),
	pTex(COPY.pTex)
{}
Block::Block(Block&& block) noexcept
	:
	rect(block.rect),
	pTex(block.pTex)
{
	block.pTex = nullptr;
}
Block& Block::operator = (const Block& rhs)
{
	rect = rhs.rect;
	pTex = rhs.pTex;

	return *this;
}
Block& Block::operator = (Block&& rhs) noexcept
{
	if (&rhs != this)
	{
		rect = rhs.rect;
		pTex = rhs.pTex;

		rhs.pTex = nullptr;
	}

	return *this;
}
void Block::SetMouseOver(Mouse& mouse)
{
	mouse_over = rect.Contains(mouse.GetPos());	
}
bool Block::MouseOver() const
{
	return mouse_over;
}
RectUI Block::Rect() const
{
	return static_cast<RectUI>(rect);
}
void Block::SetRect(const RectUI& RECT)
{
	rect = static_cast<RectF>(RECT);
}
void Block::SetTexture(std::shared_ptr<Surface> texture)
{
	pTex = texture;
}
void Block::Draw(Graphics& gfx)
{
	gfx.DrawTriangleTex(
		{ {rect.left,	rect.top,		0.0f},{ 0.0f,0.0f } },
		{ {rect.right,	rect.top,		0.0f},{ 1.0f,0.0f } },
		{ {rect.right,	rect.bottom,	0.0f},{ 1.0f,1.0f } },
		*pTex);
	gfx.DrawTriangleTex(
		{ {rect.left,	rect.top,		0.0f},{ 0.0f,0.0f } },
		{ {rect.right,	rect.bottom,	0.0f},{ 1.0f,1.0f } },
		{ {rect.left,	rect.bottom,	0.0f},{ 0.0f,1.0f } },
		*pTex);
}