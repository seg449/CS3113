#include "vector.h"

Vector::Vector() {
	x = 1.0f;
	y = 1.0f;
	z = 1.0f;
	x = 1.0f / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	y = 1.0f / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	z = 1.0f / sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length(){
	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

float Vector::normalize(){
	float temp = length();
	x = x / temp;
	y = y / temp;
	z = z / temp;
}