#include "FileIO.hpp"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "extern/stb_image.h"
#include "Macros.hpp"


namespace Renderer
{

namespace IO
{

namespace OBJ
{

	const std::string C_VERTEX 	= "v";
	const std::string C_NORMAL 	= "vn";
	const std::string C_TEXTURE	= "vt"; 
	const std::string C_FACE	= "f";

	bool read(const std::string& tAddress, Mesh& tMesh)
	{

		std::ifstream lFile(tAddress);

		if (not lFile.is_open())
		{
			return false;
		}

		std::string lLine;
		std::string lHeader;

		while (getline(lFile, lLine))
		{

			std::istringstream lStream(lLine);

			lStream >> lHeader;

			if (lHeader == C_VERTEX)
			{
				Math::vec3 lPosition;
				
				lStream >> lPosition.x;
				lStream >> lPosition.y;
				lStream >> lPosition.z;

				tMesh.mPositions.push_back(lPosition);
			}
			else if (lHeader == C_NORMAL)
			{
				Math::vec3 lNormal;
				
				lStream >> lNormal.x;
				lStream >> lNormal.y;
				lStream >> lNormal.z;

				tMesh.mNormals.push_back(lNormal);
			}
			else if (lHeader == C_TEXTURE)
			{
				Math::vec2 tTexture;
				
				lStream >> tTexture.x;
				lStream >> tTexture.y;

				tMesh.mTextureUVs.push_back(tTexture);
			}
			else if (lHeader == C_FACE)
			{
				std::string lTriple;

				Face lFace;

				for (int i = 0; i < 3; i++)
				{
					lStream >> lTriple;

					sscanf(lTriple.c_str(), "%u/%u/%u", &lFace.mIndices[i].mPosition,
									    &lFace.mIndices[i].mTextureUV,
									    &lFace.mIndices[i].mNormal);
					lFace.mIndices[i].mPosition--;
					lFace.mIndices[i].mTextureUV--;
					lFace.mIndices[i].mNormal--;
				}
				
				tMesh.mFaces.push_back(lFace);
			}
		}
		return true;
	}

}


namespace PNG
{

	bool read(const std::string& tAddress, Texture& tTexture)
	{
		int lChannelCount;

		unsigned char* lData = stbi_load(tAddress.c_str(), &tTexture.mWidth, &tTexture.mHeight, &lChannelCount, 0);

		if (not lData)
		{
			return false;
		}

		for (int i = 0; i < tTexture.mHeight * tTexture.mWidth * lChannelCount; i += lChannelCount)
		{
			tTexture.mAlpha.push_back(lChannelCount == 4? static_cast<float>(lData[i + 3]): 1.0f);

			tTexture.mColors.push_back({static_cast<float>(lData[i]), 
						    static_cast<float>(lData[i + 1]), 
						    static_cast<float>(lData[i + 2])});
		}

		stbi_image_free(lData);

		return true;
	};

}

namespace PPM
{

	const std::string C_P_PPM 		= "P6";
	const unsigned int C_COLOR_COUNT 	= 255;


	bool write(const std::string& tAddress, const std::vector<Math::vec3>& tColors,
		  int tWidth, int tHeight)
	{
		std::ofstream lFile(tAddress);
		
		if (not lFile.is_open())
		{
			return false;
		}

		lFile << C_P_PPM 	<< std::endl;
		lFile << tWidth 	<< " " << tHeight << std::endl;
		lFile << C_COLOR_COUNT 	<< std::endl;

		for (auto lColor: tColors)
		{
			lFile << static_cast<unsigned char>(lColor.x) << 
				 static_cast<unsigned char>(lColor.y) <<
				 static_cast<unsigned char>(lColor.z);
		}
		lFile.close();

		return true;
	};

	bool write(const std::string& tAddress, const Texture& tTexture)
	{
		return write(tAddress, tTexture.mColors, tTexture.mWidth, tTexture.mHeight);		
	};

}


}

}
