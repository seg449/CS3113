#include <math.h>

class Vector{
public:
	Vector();
	Vector(float x, float y, float z);
	float length();
	float normalize();
	float x, y, z;
};