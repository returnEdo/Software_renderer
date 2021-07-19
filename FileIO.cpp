#include "FileIO.hpp"

#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "extern/stb_image.h"
#include "Manager.hpp"
#include "Macros.hpp"


extern ecs::Manager gManager;

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

	bool read(const std::string& tAddress, ecs::Entity tMeshId)
	{
		Mesh& lMesh = gManager.getComponent<Mesh>(tMeshId);

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

				lMesh.mPositions.push_back(lPosition);
			}
			else if (lHeader == C_NORMAL)
			{
				Math::vec3 lNormal;
				
				lStream >> lNormal.x;
				lStream >> lNormal.y;
				lStream >> lNormal.z;

				lMesh.mNormals.push_back(lNormal);
			}
			else if (lHeader == C_TEXTURE)
			{
				Math::vec2 lTexture;
				
				lStream >> lTexture.x;
				lStream >> lTexture.y;

				lMesh.mTextureUVs.push_back(lTexture);
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
				
				lMesh.mFaces.push_back(lFace);
			}
		}
		return true;
	}

}


namespace PNG
{

	bool read(const std::string& tAddress, ecs::Entity tTextureId)
	{
		Texture& lTexture = gManager.getComponent<Texture>(tTextureId);

		int lChannelCount;

		unsigned char* lData = stbi_load(tAddress.c_str(), &lTexture.mWidth, &lTexture.mHeight, &lChannelCount, 0);

		if (not lData)
		{
			return false;
		}

		for (int i = 0; i < lTexture.mHeight * lTexture.mWidth * lChannelCount; i += lChannelCount)
		{
			lTexture.mAlpha.push_back(lChannelCount == 4? static_cast<float>(lData[i + 3]): 1.0f);

			lTexture.mColors.push_back({static_cast<float>(lData[i]), 
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
		  unsigned int tWidth, unsigned int tHeight)
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


}


}

}
