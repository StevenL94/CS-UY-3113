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
#define RESOURCE_FOLDER "Homework 1.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;

Matrix projectionMatrix;
Matrix modelMatrix;
Matrix viewMatrix;

GLuint LoadTexture(const char *image_path) {
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

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Homework 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glViewport(0, 0, 800, 600);
    GLuint emoji = LoadTexture("emoji.png");
    GLuint alien = LoadTexture("p1_front.png");
    GLuint medal = LoadTexture("flat_medal6.png");
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(program.programID);
    
    float lastFrameTicks = 0.0f;
    float angle = 0.0f;
    float X,Y,moveX, moveY = 0.0;
    
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
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        angle += elapsed;
        if (X < .35 && Y < .35) {
            X += elapsed/2;
            Y += elapsed/2;
        }
        else if (X < -.35 && Y < -.35) {
            X -= elapsed/2;
            Y -= elapsed/2;
        }
        
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
//        glActiveTexture(GL_TEXTURE1);
        modelMatrix.identity();
        modelMatrix.Scale(.2, .2, 1.0);
        if(keys[SDL_SCANCODE_UP]) {
            moveY += 1/elapsed;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            moveY -= 1/elapsed;
        }
        else if(keys[SDL_SCANCODE_RIGHT]) {
            moveX += 1/elapsed;
        }
        else if(keys[SDL_SCANCODE_LEFT]) {
            moveX -= 1/elapsed;
        }
        modelMatrix.Translate(moveX, moveY, 0);
        program.setModelMatrix(modelMatrix);
        glBindTexture(GL_TEXTURE_2D, alien);
        float vertices1[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        float texCoords1[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
        glEnableVertexAttribArray(program.positionAttribute);
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
        glEnableVertexAttribArray(program.texCoordAttribute);
        glEnable(GL_TEXTURE_2D);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
//        glActiveTexture(GL_TEXTURE2);
        modelMatrix.identity();
        modelMatrix.Scale(.05, .05, 1.0);
        modelMatrix.Translate(0, -.95, 0);
        if(keys[SDL_SCANCODE_UP]) {
            moveY += 1/elapsed;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            moveY -= 1/elapsed;
        }
        else if(keys[SDL_SCANCODE_RIGHT]) {
            moveX += 1/elapsed;
        }
        else if(keys[SDL_SCANCODE_LEFT]) {
            moveX -= 1/elapsed;
        }
        modelMatrix.Translate(moveX, moveY, 0);
        program.setModelMatrix(modelMatrix);
        glBindTexture(GL_TEXTURE_2D, medal);
        float vertices2[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        float texCoords2[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
        glEnableVertexAttribArray(program.positionAttribute);
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
        glEnableVertexAttribArray(program.texCoordAttribute);
        glEnable(GL_TEXTURE_2D);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        //        glActiveTexture(GL_TEXTURE0);
        modelMatrix.identity();
        modelMatrix.Scale(X*2, Y*2, 1.0);
        modelMatrix.Rotate((angle*100.0f) * M_PI/180);
        if(keys[SDL_SCANCODE_SPACE]) {
            modelMatrix.identity();
            modelMatrix.Scale(X*2, Y*2, 1.0);
        }
        modelMatrix.Translate(X*1.5, Y*1.5, 0);
        program.setModelMatrix(modelMatrix);
        glBindTexture(GL_TEXTURE_2D, emoji);
        float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
        float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
        glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
        glEnableVertexAttribArray(program.positionAttribute);
        glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
        glEnableVertexAttribArray(program.texCoordAttribute);
        glEnable(GL_TEXTURE_2D);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisableVertexAttribArray(program.positionAttribute);
        glDisableVertexAttribArray(program.texCoordAttribute);
        
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    SDL_Quit();
    return 0;
}
