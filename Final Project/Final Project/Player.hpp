//
//  Player.hpp
//  Final Project
//
//  Created by Steven Lee on 3/4/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp
#define MAX_BULLETS 30

#include <stdio.h>
#include <vector>
#include "SheetSprite.hpp"
#include "Bullet.hpp"

class Player : public SheetSprite {
    std::vector<Bullet> bullets;
    
public:
    Player(unsigned int textureID, float u, float v, float width, float height, float size);
    void update(float& lastFrameTicks, float& elapsed, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram& program, GLuint textureID, Entity& ent, int player);
    void shootBullet();
};

#endif /* Player_hpp */
