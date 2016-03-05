//
//  Player.cpp
//  Homework 3
//
//  Created by Steven Lee on 3/4/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#include "Player.hpp"

Player::Player(unsigned int textureID, float u, float v, float width, float height, float size) : SheetSprite(textureID, u, v, width, height, size) {
}

void Player::update(float &lastFrameTicks, float &elapsed, Matrix &projectionMatrix, Matrix &viewMatrix, ShaderProgram &program, GLuint textureID) {
    //    Update modelMatrix
    Matrix modelMatrix;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_A]) {
        if (x > -1.85) {
            x -= elapsed*2;
        }
    }
    if(keys[SDL_SCANCODE_D]) {
        if (x < 1.85) {
            x += elapsed*2;
        }
    }
//    shootBullet();
//    for (Bullet b : bullets) {
//        b.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program);
//        b.render(program, textureID);
//    }
    if (keys[SDL_SCANCODE_SPACE]) {
        static Bullet bullet(x/2);
        if (bullet.displayBullet(1.6)) {
            bullet = *new Bullet(x/2);
        }
        bullet.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program);
        bullet.render(program, textureID);
    }
    modelMatrix.identity();
    modelMatrix.Scale(0.5, 2, 0);
    modelMatrix.Translate(x, -0.4, 0);
    program.setModelMatrix(modelMatrix);
}

void Player::shootBullet() {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE]) {
        Bullet bullet(x);
        bullets.push_back(bullet);
    }
//    Bullet newBullet;
//    newBullet.x = -1.2;
//    newBullet.y = 0.0;
//    newBullet.angle = (float)(45 - (rand() % 90));
//    newBullet.speed = 2.0;
//    bullets.push_back(newBullet);
}