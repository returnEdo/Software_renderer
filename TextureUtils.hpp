#pragma once

#include <string>
#include <vector>

#include "vec2.hpp"
#include "vec3.hpp"
#include "Components.hpp"


namespace Renderer
{

template <typename T>
T sampleTexture(const Math::vec2& tTextureCoordinates, Buffer<T>& tBuffer)
{
	int xt = static_cast<int>(tBuffer.mWidth * tTextureCoordinates.x);
	int yt = static_cast<int>(tBuffer.mHeight * (1.0f - tTextureCoordinates.y));

	int lSize = tBuffer.mWidth * tBuffer.mHeight - 1;

	return tBuffer.mData[std::max(std::min(yt * tBuffer.mWidth + xt, lSize), 0)];
}


template <typename T>
void initBuffer(Buffer<T>& tBuffer, int tWidth, int tHeight, T tDefault)
{
	tBuffer.mWidth = tWidth;
	tBuffer.mHeight = tHeight;

	tBuffer.mData.erase(tBuffer.mData.begin(), tBuffer.mData.end());

	for (int i = 0; i < tWidth * tHeight; i++)
	{
		tBuffer.mData.push_back(tDefault);
	}
}

}
