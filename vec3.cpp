#include "vec3.hpp"

#include <assert.h>
#include <cmath>



namespace Math
{

vec3::vec3(float xx, float yy, float zz):  		x(xx),	y(yy),	z(zz)		{}
vec3::vec3(const vec3& other):			x(other.x), y(other.y), z(other.z)	{}		
vec3::vec3(const std::vector<float>& other): 	x(other[0]), y(other[1]), z(other[2])	{}
vec3::vec3(float xyz):				x(xyz), y(xyz), z(xyz)	{}
vec3::vec3(void):					x(.0),	y(.0),	z(.0)	{}

void vec3::getRaw(float* data) const{

	data[0] = x;
	data[1] = y;
	data[2] = z;
}

vec3 operator +(const vec3& a, const vec3& b){
	
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator -(const vec3& a, const vec3& b){
	
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}


float operator *(const vec3& a, const vec3& b){
	
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
	

vec3 operator *(const vec3& a, float c){
	
	return vec3(a.x * c, a.y * c, a.z * c);
} 

vec3 operator /(const vec3& a, float c){
	
	//assert(c != .0);
	
	return vec3(a.x / c, a.y / c, a.z / c);
} 


vec3 operator ^(const vec3& a, const vec3& b){
	
	vec3 result;
	
	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	
	return result;
}


bool operator ==(const vec3& a, const vec3& b){
	
	return (a.x == b.x and a.y == b.y and a.z == b.z);
}

vec3 operator %(const vec3& a, const vec3& b){

	return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}



float& vec3::operator [](const int indx){
	
	assert(indx >= 0 and indx <= 2);
	
	switch (indx){
		
		case 0:			return (this -> x);
		case 1:			return (this -> y);
		case 2:			return (this -> z);
	}
	return x;
}

void vec3::operator =(const vec3& a){

	x = a.x;
	y = a.y;
	z = a.z;
}


void vec3::operator +=(const vec3 &b){
	
	*this = *this + b;
}

void vec3::operator -=(const vec3 &b){
	
	*this = *this - b;
}

void vec3::operator *=(float c){
	
	*this = *this * c;
}

void vec3::operator /=(float c){
	
	*this = *this / c;
}

std::ostream& operator <<(std::ostream& os, const vec3& a){
	
	os << '(' << a.x << ',' << a.y << ',' << a.z << ')';
	
	return os;
}

float vec3::norm(void) const{
	
	return sqrt((* this) * (* this));
}

float norm2(const vec3& a){

	return (a * a);
}


float norm(vec3& a){
	
	return a.norm();
}

vec3 abs(const vec3& a){
	
	return vec3(a.x >= .0f? a.x: -a.x,
		      a.y >= .0f? a.y: -a.y,
		      a.z >= .0f? a.z: -a.z);
}

void vec3::normalize(void){
	
	*this /= this -> norm();
}

vec3 normalize(vec3& a){
	
	vec3 result;
	
	result = a / a.norm();
	
	return result;
}


float distance(const vec3& a, const vec3& b){
	
	vec3 c = a - b;
	
	return norm(c);
}

bool nonNegative(const vec3& a){

	return (a.x >= 0.0 and  a.y >= 0.0 and a.z >= 0.0);
}

};
