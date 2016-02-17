//
//  main.cpp
//  Homework 1
//
//  Created by Steven Lee on 2/8/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include <math.h>
#include "ShaderProgram.h"
#include "Matrix.h"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Homework 2.app/Contents/Resources/"
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
    displayWindow = SDL_CreateWindow("Homework 2 - Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 640, 480);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void update(float& lastFrameTicks, float& elapsed, Matrix& projectionMatrix, Matrix& viewMatrix, ShaderProgram& program, bool paddle, bool ball, int padNum = 0) {
//    Update modelMatrix
    Matrix modelMatrix;
    static float angle, X, Y = 0.0f;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (paddle) {
        if (padNum == 1) {
            if(keys[SDL_SCANCODE_W]) {
                if (Y < .5) {
                    Y += 1/elapsed;
                }
            }
            else if (keys[SDL_SCANCODE_S]) {
                if (Y > -.5) {
                    Y -= 1/elapsed;
                }
            }
            modelMatrix.identity();
            modelMatrix.Translate(X, Y, 0);
            program.setModelMatrix(modelMatrix);
        }
        if (padNum == 2) {
            if(keys[SDL_SCANCODE_UP]) {
                if (Y < .5) {
                    Y += 1/elapsed;
                }
            }
            else if (keys[SDL_SCANCODE_DOWN]) {
                if (Y > -.5) {
                    Y -= 1/elapsed;
                }
            }
        }
        modelMatrix.identity();
        modelMatrix.Translate(X, Y, 0);
        program.setModelMatrix(modelMatrix);
    }
    else {
        if(keys[SDL_SCANCODE_SPACE]) {
            modelMatrix.identity();
        }
        program.setModelMatrix(modelMatrix);
    }
}

void render(ShaderProgram& program,float vertices[], float texCoords[]) {
//    glBindTexture(GL_TEXTURE_2D, textureID);
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
//    GLuint pong = LoadTexture("pong.png");
//    GLuint pong1 = LoadTexture("pong.png");
//    GLuint pong2 = LoadTexture("pong.png");
    float lastFrameTicks = 0.0f;
    //    Ball
    float vertices[] = {-0.05, -0.05, 0.05, -0.05, 0.05, 0.05, -0.05, -0.05, 0.05, 0.05, -0.05, 0.05};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    //    Left Paddle
    float vertices1[] = {-0.9, -0.4, -0.8, -0.4, -0.8, 0.4, -0.9, -0.4, -0.8, 0.4, -0.9, 0.4};
    float texCoords1[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    //    Right Paddle
    float vertices2[] = {0.9, -0.4, 0.8, -0.4, 0.8, 0.4, 0.9, -0.4, 0.8, 0.4, 0.9, 0.4};
    float texCoords2[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    SDL_Event event;
    bool done = false;
    
    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
                done = true;
            }
        }
        program.setProjectionMatrix(projectionMatrix);
        program.setModelMatrix(modelMatrix);
        program.setViewMatrix(viewMatrix);
        
        float ticks = (float)SDL_GetTicks();
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, false, true);
        render(program, vertices, texCoords);
        update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true, false, 1);
        render(program, vertices1, texCoords1);
        update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true, false, 2);
        render(program, vertices2, texCoords2);
        SDL_GL_SwapWindow(displayWindow);
    }
    cleanup();
    return 0;
}
