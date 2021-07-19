#pragma once

#include <iostream>
#include <vector>

namespace Math
{

class vec2{
	
	public:
	
	float x;
	float y;
	
	vec2(float xx, float yy);
	vec2(const vec2& other);
	vec2(const std::vector<float>& other);
	vec2(float xyz);
	vec2(void);

	void getRaw(float* data) const;
	
	friend vec2 operator +(const vec2& a, const vec2& b);
	friend vec2 operator -(const vec2& a, const vec2& b);
	friend float operator *(const vec2& a, const vec2& b);	
	
	friend vec2 operator *(const vec2& a, float c);
	inline friend vec2 operator *(float c, const vec2& a) { return (a * c); }
	
	friend vec2 operator /(const vec2& a, float c);
	
	friend float operator ^(const vec2& a, const vec2& b);
	friend bool operator ==(const vec2& a, const vec2& b);	
	friend vec2 operator %(const vec2& a, const vec2& b);
	float& operator [](const int indx);			
	
	void operator =(const vec2& a);

	void operator +=(const vec2 &a);
	void operator -=(const vec2 &b);

	void operator *=(float c);

	void operator /=(float c);

	friend std::ostream& operator <<(std::ostream& os, const vec2& a);
	
	float norm(void) const;
	friend float norm(vec2& a);
	friend float norm2(const vec2& a);

	friend vec2 abs(const vec2& a);
	
	void normalize(void);
	friend vec2 normalize(vec2& a);

	friend float distance(const vec2& a, const vec2& b);

	friend bool nonNegative(const vec2& a);
};

};
