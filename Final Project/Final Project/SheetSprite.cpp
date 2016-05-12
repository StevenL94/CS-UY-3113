//
//  SheetSprite.cpp
//  Final Project
//
//  Created by Steven Lee on 3/1/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#include "SheetSprite.hpp"

SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) : textureID(textureID), u(u), v(v), width(width), height(height), size(size) {
}

void SheetSprite::draw(ShaderProgram& program) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    GLfloat texCoords[] = {
        u, v+height,
        u+width, v,
        u, v,
        u+width, v,
        u, v+height,
        u+width, v+height
    };
    float aspect = width / height;
    float vertices[] = {
        -0.5f * size * aspect, -0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, 0.5f * size,
        0.5f * size * aspect, 0.5f * size,
        -0.5f * size * aspect, -0.5f * size ,
        0.5f * size * aspect, -0.5f * size};
    setTexCoords(texCoords, 12);
    setVertices(vertices, 12);
    render(program, textureID);
//    glDisableVertexAttribArray(program.positionAttribute);
//    glDisableVertexAttribArray(program.texCoordAttribute);
}