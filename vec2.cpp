#include "vec2.hpp"

#include <assert.h>
#include <cmath>



namespace Math
{

vec2::vec2(float xx, float yy):  			x(xx),	y(yy)			{}
vec2::vec2(const vec2& other):				x(other.x), y(other.y)		{}		
vec2::vec2(const std::vector<float>& other): 		x(other[0]), y(other[1])	{}
vec2::vec2(float xyz):					x(xyz), y(xyz)			{}
vec2::vec2(void):					x(.0),	y(.0)			{}

void vec2::getRaw(float* data) const{

	data[0] = x;
	data[1] = y;
}

vec2 operator +(const vec2& a, const vec2& b){
	
	return vec2(a.x + b.x, a.y + b.y);
}

vec2 operator -(const vec2& a, const vec2& b){
	
	return vec2(a.x - b.x, a.y - b.y);
}


float operator *(const vec2& a, const vec2& b){
	
	return a.x * b.x + a.y * b.y;
}
	

vec2 operator *(const vec2& a, float c){
	
	return vec2(a.x * c, a.y * c);
} 

vec2 operator /(const vec2& a, float c){
	
	//assert(c != .0);
	
	return vec2(a.x / c, a.y / c);
} 


float operator ^(const vec2& a, const vec2& b){
	
	vec2 result;
	
	return (a.x * b.y - a.y * b.x);
}


bool operator ==(const vec2& a, const vec2& b){
	
	return (a.x == b.x and a.y == b.y);
}

vec2 operator %(const vec2& a, const vec2& b){

	return vec2(a.x * b.x, a.y * b.y);
}



float& vec2::operator [](const int indx){
	
	assert(indx == 0 or indx == 1);
	
	switch (indx){
		
		case 0:			return (this -> x);
		case 1:			return (this -> y);
	}
	return x;
}

void vec2::operator =(const vec2& a){

	x = a.x;
	y = a.y;
}


void vec2::operator +=(const vec2& b){
	
	*this = *this + b;
}

void vec2::operator -=(const vec2& b){
	
	*this = *this - b;
}

void vec2::operator *=(float c){
	
	*this = *this * c;
}

void vec2::operator /=(float c){
	
	*this = *this / c;
}

std::ostream& operator <<(std::ostream& os, const vec2& a){
	
	os << '(' << a.x << ',' << a.y << ')';
	
	return os;
}

float vec2::norm(void) const{
	
	return sqrt((* this) * (* this));
}

float norm2(const vec2& a){

	return (a * a);
}


float norm(vec2& a){
	
	return a.norm();
}

vec2 abs(const vec2& a){
	
	return vec2(a.x >= .0f? a.x: -a.x,
		      a.y >= .0f? a.y: -a.y);
}

void vec2::normalize(void){
	
	*this /= this -> norm();
}

vec2 normalize(vec2& a){
	
	vec2 result;
	
	result = a / a.norm();
	
	return result;
}


float distance(const vec2& a, const vec2& b){
	
	vec2 c = a - b;
	
	return norm(c);
}

bool nonNegative(const vec2& a){

	return (a.x >= 0.0 and  a.y >= 0.0);
}

};
