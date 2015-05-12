//
//  Vector.cpp
//  HW6
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 4/21/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#include "Vector.h"

Vector::Vector() {}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length() {
    
    return sqrtf(x*x + y*y);
}

void Vector::normalize() {
    
    if (length()) {
        x /= length();
        y /= length();
    }
}