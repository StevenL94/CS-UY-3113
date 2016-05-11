//
//  Bullet.hpp
//  Homework 3
//
//  Created by Steven Lee on 3/4/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifndef Bullet_hpp
#define Bullet_hpp

#include <stdio.h>
#include "Entity.hpp"

class Bullet : public Entity {
public:
    float angle, speed, timeAlive;
    float vertices[12] = {-0.005, -0.005, 0.005, -0.005, 0.005, 0.005, -0.005, -0.005, 0.005, 0.005, -0.005, 0.005};
    float texCoords[12] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    Bullet(float x1);
    bool displayBullet(float time);
    Bullet();
    void update(float& lastFrameTicks, float& elapsed, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram& program);
};

#endif /* Bullet_hpp */
