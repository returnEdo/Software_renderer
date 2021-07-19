#pragma once

#include <iostream>
#include "vec3.hpp"


namespace Math
{

class mat3{
	
	public:

	vec3 col1;
	vec3 col2;
	vec3 col3;
	
	vec3 row1;
	vec3 row2;
	vec3 row3;
	
	mat3(const vec3& r1, const vec3& r2, const vec3& r3);
	mat3(const mat3& other);
	mat3(const vec3& axis, float theta, bool tNormalize = false);
	mat3(const vec3& diag);
	mat3(void);

	void buildCol(void);
	void getColumnMajorOrder(float* data) const;

	friend mat3 operator *(const mat3& a, float b);
	friend mat3 operator *(const mat3& a, const mat3& b);
	friend vec3 operator *(const mat3& a, const vec3& b);
	void operator *=(float b);
	void operator *=(const mat3& b);
	
	friend mat3 operator /(const mat3& a, float b);
	void operator /=(float b);
	
	friend mat3 operator +(const mat3& a, const mat3& b);
	void operator +=(const mat3& b);
	
	friend mat3 operator -(const mat3& a, const mat3& b);
	void operator -=(const mat3& b);
	
	
	friend float trace(const mat3& tA);
	friend float det(const mat3& a);
	friend bool isSingular(const mat3& a)	{ return (det(a) == .0); }
	
	friend mat3 transpose(const mat3& a);
	friend mat3 inv(const mat3& a);
	friend mat3 diagonalInv(const mat3& mat);

	friend std::ostream& operator <<(std::ostream& os, const mat3& a);
	
};

};
