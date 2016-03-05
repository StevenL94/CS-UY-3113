//
//  Bullet.cpp
//  Homework 3
//
//  Created by Steven Lee on 3/4/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#include "Bullet.hpp"

Bullet::Bullet(float x1) {
    setVertices(vertices, 12);
    setTexCoords(texCoords, 12);
    x = x1;
    y = -0.85;
}

bool Bullet::displayBullet(float time) {
    if (time >= timeAlive) {
        return false;
    }
    else {
        return true;
    }
}

void Bullet::update(float& lastFrameTicks, float& elapsed, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram& program) {
    //    Update modelMatrix
    Matrix modelMatrix;
    if (y < 1) {
        y += elapsed;
        timeAlive += elapsed;
    }
    modelMatrix.identity();
    modelMatrix.Translate(x, y, 0);
    program.setModelMatrix(modelMatrix);
}