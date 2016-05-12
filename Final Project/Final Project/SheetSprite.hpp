//
//  SheetSprite.hpp
//  Final Project
//
//  Created by Steven Lee on 3/1/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifndef SheetSprite_hpp
#define SheetSprite_hpp

#include <stdio.h>
#include "Entity.hpp"

class SheetSprite : public Entity {
public:
    float size, u, v, width, height;
    unsigned textureID;
//    SheetSprite();
    SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size);
    void draw(ShaderProgram& program);
};

#endif /* SheetSprite_hpp */
