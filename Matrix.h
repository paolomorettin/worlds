#pragma once
#include"Vector.h"
#include<vector>
#include <math.h>

//it's a 4x4 matrix
class Matrix
{
public:
	Matrix(void);
	Matrix(float values[16]); //values has to be an array with 16 elements!
	~Matrix(void);

	//static metods
	static Matrix Identity();

	//set
	void setValue(Matrix m);
	void setValue(float values[16]); //values has to be an array with 16 elements!
	void setValue(int row, int column, float value);

	//get
	float getValue(int row, int column) const; //row and column values accepted from 0 to 3

	//operators
	Matrix operator+(Matrix m) const;
	Matrix operator-(Matrix m) const;
	Matrix operator*(Matrix m) const;
	Vector operator*(Vector v) const;
	Vector &operator[](int row); //access row by index
	const Vector &operator[](int row) const;

	//static
	static Matrix createTranslation(Vector translation);
	static Matrix createRotationX(float angle);
	static Matrix createRotationY(float angle);
	static Matrix createRotationZ(float angle);
	static Matrix createRotation(float angle, Vector axis);
	static Matrix createScale(Vector scale);
	static Matrix createView(Vector position, Vector direction, Vector up);
	static Matrix createProjection(float left, float bottom, float near, float right, float top, float far);

private:
	std::vector<Vector> values;
};

