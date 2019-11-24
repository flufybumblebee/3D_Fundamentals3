/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
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
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include <functional>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
	:
	sysBuffer( ScreenWidth,ScreenHeight )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL	featureLevelsRequested = D3D_FEATURE_LEVEL_9_1;
	UINT				numLevelsRequested = 1;
	D3D_FEATURE_LEVEL	featureLevelsSupported;
	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef _DEBUG
#ifdef USE_DIRECT3D_DEBUG_RUNTIME
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		&featureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		&featureLevelsSupported,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler button for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}
}

Graphics::~Graphics()
{
	// clear the button of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// perform the copy line-by-line
	sysBuffer.Present( mappedSysBufferTexture.RowPitch,
		reinterpret_cast<BYTE*>(mappedSysBufferTexture.pData) );
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
	}
}

void Graphics::BeginFrame()
{
	sysBuffer.Clear( Color(255,255,55) );
}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}

void Graphics::DrawLineDepth(ZBuffer& zb, Vec3& v0, Vec3& v1, Color c)
{
	float dx = v1.x - v0.x;
	float dy = v1.y - v0.y;

	if (dy == 0.0f && dx == 0.0f)
	{
	}
	else if (abs(dy) > abs(dx))
	{
		if (dy < 0.0f)
		{
			std::swap(v0, v1);
			dy = -dy;
		}

		const auto dv = (v1 - v0) / dy;
		for (auto v = v0; v.y < v1.y; v += dv)
		{
			const auto x = int(v.x);
			const auto y = int(v.y);
			if (x < 0 || x >= Graphics::ScreenWidth || y < 0 || y >= Graphics::ScreenHeight)
			{
				continue;
			}
			if (zb.TestAndSet(x, y, v.z))
			{
				PutPixel(x, y, c);
			}
		}
	}
	else
	{
		if (dx < 0.0f)
		{
			std::swap(v0, v1);
			dx = -dx;
		}

		const auto dv = (v1 - v0) / dx;
		for (auto v = v0; v.x < v1.x; v += dv)
		{
			const auto x = int(v.x);
			const auto y = int(v.y);
			if (x < 0 || x >= Graphics::ScreenWidth || y < 0 || y >= Graphics::ScreenHeight)
			{
				continue;
			}
			if (zb.TestAndSet(x, y, v.z))
			{
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c)
{
	// using pointers so we can swap (for sorting purposes)
	const Vec2* pv0 = &v0;
	const Vec2* pv1 = &v1;
	const Vec2* pv2 = &v2;

	// sorting vertices by y
	if (pv1->y < pv0->y) std::swap(pv0, pv1);
	if (pv2->y < pv1->y) std::swap(pv1, pv2);
	if (pv1->y < pv0->y) std::swap(pv0, pv1);

	if (pv0->y == pv1->y) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->x < pv0->x) std::swap(pv0, pv1);
		DrawFlatTopTriangle(*pv0, *pv1, *pv2, c);
	}
	else if (pv1->y == pv2->y) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->x < pv1->x) std::swap(pv1, pv2);
		DrawFlatBottomTriangle(*pv0, *pv1, *pv2, c);
	}
	else // general triangle
	{
		// find splitting vertex
		const float alphaSplit =
			(pv1->y - pv0->y) /
			(pv2->y - pv0->y);
		const Vec2 vi = *pv0 + (*pv2 - *pv0) * alphaSplit;

		if (pv1->x < vi.x) // major right
		{
			DrawFlatBottomTriangle(*pv0, *pv1, vi, c);
			DrawFlatTopTriangle(*pv1, vi, *pv2, c);
		}
		else // major left
		{
			DrawFlatBottomTriangle(*pv0, vi, *pv1, c);
			DrawFlatTopTriangle(vi, *pv1, *pv2, c);
		}
	}
}
void Graphics::DrawFlatTopTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c)
{
	// calulcate slopes in screen space
	const float m0 = (v2.x - v0.x) / (v2.y - v0.y);
	const float m1 = (v2.x - v1.x) / (v2.y - v1.y);

	// calculate start and end scanlines
	const int yStart = (int)ceil(v0.y - 0.5f);
	const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = yStart; y < yEnd; y++)
	{
		// caluclate start and end points (x-coords)
		// add 0.5 to y value because we're calculating based on pixel CENTERS
		const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		const float px1 = m1 * (float(y) + 0.5f - v1.y) + v1.x;

		// calculate start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

		for (int x = xStart; x < xEnd; x++)
		{
			PutPixel(x, y, c);
		}
	}
}
void Graphics::DrawFlatBottomTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, Color c)
{
	// calulcate slopes in screen space
	const float m0 = (v1.x - v0.x) / (v1.y - v0.y);
	const float m1 = (v2.x - v0.x) / (v2.y - v0.y);

	// calculate start and end scanlines
	const int yStart = (int)ceil(v0.y - 0.5f);
	const int yEnd = (int)ceil(v2.y - 0.5f); // the scanline AFTER the last line drawn

	for (int y = yStart; y < yEnd; y++)
	{
		// caluclate start and end points
		// add 0.5 to y value because we're calculating based on pixel CENTERS
		const float px0 = m0 * (float(y) + 0.5f - v0.y) + v0.x;
		const float px1 = m1 * (float(y) + 0.5f - v0.y) + v0.x;

		// calculate start and end pixels
		const int xStart = (int)ceil(px0 - 0.5f);
		const int xEnd = (int)ceil(px1 - 0.5f); // the pixel AFTER the last pixel drawn

		for (int x = xStart; x < xEnd; x++)
		{
			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex)
{
	// using pointers so we can swap (for sorting purposes)
	const TexVertex* pv0 = &v0;
	const TexVertex* pv1 = &v1;
	const TexVertex* pv2 = &v2;

	// sorting vertices by y
	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
	if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

	if (pv0->pos.y == pv1->pos.y) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);
		DrawFlatTopTriangleTex(*pv0, *pv1, *pv2, tex);
	}
	else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);
		DrawFlatBottomTriangleTex(*pv0, *pv1, *pv2, tex);
	}
	else // general triangle
	{
		// find splitting vertex
		const float alphaSplit =
			(pv1->pos.y - pv0->pos.y) /
			(pv2->pos.y - pv0->pos.y);
		const TexVertex vi = pv0->InterpolateTo(*pv2, alphaSplit);

		if (pv1->pos.x < vi.pos.x) // major right
		{
			DrawFlatBottomTriangleTex(*pv0, *pv1, vi, tex);
			DrawFlatTopTriangleTex(*pv1, vi, *pv2, tex);
		}
		else // major left
		{
			DrawFlatBottomTriangleTex(*pv0, vi, *pv1, tex);
			DrawFlatTopTriangleTex(vi, *pv1, *pv2, tex);
		}
	}
}
void Graphics::DrawFlatTopTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex)
{
	// calulcate dVertex / dy
	const float delta_y = v2.pos.y - v0.pos.y;
	const TexVertex dv0 = (v2 - v0) / delta_y;
	const TexVertex dv1 = (v2 - v1) / delta_y;

	// create right edge interpolant
	TexVertex itEdge1 = v1;

	// call the flat triangle render routine
	DrawFlatTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1);
}
void Graphics::DrawFlatBottomTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex)
{
	// calulcate dVertex / dy
	const float delta_y = v2.pos.y - v0.pos.y;
	const TexVertex dv0 = (v1 - v0) / delta_y;
	const TexVertex dv1 = (v2 - v0) / delta_y;

	// create right edge interpolant
	TexVertex itEdge1 = v0;

	// call the flat triangle render routine
	DrawFlatTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1);
}
void Graphics::DrawFlatTriangleTex(const TexVertex& v0, const TexVertex& v1, const TexVertex& v2, const Surface& tex,
	const TexVertex& dv0, const TexVertex& dv1, TexVertex& itEdge1)
{
	// create edge interpolant for left edge (always v0)
	TexVertex itEdge0 = v0;

	// calculate start and end scanlines
	const int yStart = std::max((int)ceil(v0.pos.y - 0.5f),0);
	const int yEnd = std::min((int)ceil(v2.pos.y - 0.5f),(int)ScreenHeight - 1); // the scanline AFTER the last line drawn

	// do interpolant prestep
	itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
	itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

	// init tex width/height and clamp values
	const float tex_width = float(tex.GetWidth());
	const float tex_height = float(tex.GetHeight());
	const float tex_clamp_x = tex_width - 1.0f;
	const float tex_clamp_y = tex_height - 1.0f;

	for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
	{
		// calculate start and end pixels
		const int xStart = std::max((int)ceil(itEdge0.pos.x - 0.5f),0);
		const int xEnd = std::min((int)ceil(itEdge1.pos.x - 0.5f),(int)ScreenWidth - 1); // the pixel AFTER the last pixel drawn

		// calculate scanline dTexCoord / dx
		const Vec2 dtcLine = (itEdge1.tc - itEdge0.tc) / (itEdge1.pos.x - itEdge0.pos.x);

		// create scanline tex coord interpolant and prestep
		Vec2 itcLine = itEdge0.tc + dtcLine * (float(xStart) + 0.5f - itEdge0.pos.x);

		for (int x = xStart; x < xEnd; x++, itcLine += dtcLine)
		{
			PutPixelAlpha(x, y, tex.GetPixel(
				int(std::min(itcLine.x * tex_width, tex_clamp_x)),
				int(std::min(itcLine.y * tex_height, tex_clamp_y))));
			// need std::min b/c tc.x/y == 1.0, we'll read off edge of tex
			// and with fp err, tc.x/y can be > 1.0 (by a tiny amount)
		}
	}
}

void Graphics::DrawTriangleTex(
	const TexVertex& v0,
	const TexVertex& v1,
	const TexVertex& v2,
	const Surface& tex,
	const unsigned int alpha)
{
	// using pointers so we can swap (for sorting purposes)
	const TexVertex* pv0 = &v0;
	const TexVertex* pv1 = &v1;
	const TexVertex* pv2 = &v2;

	// sorting vertices by y
	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);
	if (pv2->pos.y < pv1->pos.y) std::swap(pv1, pv2);
	if (pv1->pos.y < pv0->pos.y) std::swap(pv0, pv1);

	if (pv0->pos.y == pv1->pos.y) // natural flat top
	{
		// sorting top vertices by x
		if (pv1->pos.x < pv0->pos.x) std::swap(pv0, pv1);
		DrawFlatTopTriangleTex(*pv0, *pv1, *pv2, tex, alpha);
	}
	else if (pv1->pos.y == pv2->pos.y) // natural flat bottom
	{
		// sorting bottom vertices by x
		if (pv2->pos.x < pv1->pos.x) std::swap(pv1, pv2);
		DrawFlatBottomTriangleTex(*pv0, *pv1, *pv2, tex, alpha);
	}
	else // general triangle
	{
		// find splitting vertex
		const float alphaSplit =
			(pv1->pos.y - pv0->pos.y) /
			(pv2->pos.y - pv0->pos.y);
		const TexVertex vi = pv0->InterpolateTo(*pv2, alphaSplit);

		if (pv1->pos.x < vi.pos.x) // major right
		{
			DrawFlatBottomTriangleTex(*pv0, *pv1, vi, tex,alpha);
			DrawFlatTopTriangleTex(*pv1, vi, *pv2, tex, alpha);
		}
		else // major left
		{
			DrawFlatBottomTriangleTex(*pv0, vi, *pv1, tex, alpha);
			DrawFlatTopTriangleTex(vi, *pv1, *pv2, tex, alpha);
		}
	}
}
void Graphics::DrawFlatTopTriangleTex(
	const TexVertex& v0,
	const TexVertex& v1,
	const TexVertex& v2,
	const Surface& tex,
	const unsigned int alpha)
{
	// calulcate dVertex / dy
	const float delta_y = v2.pos.y - v0.pos.y;
	const TexVertex dv0 = (v2 - v0) / delta_y;
	const TexVertex dv1 = (v2 - v1) / delta_y;

	// create right edge interpolant
	TexVertex itEdge1 = v1;

	// call the flat triangle render routine
	DrawFlatTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1, alpha);
}
void Graphics::DrawFlatBottomTriangleTex(
	const TexVertex& v0,
	const TexVertex& v1,
	const TexVertex& v2,
	const Surface& tex,
	const unsigned int alpha)
{
	// calulcate dVertex / dy
	const float delta_y = v2.pos.y - v0.pos.y;
	const TexVertex dv0 = (v1 - v0) / delta_y;
	const TexVertex dv1 = (v2 - v0) / delta_y;

	// create right edge interpolant
	TexVertex itEdge1 = v0;

	// call the flat triangle render routine
	DrawFlatTriangleTex(v0, v1, v2, tex, dv0, dv1, itEdge1, alpha);
}
void Graphics::DrawFlatTriangleTex(
	const TexVertex& v0,
	const TexVertex& v1,
	const TexVertex& v2,
	const Surface& tex,
	const TexVertex& dv0,
	const TexVertex& dv1,
	TexVertex& itEdge1,
	const unsigned int alpha)
{
	// create edge interpolant for left edge (always v0)
	TexVertex itEdge0 = v0;

	// calculate start and end scanlines
	const int yStart = std::max((int)ceil(v0.pos.y - 0.5f), 0);
	const int yEnd = std::min((int)ceil(v2.pos.y - 0.5f), (int)ScreenHeight - 1); // the scanline AFTER the last line drawn

	// do interpolant prestep
	itEdge0 += dv0 * (float(yStart) + 0.5f - v0.pos.y);
	itEdge1 += dv1 * (float(yStart) + 0.5f - v0.pos.y);

	// init tex width/height and clamp values
	const float tex_width = float(tex.GetWidth());
	const float tex_height = float(tex.GetHeight());
	const float tex_clamp_x = tex_width - 1.0f;
	const float tex_clamp_y = tex_height - 1.0f;

	for (int y = yStart; y < yEnd; y++, itEdge0 += dv0, itEdge1 += dv1)
	{
		// calculate start and end pixels
		const int xStart = std::max((int)ceil(itEdge0.pos.x - 0.5f), 0);
		const int xEnd = std::min((int)ceil(itEdge1.pos.x - 0.5f), (int)ScreenWidth - 1); // the pixel AFTER the last pixel drawn

		// calculate scanline dTexCoord / dx
		const Vec2 dtcLine = (itEdge1.tc - itEdge0.tc) / (itEdge1.pos.x - itEdge0.pos.x);

		// create scanline tex coord interpolant and prestep
		Vec2 itcLine = itEdge0.tc + dtcLine * (float(xStart) + 0.5f - itEdge0.pos.x);

		for (int x = xStart; x < xEnd; x++, itcLine += dtcLine)
		{
			PutPixelAlpha(x, y, tex.GetPixel(
				int(std::min(itcLine.x * tex_width, tex_clamp_x)),
				int(std::min(itcLine.y * tex_height, tex_clamp_y))),alpha);
			// need std::min b/c tc.x/y == 1.0, we'll read off edge of tex
			// and with fp err, tc.x/y can be > 1.0 (by a tiny amount)
		}
	}
}

void Graphics::PutPixelAlpha(unsigned int x, unsigned int y, const Color dst)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < ScreenWidth);
	assert(y < ScreenHeight);

	const Color src = sysBuffer.GetPixel(x, y);
	// blend channels
	const unsigned char rsltRed = (dst.GetR() * dst.GetA() + src.GetR() * (255u - dst.GetA())) / 256u;
	const unsigned char rsltGreen = (dst.GetG() * dst.GetA() + src.GetG() * (255u - dst.GetA())) / 256u;
	const unsigned char rsltBlue = (dst.GetB() * dst.GetA() + src.GetB() * (255u - dst.GetA())) / 256u;

	// pack channels back into pixel and fire pixel onto surface
	PutPixel(x, y, { rsltRed,rsltGreen,rsltBlue });
}

void Graphics::PutPixelAlpha(unsigned int x, unsigned int y, const Color dst, const unsigned int alpha)
{
	assert(x >= 0);
	assert(y >= 0);
	assert(x < ScreenWidth);
	assert(y < ScreenHeight);

	const Color src = sysBuffer.GetPixel(x, y);
	// blend channels
	const unsigned char rsltRed = (dst.GetR() * alpha + src.GetR() * (255u - alpha)) / 256u;
	const unsigned char rsltGreen = (dst.GetG() * alpha + src.GetG() * (255u - alpha)) / 256u;
	const unsigned char rsltBlue = (dst.GetB() * alpha + src.GetB() * (255u - alpha)) / 256u;

	// pack channels back into pixel and fire pixel onto surface
	PutPixel(x, y, { rsltRed,rsltGreen,rsltBlue });
}

void Graphics::DrawLine(int x1, int y1, int x2, int y2, Color c)
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if (dy == 0 && dx == 0)
	{
		PutPixel(x1, y1, c);
	}
	else if (abs(dy) > abs(dx))
	{
		if (dy < 0)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dx / (float)dy;
		const float b = x1 - m * y1;
		for (int y = y1; y <= y2; y = y + 1)
		{
			int x = (int)(m * y + b + 0.5f);
					
			PutPixel(x, y, c);			
		}
	}
	else
	{
		if (dx < 0)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dy / (float)dx;
		const float b = y1 - m * x1;
		for (int x = x1; x <= x2; x = x + 1)
		{
			int y = (int)(m * x + b + 0.5f);

			PutPixel(x, y, c);
		}
	}
}

void Graphics::DrawLineAlpha(int x1, int y1, int x2, int y2, Color c)
{
	const int dx = x2 - x1;
	const int dy = y2 - y1;

	if (dy == 0 && dx == 0)
	{
		PutPixelAlpha(x1, y1, c);
	}
	else if (abs(dy) > abs(dx))
	{
		if (dy < 0)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dx / (float)dy;
		const float b = x1 - m * y1;
		for (int y = y1; y <= y2; y = y + 1)
		{
			int x = (int)(m * y + b + 0.5f);
			PutPixelAlpha(x, y, c);
		}
	}
	else
	{
		if (dx < 0)
		{
			int temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		const float m = (float)dy / (float)dx;
		const float b = y1 - m * x1;
		for (int x = x1; x <= x2; x = x + 1)
		{
			int y = (int)(m * x + b + 0.5f);
			PutPixelAlpha(x, y, c);
		}
	}
}

void Graphics::DrawRect(bool filled,int x1, int y1, int x2, int y2, Color c)
{
	int width = std::abs(x2 - x1);
	int height = std::abs(y2 - y1);

	if (filled)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				PutPixel(x1 + x, y1 + y, c);
			}
		}
	}
	else
	{
		for (int i = 0; i < width; i++)
		{
			PutPixel(x1 + i, y1, c);
			PutPixel(x1 + i, y2, c);
		}

		for (int i = 0; i < height; i++)
		{
			PutPixel(x1, y1 + i, c);
			PutPixel(x2, y1 + i, c);
		}
	}
}

void Graphics::DrawRectAlpha(bool filled, int x1, int y1, int x2, int y2, Color c)
{
	int width = std::abs(x2 - x1);
	int height = std::abs(y2 - y1);

	if (filled)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				PutPixelAlpha(x1 + x, y1 + y, c);
			}
		}
	}
	else
	{
		for (int i = 0; i < width; i++)
		{
			PutPixelAlpha(x1 + i, y1, c);
			PutPixelAlpha(x1 + i, y2, c);
		}

		for (int i = 0; i < height; i++)
		{
			PutPixelAlpha(x1, y1 + i, c);
			PutPixelAlpha(x2, y1 + i, c);
		}
	}
}
