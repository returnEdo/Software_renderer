#pragma once

#include <iostream>
#include <vector>


namespace Math
{

class vec3{
	
	public:
	
	float x;
	float y;
	float z;
	
	vec3(float xx, float yy, float zz);
	vec3(const vec3& other);
	vec3(const std::vector<float>& other);
	vec3(float xyz);
	vec3(void);

	void getRaw(float* data) const;
	
	friend vec3 operator +(const vec3& a, const vec3& b);
	friend vec3 operator -(const vec3& a, const vec3& b);
	friend float operator *(const vec3& a, const vec3& b);	
	
	friend vec3 operator *(const vec3& a, float c);
	inline friend vec3 operator *(float c, const vec3& a) { return (a * c); }
	
	friend vec3 operator /(const vec3& a, float c);
	
	friend vec3 operator ^(const vec3& a, const vec3& b);
	friend bool operator ==(const vec3& a, const vec3& b);	
	friend vec3 operator %(const vec3& a, const vec3& b);
	float& operator [](const int indx);			
	
	void operator =(const vec3& a);

	void operator +=(const vec3 &a);
	void operator -=(const vec3 &b);

	void operator *=(float c);

	void operator /=(float c);

	friend std::ostream& operator <<(std::ostream& os, const vec3& a);
	
	float norm(void) const;
	friend float norm(vec3& a);
	friend float norm2(const vec3& a);

	friend vec3 abs(const vec3& a);
	
	void normalize(void);
	friend vec3 normalize(vec3& a);

	friend float distance(const vec3& a, const vec3& b);

	friend bool nonNegative(const vec3& a);
};

};

