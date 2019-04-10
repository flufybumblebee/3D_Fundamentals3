#pragma once

#include <limits>
#include <cassert>
#include <algorithm>

class ZBuffer
{
public:
	ZBuffer( int width,int height )
		:
		width( width ),
		height( height ),
		pBuffer( new float[static_cast<size_t>(width)* static_cast<size_t>(height)] )
	{}
	~ZBuffer()
	{
		delete[] pBuffer;
		pBuffer = nullptr;
	}
	ZBuffer( const ZBuffer& ) = delete;
	ZBuffer& operator=( const ZBuffer& ) = delete;
	void Clear()
	{
		const int nDepths = width * height;
		for( int i = 0; i < nDepths; i++ )
		{
			pBuffer[i] = std::numeric_limits<float>::infinity();
		}
	}
	float& At( int x,int y )
	{
		assert( x >= 0 );
		assert( x < width );
		assert( y >= 0 );
		assert( y < height );
		return pBuffer[y * width + x];
	}
	const float& At( int x,int y ) const
	{
		return const_cast<ZBuffer*>(this)->At( x,y );
	}
	bool TestAndSet( int x,int y,float depth )
	{
		float& depthInBuffer = At( x,y );
		if( depth < depthInBuffer )
		{
			depthInBuffer = depth;
			return true;
		}
		return false;
	}
	int GetWidth() const
	{
		return width;
	}
	int GetHeight() const
	{
		return height;
	}
	auto GetMinMax() const
	{
		size_t size = static_cast<size_t>(width) * static_cast<size_t>(height);
		return std::minmax_element( pBuffer,pBuffer + size );
	}
private:
	int width;
	int height;
	float* pBuffer = nullptr;
};