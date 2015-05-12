//
//  Matrix.h
//  HW6
//
//  Created by Hannah Mendez and Sebastien Emmanuel Gerard on 4/21/15.
//  Copyright (c) 2015 Hannah Mendez. All rights reserved.
//

#pragma once
#include <stdio.h>

#include "Vector.h"

class Matrix {
public:
    Matrix();
    union {
        float m[4][4];
        float ml[16];
    };
    void identity();
    Matrix inverse();
    Matrix operator*(const Matrix &m2);
    Vector operator*(const Vector &v2);
};