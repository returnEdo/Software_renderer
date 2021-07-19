#include "mat3.hpp"

#include <assert.h>
#include <math.h>

namespace Math
{

mat3::mat3(const vec3& r1, const vec3& r2, const vec3& r3):
		row1(r1), row2(r2), row3(r3)			{this -> buildCol();}	

mat3::mat3(const mat3& other):
	row1(other.row1), row2(other.row2), row3(other.row3)	{this -> buildCol();}

mat3::mat3(void):				
	row1(vec3(1.0, .0, .0)), row2(vec3(.0, 1.0, .0)), row3(vec3(.0, .0, 1.0))
	{this -> buildCol();}

mat3::mat3(const vec3& diag):
	row1(vec3(diag.x, .0, .0)), row2(vec3(.0, diag.y, .0)), row3(vec3(.0, .0, diag.z))
	{this -> buildCol();}

mat3::mat3(const vec3& nn, float t, bool tNormalize){
	
	vec3 n { nn };
	if (tNormalize)
	{
		n.normalize();
	}

	row1 = vec3(cos(t) + (1 - cos(t)) * n.x * n.x, -sin(t) * n.z + (1 - cos(t)) * n.x * n.z, sin(t) * n.y + (1 - cos(t)) * n.x * n.z);
	row2 = vec3(sin(t) * n.z + (1 - cos(t)) * n.x * n.z, cos(t) + (1 - cos(t)) * n.y * n.y, -sin(t) * n.x + (1 - cos(t)) * n.y * n.z);
	row3 = vec3(-sin(t) * n.y + (1 - cos(t)) * n.x * n.z, sin(t) * n.x + (1 - cos(t)) * n.y * n.z, cos(t) + (1 - cos(t)) * n.z * n.z);

	this -> buildCol();
}

void mat3::buildCol(void){
	
	this -> col1 = vec3(row1.x, row2.x , row3.x);
	this -> col2 = vec3(row1.y, row2.y , row3.y);
	this -> col3 = vec3(row1.z, row2.z , row3.z);
}


void mat3::getColumnMajorOrder(float* data) const{

	col1.getRaw(data);
	col2.getRaw(data + 3);
	col3.getRaw(data + 6);
}


mat3 operator *(const mat3& a, float b){
	
	return mat3(a.row1 * b, a.row2 * b, a.row3 * b);
}


mat3 operator *(const mat3& a, const mat3& b){

	vec3 res1 = vec3(a.row1 * b.col1, a.row1 * b.col2, a.row1 * b.col3);
	vec3 res2 = vec3(a.row2 * b.col1, a.row2 * b.col2, a.row2 * b.col3);
	vec3 res3 = vec3(a.row3 * b.col1, a.row3 * b.col2, a.row3 * b.col3);

	return mat3(res1, res2, res3);
}


vec3 operator *(const mat3& a, const vec3& b){
	
	return vec3(a.row1 * b, a.row2 * b, a.row3 * b);
}


void mat3::operator *=(float b){
	
	*this = *this * b;
}


void mat3::operator *=(const mat3& b){
	
	*this = *this * b;
}


mat3 operator /(const mat3& a, float b){
	
	assert(b != .0);
	
	return mat3(a.row1 / b, a.row2 / b, a.row3 / b);
}


void mat3::operator /=(float b){
	
	*this = *this / b;
}


mat3 operator +(const mat3& a, const mat3& b){
	
	return mat3(a.row1 + b.row1, a.row2 + b.row2, a.row3 + b.row3);
}

void mat3::operator +=(const mat3& b){
	
	*this = *this + b;
}

mat3 operator -(const mat3& a, const mat3& b){
	
	return mat3(a.row1 - b.row1, a.row2 - b.row2, a.row3 - b.row3);
}

void mat3::operator -=(const mat3& b){
	
	*this = *this - b;
}


float trace(const mat3& tA)
{
	
	return (tA.row1.x + tA.row2.y + tA.row3.z);
}

float det(const mat3& a){
	
	return a.row1.x * a.row2.y * a.row3.z + 
		   a.row1.y * a.row2.z * a.row3.x + 
		   a.row1.z * a.row2.x * a.row3.y - 
		   a.row1.z * a.row2.y * a.row3.x - 
		   a.row1.x * a.row2.z * a.row3.y - 
		   a.row1.y * a.row2.x * a.row3.z;
} 


mat3 transpose(const mat3& a){
	
	return mat3(a.col1, a.col2, a.col3);
}


mat3 inv(const mat3& a){
	
	float dt = det(a);
	
	assert(dt != .0);
	
	vec3 r1 = vec3(a.row2.y * a.row3.z - a.row2.z * a.row3.y,
				   a.row2.z * a.row3.x - a.row2.x * a.row3.z,
				   a.row2.x * a.row3.y - a.row2.y * a.row3.x);
	
	vec3 r2 = vec3(a.row3.y * a.row1.z - a.row3.z * a.row1.y,
				   a.row3.z * a.row1.x - a.row3.x * a.row1.z,
				   a.row3.x * a.row1.y - a.row3.y * a.row1.x);
	
	vec3 r3 = vec3(a.row1.y * a.row2.z - a.row1.z * a.row2.y,
				   a.row1.z * a.row2.x - a.row1.x * a.row2.z,
				   a.row1.x * a.row2.y - a.row1.y * a.row2.x);
    
    return transpose(mat3(r1, r2, r3)) / dt;
}

mat3 diagonalInv(const mat3& mat){

	return mat3(vec3(1.0f / mat.row1.x, 1.0f / mat.row2.y, 1.0f / mat.row3.z));
}

    
std::ostream& operator <<(std::ostream& os, const mat3& a){
	
	os << a.row1.x << ' ' << a.row1.y << ' ' << a.row1.z << '\n' <<
		  a.row2.x << ' ' << a.row2.y << ' ' << a.row2.z << '\n' <<
		  a.row3.x << ' ' << a.row3.y << ' ' << a.row3.z << '\n';
	
	return os;
}
		

};
