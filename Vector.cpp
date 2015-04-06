#include "Vector.h"
#include<math.h>
#include <stdexcept>
#include<sstream>

using namespace std;

Vector::Vector()
{
	X = Y = Z = W = 0;
}
Vector::Vector(float X, float Y, float Z)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
	this->W = 1;
}
Vector::Vector(float X, float Y, float Z, float W)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
	this->W = W;
}

//set
void Vector::setValue(float X, float Y, float Z)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
}
void Vector::setValue(float X, float Y, float Z, float W)
{
	this->X = X;
	this->Y = Y;
	this->Z = Z;
	this->W = W;
}
void Vector::normalizeW()
{
	if(W != 0)
	{
		X /= W;
		Y /= W;
		Z /= W;
		W = 1;
	}
}

//get
float Vector::length() const { return sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2)); }
string Vector::toString() const
{
	stringstream sstr;
	sstr << "{" << X << ", " << Y << ", " << Z << "}";
	return sstr.str();
}


//normalized
Vector Vector::normalized() const
{
	Vector normalized;

	float vectorLen = length();
	if(vectorLen != 0)
	{
		normalized.X = X/vectorLen;
		normalized.Y = Y/vectorLen;
		normalized.Z = Z/vectorLen;
	}

	return normalized;
}

//operators
Vector Vector::operator+(Vector add) const
{
	return Vector(this->X + add.X, this->Y + add.Y, this->Z + add.Z, this->W + add.W);
}
Vector Vector::operator-(Vector diff) const
{
	return Vector(this->X - diff.X, this->Y - diff.Y, this->Z - diff.Z, this->W - diff.W);
}
Vector Vector::operator-() const
{
	return Vector() - *this;
}
Vector Vector::operator*(Vector v2) const
{
	return Vector(Y*v2.Z-Z*v2.Y, Z*v2.X-X*v2.Z, X*v2.Y-Y*v2.X);
}
Vector Vector::operator*(float mult) const
{
	return Vector(this->X * mult, this->Y * mult, this->Z * mult, this->W * mult);
}
Vector Vector::operator/(float div) const
{
	return Vector(this->X / div, this->Y / div, this->Z / div, this->W / div);
}
float &Vector::operator[](int i)
{
	switch(i)
	{
	case 0: return X;
	case 1: return Y;
	case 2: return Z;
	case 3: return W;
	default: throw std::invalid_argument("invalid matrix indices");
	}
}
const float &Vector::operator[](int i) const
{
	switch(i)
	{
	case 0: return X;
	case 1: return Y;
	case 2: return Z;
	case 3: return W;
	default: throw std::invalid_argument("invalid matriX indices");
	}
}
void Vector::operator+=(Vector add)
{
	Vector v = (*this)+add;
	for(int i = 0; i < 4; i++)
		(*this)[i] = v[i];
}
void Vector::operator-=(Vector diff)
{
	Vector v = (*this)-diff;
	for(int i = 0; i < 4; i++)
		(*this)[i] = v[i];
}
void Vector::operator*=(Vector v2)
{
	Vector v = (*this)*v2;
	for(int i = 0; i < 4; i++)
		(*this)[i] = v[i];
}
void Vector::operator*=(float mult)
{
	for(int i = 0; i < 4; i++)
		(*this)[i] *= mult;
}
void Vector::operator/=(float div)
{
	for(int i = 0; i < 4; i++)
		(*this)[i] /= div;
}
bool Vector::operator==(Vector v2)
{
	for(int i = 0; i < 4; i++)
		if((*this)[i] != v2[i])
			return false;
	return true;
}
bool Vector::operator!=(Vector v2)
{
	return !((*this) == v2);
}