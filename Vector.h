#pragma once

#include<string>

using namespace std;

class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	Vector(float x, float y, float z, float w);
	
	//set
	void setValue(float x, float y, float z);
	void setValue(float x, float y, float z, float w);
	void normalizeW();

	//get
	float length() const;
	string toString() const;

	//return the normalized vector (only on 3D axis)
	Vector normalized() const;

	//operators
	Vector operator+(Vector add) const;
	Vector operator-(Vector diff) const;
	Vector operator-() const;
	Vector operator*(Vector v2) const; //Cross product
	Vector operator*(float mult) const;
	Vector operator/(float div) const;
	void operator+=(Vector add);
	void operator-=(Vector diff);
	void operator*=(Vector v2);
	void operator*=(float mult);
	void operator/=(float div);
	bool operator==(Vector v2);
	bool operator!=(Vector v2);
	float &operator[](int i); //access by index
	const float &operator[](int i) const;

	float X, Y, Z, W;
};

