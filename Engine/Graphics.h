/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "GDIPlusManager.h"
#include "ChiliException.h"
#include "Surface.h"
#include "Colors.h"
#include "Vec2.h"
#include "ZBuffer.h"
#include "TexVertex.h"

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	~Graphics();

public:
	void BeginFrame();
	void EndFrame();

public:
	void PutPixel( int x,int y,int r,int g,int b )
	{
		PutPixel( x,y,{ unsigned char( r ),unsigned char( g ),unsigned char( b ) } );
	}
	void PutPixel( int x,int y,Color c )
	{
		sysBuffer.PutPixel( x,y,c );
	}
	Surface CopySysBuffer() const
	{
		Surface copy(ScreenWidth, ScreenHeight);
		copy.Copy(sysBuffer);
		return std::move(copy);
	}
	void DrawLineDepth(ZBuffer& zb, Vec3& v0, Vec3& v1, Color c);
	
	void DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c);
	void DrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
	
	void DrawFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c);
	void DrawFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c);

	void DrawFlatTopTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
	void DrawFlatBottomTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex);
	void DrawFlatTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex,
		const TexVertex& dv0, const TexVertex& dv1, TexVertex& itEdge1);

	void PutPixelAlpha(unsigned int x, unsigned int y, const Color src);
	
	void DrawLine(int x1, int y1, int x2, int y2, Color c);

	void DrawRect(int x1, int y1, int x2, int y2, Color c);
	void DrawRect(RectI rect, Color c)
	{
		DrawRect(rect.left, rect.top, rect.right, rect.bottom, c);
	}
	//void DrawRect(int x, int y, int width, int height, Color c);

private:
	GDIPlusManager										gdipMan;
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Surface												sysBuffer;
public:
	static constexpr unsigned int ScreenWidth = 800u;
	static constexpr unsigned int ScreenHeight = 600u;
};