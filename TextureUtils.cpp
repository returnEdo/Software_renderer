#include "TextureUtils.hpp"


#include "Macros.hpp"



namespace Renderer
{

TextureSample sampleTexture(const Math::vec2& tTextureCoordinates, Texture& tTexture)
{
	// Transformation to texture coordinates
	int xT = static_cast<int>(tTexture.mWidth * tTextureCoordinates.x);
	int yT = static_cast<int>(-tTexture.mHeight * tTextureCoordinates.y + tTexture.mHeight);
	
	int lSize = tTexture.mWidth * tTexture.mHeight - 1;

	return {tTexture.mAlpha[std::max(std::min(yT * tTexture.mWidth + xT, lSize), 0)],
		tTexture.mColors[std::max(std::min(yT * tTexture.mWidth + xT, lSize), 0)]};
}




}
