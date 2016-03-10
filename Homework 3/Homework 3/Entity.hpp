//
//  Entity.hpp
//  Homework 3
//
//  Created by Steven Lee on 3/1/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifndef Entity_hpp
#define Entity_hpp

#include <stdio.h>
#include <vector>
#include <SDL.h>
#include "Matrix.h"
#include "ShaderProgram.h"

class Entity {
public:
    Matrix modelMatrix;
    float x, y;
    float vertices[12];
    float texCoords[12];
    bool display;
    void update(float& lastFrameTicks, float& elapsed, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram& program, bool stat);
    void render(ShaderProgram& program, GLuint textureID);
    void setVertices(float arr[], size_t size);
    void setTexCoords(float arr[], size_t size);
    void DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing, float t_x, float t_y);
};

#endif /* Entity_hpp */
