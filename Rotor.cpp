#include "Rotor.hpp"

#include <cmath>

#include "MathUtils.hpp"


namespace Math
{

Rotor::Rotor(const Math::mat3& tR)
{
	if (tR.row3.z <= 0.0f)			// c**2+d**2 > a**2+b**2
	{
		if (tR.row1.x >= tR.row2.y)	// c**2 >= d**2
		{
			c = 0.5f * sqrt(1.0f + tR.row1.x - tR.row2.y - tR.row3.z);
			a = (tR.row3.y - tR.row2.z) / (4.0f * c);
			b = (tR.row3.x + tR.row1.z) / (4.0f * c);
			d = (tR.row1.y + tR.row2.x) / (4.0f * c);
		}
		else
		{
			d = 0.5f * sqrt(1.0f + tR.row2.y - tR.row1.x - tR.row3.z);
			a = (tR.row1.z - tR.row3.x) / (4.0f * d);
			b = (tR.row2.z + tR.row3.y) / (4.0f * d);
			c = (tR.row1.y + tR.row2.x) / (4.0f * d);
		}
	}
	else
	{
		if (tR.row1.x >= -tR.row2.y)	// a**2 >= b**2
		{
			a = 0.5f * sqrt(1.0f + tR.row1.x + tR.row2.y + tR.row3.z);
			b = (tR.row2.x - tR.row1.y) / (4.0f * a);
			c = (tR.row3.y - tR.row2.z) / (4.0f * a);
			d = (tR.row1.z - tR.row3.x) / (4.0f * a);
		}
		else
		{
			b = 0.5f * sqrt(1.0f + tR.row3.z - tR.row1.x - tR.row2.y);
			a = (tR.row2.x - tR.row1.y) / (4.0f * b);
			c = (tR.row3.x + tR.row1.z) / (4.0f * b);
			d = (tR.row3.y + tR.row2.z) / (4.0f * b);
		}
	}
}



Math::mat3 Rotor::getMatrixForm(void)
{
	Math::mat3 lMat;

	lMat.row1 = Math::vec3(a*a - b*b + c*c - d*d,
	          	       2.0f * (-a*b + c*d),
	         	       2.0f * (a*d + b*c));
	lMat.row2 = Math::vec3(2.0f * (a*b + c*d),
	         	       a*a - b*b - c*c + d*d,
	         	       2.0f * (-a*c + b*d));
	lMat.row3 = Math::vec3(2.0f * (-a*d + b*c),
			       2.0f * (a*c + b*d),
			       a*a + b*b - c*c - d*d);

	lMat.buildCol();

	return lMat;
}

void Rotor::normalize(void)
{
	float lNorm = getNorm();
	
	a /= lNorm;
	b /= lNorm;
	c /= lNorm;
	d /= lNorm;
}


std::ostream& operator <<(std::ostream& tOs, Rotor& tRotor)
{
	tOs << "(" << tRotor.a << ", " << tRotor.b << ", "
	    << tRotor.c << ", "  << tRotor.d << ")";
	
	return tOs;

}


};
