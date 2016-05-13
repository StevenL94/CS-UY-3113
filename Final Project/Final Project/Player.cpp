//
//  Player.cpp
//  Final Project
//
//  Created by Steven Lee on 3/4/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#include "Player.hpp"

Player::Player(unsigned int textureID, float u, float v, float width, float height, float size) : SheetSprite(textureID, u, v, width, height, size) {
}

void Player::update(float &lastFrameTicks, float &elapsed, Matrix &projectionMatrix, Matrix &viewMatrix, ShaderProgram &program, GLuint textureID, Entity& ent, int player, int& score) {
    //    Update modelMatrix
    Matrix modelMatrix;
    static Bullet bullet(x/2);
    static bool pressed = false;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (player == 1) {
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
    }
    else {
        if (keys[SDL_SCANCODE_LEFT]) {
            if (x > -1.85) {
                x -= elapsed*2;
            }
        }
        if(keys[SDL_SCANCODE_RIGHT]) {
            if (x < 1.85) {
                x += elapsed*2;
            }
        }
    }
    
//    shootBullet();
//    for (Bullet b : bullets) {
//        b.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program);
//        b.render(program, textureID);
//    }
    if (player == 1) {
        if (keys[SDL_SCANCODE_SPACE]) {
            pressed = true;
        }
    }
    else {
        if (keys[SDL_SCANCODE_UP]) {
            pressed = true;
        }
    }
    
    if (pressed) {
        if (bullet.displayBullet(1.6)) {
            pressed = false;
            bullet = *new Bullet(x/2);
        }
        bullet.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program);
        bullet.render(program, textureID);
    }
    if ((ent.y - 0.0786241/2 <= bullet.y + 0.0905/2) && (ent.y + 0.0786241/2 >= bullet.y - 0.0905/2) && ((bullet.x - 0.0103/2) <= ((ent.x + 0.356564/2)))  && ((bullet.x + 0.0103/2) >= ((ent.x - 0.0356564/2)))){
         std::cout << "Target Hit" << std::endl;
        score += 100;
        ent.display = false;
    }
    modelMatrix.identity();
    modelMatrix.Scale(0.5, 2, 0);
    modelMatrix.Translate(x, -0.4, 0);
    program.setModelMatrix(modelMatrix);
}

void Player::shootBullet() {
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE]) {
        Bullet bullet(x/2);
        bullets.push_back(bullet);
    }
//    Bullet newBullet;
//    newBullet.x = -1.2;
//    newBullet.y = 0.0;
//    newBullet.angle = (float)(45 - (rand() % 90));
//    newBullet.speed = 2.0;
//    bullets.push_back(newBullet);
}