#include "Matrix.h"
#include <stdexcept>

#define PI 3.14159265

Matrix::Matrix(void)
{
	for(int i = 0; i < 4; i++)
		values.push_back(Vector());
}
Matrix::Matrix(float values[16])
{
	setValue(values);
}
Matrix::~Matrix(void) { }


//static metods
Matrix Matrix::Identity()
{
	Matrix identity;

	for(int i = 0; i < 4; i++)
		identity[i][i] = 1;

	return identity;
}


//set
void Matrix::setValue(Matrix m)
{
	values = m.values;
}
void Matrix::setValue(float values[16])
{
	for(int i = 0; i < 16; i+=4)
	{
		Vector v;
		for(int j = 0; j < 4; j++)
			v[j] = values[i+j];
		this->values.push_back(v);
	}
}
void Matrix::setValue(int row, int column, float value)
{
	if(row < 0 || row >= 4 || column < 0 || column >= 4)
		throw std::invalid_argument("invalid matrix indices");

	values[row][column] = value;
}


//get
float Matrix::getValue(int row, int column) const
{
	return values[row][column];
}


//operators
Matrix Matrix::operator+(Matrix m) const
{
	Matrix sum;

	for(int i = 0; i < 4; i++)
		sum[i] = values[i]+m[i];
	
	return sum;
}
Matrix Matrix::operator-(Matrix m) const
{
	Matrix diff;
	
	for(int i = 0; i < 4; i++)
		diff[i] = values[i]-m[i];

	return diff;
}
Matrix Matrix::operator*(Matrix m) const
{
	Matrix mult;

	for(int row = 0; row < 4; row++)
		for(int column = 0; column < 4; column++)
			for(int iter = 0; iter < 4; iter++)
				mult[row][column] += values[row][iter] * m[iter][column];

	return mult;
}
Vector Matrix::operator*(Vector v) const
{
	Vector mult;

	for(int row = 0; row < 4; row++)
		for(int iter = 0; iter < 4; iter++)
			mult[row] += values[row][iter] * v[iter];

	return mult;
}
Vector &Matrix::operator[](int row)
{
	return values[row];
}
const Vector &Matrix::operator[](int row) const
{
	return values[row];
}

//static functions
Matrix Matrix::createTranslation(Vector translation)
{
	Matrix t = Matrix::Identity();

	for(int i = 0; i < 3; i++)
		t[i][3] = translation[i];

	return t;
}
Matrix Matrix::createRotationX(float angle)
{
	Matrix rx = Matrix::Identity();
	float aRad = angle * PI / 180;

	rx[1][1] = cos(aRad);
	rx[1][2] = -sin(aRad);
	rx[2][1] = sin(aRad);
	rx[2][2] = cos(aRad);

	return rx;
}
Matrix Matrix::createRotationY(float angle)
{
	Matrix ry = Matrix::Identity();
	float aRad = angle * PI / 180;

	ry[0][0] = cos(aRad);
	ry[0][2] = sin(aRad);
	ry[2][0] = -sin(aRad);
	ry[2][2] = cos(aRad);

	return ry;
}
Matrix Matrix::createRotationZ(float angle)
{
	Matrix rz = Matrix::Identity();
	float aRad = angle * PI / 180;

	rz[0][0] = cos(aRad);
	rz[0][1] = -sin(aRad);
	rz[1][0] = sin(aRad);
	rz[1][1] = cos(aRad);

	return rz;
}
Matrix Matrix::createRotation(float angle, Vector axis)
{
	float rotationFromXYPlane = -(atan2(axis[2], axis[0])) * 180 / PI;
	float rotationFromYAxis = -(atan2(sqrt(pow(axis[0], 2) + pow(axis[2], 2)), axis[1])) * 180 / PI;

	return createRotationY(rotationFromXYPlane) //restore rotation axis in XYZ space
			* (createRotationZ(rotationFromYAxis) //restore rotation axis on XY plane 
			* (createRotationY(angle) //rotate
			* (createRotationZ(-rotationFromYAxis) //align rotation axis to Y axis
			* createRotationY(-rotationFromXYPlane)))); //align rotation axis on XY plane;
}
Matrix Matrix::createScale(Vector scale)
{
	Matrix s = Matrix::Identity();

	for(int i = 0; i < 3; i++)
		s[i][i] = scale[i];

	return Matrix(s);
}
Matrix Matrix::createView(Vector position, Vector direction, Vector up)
{
	float angle;
	Matrix tempMatrix;

	tempMatrix = createTranslation(-position);
	Matrix view = tempMatrix; //now position is (0, 0, 0)

	Vector rotationAxis = direction * Vector(0, 1, 0);
	angle = -atan2(direction.Y, sqrt(pow(direction.X, 2) + pow(direction.Z, 2))) * 180 / PI;
	tempMatrix = createRotation(angle, rotationAxis);
	direction = tempMatrix * direction;
	up = tempMatrix * up;
	view = tempMatrix * view; //now it's posed on XZ plane

	angle = -atan2(-direction.X, -direction.Z) * 180 / PI;
	tempMatrix = createRotationY(angle);
	direction = tempMatrix * direction;
	up = tempMatrix * up;
	view = tempMatrix * view; //now direction is (0, 0, -1)

	angle = -atan2(-up.X, up.Y) * 180 / PI;
	tempMatrix = createRotationZ(angle);
	direction = tempMatrix * direction;
	up = tempMatrix * up;
	view = tempMatrix * view; //now up is (0, 1, 0)

	return view;
}
Matrix Matrix::createProjection(float l, float b, float n, float r, float t, float f)
{
	float values[16] = {2*n/(r-l), 0        , (r+l)/(r-l) , 0           ,
						0        , 2*n/(t-b), (t+b)/(t-b) , 0           ,
						0        , 0        , -(f+n)/(f-n), -(2*f*n)/(f-n),
						0        , 0        , -1          , 0           };
	
	return Matrix(values);
}
