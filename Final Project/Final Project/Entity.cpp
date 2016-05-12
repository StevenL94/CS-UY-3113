//
//  Entity.cpp
//  Final Project
//
//  Created by Steven Lee on 3/1/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#include "Entity.hpp"

void Entity::update(float& lastFrameTicks, float& elapsed, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram& program, bool stat) {
    //    Update modelMatrix
    Matrix modelMatrix;
    static bool wall = false;
    if (stat) {
        modelMatrix.identity();
        modelMatrix.Scale(0.5, 2, 0);
        program.setModelMatrix(modelMatrix);
    }
    else {
        if (y >= -0.65) {
            if (x > -1.85 && !wall) {
                x -= elapsed/2;
            }
            if (x <= -1.85) {
                wall = true;
                y -= elapsed*2;
            }
            if (x < 1.85 && wall) {
                x += elapsed/2;
            }
            if (x >= 1.85) {
                wall = false;
                y -= elapsed*4;
            }
        }
        modelMatrix.identity();
        modelMatrix.Scale(0.5, 2, 0);
        modelMatrix.Translate(x, y+0.4, 0);
        program.setModelMatrix(modelMatrix);
    }
}

void Entity::render(ShaderProgram& program, GLuint textureID) {
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Entity::setVertices(float arr[], size_t size) {
    if (size <= 12) {
        for (int i = 0; i < 12; i++) {
            vertices[i] = arr[i];
        }
    }
}

void Entity::setTexCoords(float arr[], size_t size) {
    if (size <= 12) {
        for (int i = 0; i < 12; i++) {
            texCoords[i] = arr[i];
        }
    }
}

void Entity::DrawText(ShaderProgram& program, int fontTexture, std::string text, float size, float spacing, float t_x, float t_y) {
    float texture_size = 1.0/16.0f;
    std::vector<float> vertexData;
    std::vector<float> texCoordData;
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i) + (-0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (0.5f * size), -0.5f * size,
            ((size+spacing) * i) + (0.5f * size), 0.5f * size,
            ((size+spacing) * i) + (-0.5f * size), -0.5f * size,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }
    modelMatrix.Translate(t_x, t_y, 0);
    program.setModelMatrix(modelMatrix);
    glUseProgram(program.programID);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program.texCoordAttribute);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);
}