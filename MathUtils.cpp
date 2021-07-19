#include "MathUtils.hpp"

#include <cmath>
#include <cstdlib>



namespace Math
{
	
float sign(float tValue)
{	
	return (tValue >= .0f? 1.0f: -1.0f);
}

float segmentOverlap(float tCenterA, float tDimensionA,
		     float tCenterB, float tDimensionB)
{
	return (fmin(tCenterA + tDimensionA / 2.0f, tCenterB + tDimensionB / 2.0f) -
		fmax(tCenterA - tDimensionA / 2.0f, tCenterB - tDimensionB / 2.0f));
}


bool areRectanglesIntersecting(const Math::vec2& tCenterA, const Math::vec2& tDimensionsA,
			       const Math::vec2& tCenterB, const Math::vec2& tDimensionsB)
{
	return ((segmentOverlap(tCenterA.x, tDimensionsA.x, tCenterB.x, tDimensionsB.x) > 0) and
		(segmentOverlap(tCenterA.y, tDimensionsA.y, tCenterB.y, tDimensionsB.y) > 0));
}


bool near(float tValue, float tReference, float tTollerance)
{
	return (std::abs(tValue - tReference) <= tTollerance);
}


Math::mat3 cross(const Math::vec3& tV)
{
	return Math::mat3(Math::vec3(0.0f, -tV.z, tV.y),
			  Math::vec3(tV.z, 0.0f, -tV.x),
			  Math::vec3(-tV.y, tV.x, 0.0f));
}


float randf(float tA, float tB)
{	
	return (tA + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (tB - tA));
}


float clamp(float tX, float tLower, float tUpper)
{
	return fmax(tLower, fmin(tUpper, tX));
}

Math::vec3 vLerp(const Math::vec3& tA, const Math::vec3& tB, float tAlpha)
{
	return (1.0f - tAlpha) * tA + tAlpha * tB;
}

Math::vec3 findBarycentric(const Math::vec3& x, const Math::vec3& u,
			   const Math::vec3& v, const Math::vec3& w)
{
	Math::vec2 q {x.x - w.x, x.y - w.y};
	Math::vec2 m {u.x - w.x, u.y - w.y};
	Math::vec2 n {v.x - w.x, v.y - w.y};

	float qm = q * m;
	float qn = q * n;
	float mn = m * n;
	float mm = norm2(m);
	float nn = norm2(n);

	float det = mm * nn - mn * mn;

	// TODO: take care singular case
	
	float lAlpha 	= (nn * qm - mn * qn) / det;
	float lBeta 	= (-mn * qm + mm * qn) / det;
	
	return {lAlpha, lBeta, 1.0f - lAlpha - lBeta};
}

bool isInsideBarycentric(const Math::vec3& tB)
{
	return (tB.x >= .0f and tB.y >= .0f and tB.z >= .0f);
}

};
