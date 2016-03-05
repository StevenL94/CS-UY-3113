//
//  main.cpp
//  Homework 3
//
//  Created by Steven Lee on 3/1/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <math.h>
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include "Matrix.h"
#include "SheetSprite.hpp"
#include "Bullet.hpp"
#include "Player.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Homework 3.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path) {
//    Load texture from string and return textureID
    SDL_Surface *surface = IMG_Load(image_path);
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    #ifdef __APPLE__
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
    #else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    #endif
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

void init() {
//    Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Homework 3 - Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cleanup() {
    SDL_Quit();
}

void processEvents(Matrix& projectionMatrix, Matrix& modelMatrix, Matrix& viewMatrix, ShaderProgram& program, GLuint& textureID) {
    
}

int main(int argc, char *argv[]) {
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    
    init();
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glUseProgram(program.programID);

    float lastFrameTicks = 0.0f;

//    float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
//    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
//    float vertices[] = {-0.09, -0.09, 0.09, -0.09, 0.09, 0.09, -0.09, -0.09, 0.09, 0.09, -0.09, 0.09};
    float vertices[] = {-0.09, -0.9, 0.09, -0.9, 0.09, -0.72, -0.09, -0.9, 0.09, -0.72, -0.09, -0.72};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    SDL_Event event;
    bool done = false;
    
//    GLuint spriteSheetTexture = LoadTexture("characters_3.png");
    GLuint spriteSheetTexture = LoadTexture("space_invaders_sprite_sheet_by_gooperblooper22.png");
    GLuint white = LoadTexture("white.png");
    GLuint textTexture = LoadTexture("font1.png");
    int score = 0;
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        program.setProjectionMatrix(projectionMatrix);
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        
        float ticks = (float)SDL_GetTicks()/1000;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        glClear(GL_COLOR_BUFFER_BIT);
//        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        
        score = ticks;
        
        Entity text;
//        Player cannon;
//        Player alien2;
        text.DrawText(program, textTexture, "Score:" + std::to_string(score), 0.075, 0);
//        Bullet bullet;
//        bullet.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program);
//        bullet.render(program, white);
        Player cannon(spriteSheetTexture, 277.0f/617.0f, 227.0f/2035.0f, 26.0f/617.0f, 17.0f/2035.0f, 0.05);
        cannon.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, white);
        cannon.draw(program);
//        cannon.setTexCoords(texCoords, 12);
//        cannon.setVertices(vertices, 12);
//        cannon.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, white);
//        cannon.render(program, spriteSheetTexture);
//        SheetSprite sprite(spriteSheetTexture, u, v, spriteWidth, spriteHeight, 0.2);
//        sprite.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program);
//        sprite.render(program);
        SDL_GL_SwapWindow(displayWindow);
    }
    cleanup();
    return 0;
}
