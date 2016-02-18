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
    static float angle, leftY, rightY, ballX, ballY, padLeftBtm, padRightBtm, padLeftTop, padRightTop, ballBtm, ballTop = 0.0f;
    static bool collision = false;
    static bool newGame = true;
    float rightX = 0.85;
    float leftX = -0.85;
    float height = 0.8;
    float width = 0.1;
    float ballWidth = 0.1;
    angle += elapsed;
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (paddle) {
        if (padNum == 1) {
            if(keys[SDL_SCANCODE_W]) {
                if (leftY < .5) {
                    leftY += 1/elapsed;
                }
            }
            else if (keys[SDL_SCANCODE_S]) {
                if (leftY > -.5) {
                    leftY -= 1/elapsed;
                }
            }
            modelMatrix.identity();
            modelMatrix.Translate(0, leftY, 0);
        }
        else if (padNum == 2){
            if(keys[SDL_SCANCODE_UP]) {
                if (rightY < .5) {
                    rightY += 1/elapsed;
                }
            }
            else if (keys[SDL_SCANCODE_DOWN]) {
                if (rightY > -.5) {
                    rightY -= 1/elapsed;
                }
            }
            modelMatrix.identity();
            modelMatrix.Translate(0, rightY, 0);
        }
    }
    padLeftTop = leftY + height/2;
    padLeftBtm = leftY - height/2;
    padRightTop = rightY + height/2;
    padRightBtm = rightY - height/2;
    ballTop = ballY + ballWidth/2;
    ballBtm = ballY - ballWidth/2;
    
    if (ball) {
        if (newGame) {
            if (!collision) {
                ballX += (elapsed/1500);
                if (ballX >= (rightX - width/2)) {
                    if ((ballBtm <= padRightTop) && (ballTop >= padRightBtm) && ((ballX - ballWidth/2) <= ((rightX + width/2)))  && ((ballX + ballWidth/2) >= ((rightX - width/2)))) {
                        ballY += elapsed/1500;
                        collision = true;
                    }
                    else {
                        modelMatrix.Translate(ballX, ballY, 0);
                        if (ballX < -1 || ballX > 1) {
                            newGame = false;
                        }
                        else if (ballY < -1 || ballY > 1 ) {
                            ballY -= elapsed/1500;
                            collision = true;
                        }
                    }
                }
                else {
                    modelMatrix.Translate(ballX, ballY, 0);
                }
            }
            else {
                ballX -= (elapsed/1500);
                if (ballX < (leftX + width/2)) {
                    if ((ballBtm <= padLeftTop) && (ballTop >= padLeftBtm) && ((ballX - ballWidth/2) <= ((leftX + width/2)))  && ((ballX + ballWidth/2) >= ((leftX - width/2)))) {
                        ballY -= elapsed/1500;
                        collision = false;
                    }
                    else {
                        modelMatrix.Translate(ballX, 0, 0);
                        if (ballX < -1 || ballX > 1) {
                            newGame = false;
                        }
                        else if (ballY < -1 || ballY > 1 ) {
                            ballY -= elapsed/1500;
                            collision = false;
                        }
                    }
                }
                else {
                    modelMatrix.Translate(ballX, 0, 0);
                }
            }
        }
        else {
            if(keys[SDL_SCANCODE_SPACE]) {
                modelMatrix.identity();
                ballX = 0;
                ballY = 0;
                leftX = 0;
                leftY = 0;
                rightX = 0;
                rightY = 0;
                newGame = true;
            }
        }
    }
    program.setModelMatrix(modelMatrix);
}

void render(ShaderProgram& program,float vertices[], float texCoords[]) {
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
    GLuint white = LoadTexture("white.png");
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
//        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//        Ball
        update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, false, true);
        render(program, vertices, texCoords);
//        Left Paddle
        update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true, false, 1);
        render(program, vertices1, texCoords1);
//        Right Paddle
        update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true, false, 2);
        render(program, vertices2, texCoords2);
        SDL_GL_SwapWindow(displayWindow);
    }
    cleanup();
    return 0;
}
