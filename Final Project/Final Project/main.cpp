//
//  main.cpp
//  Final Project
//
//  Created by Steven Lee on 3/28/16.
//  Copyright © 2016 Steven Lee. All rights reserved.
//

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <math.h>
#include <vector>
#include <string>

#include "ShaderProgram.h"
#include "Matrix.h"
#include "PerlinNoise.h"
#include "SheetSprite.hpp"
#include "Bullet.hpp"
#include "Player.hpp"

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Final Project.app/Contents/Resources/"
#endif

SDL_Window* displayWindow;
enum GameState {STATE_SPLASH, STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_PAUSE};

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
    displayWindow = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(0, 0, 800, 600);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cleanup() {
    Mix_Quit();
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

    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    SDL_Event event;
    bool done = false;
    
/*    Sprites courtesy of http://gooperblooper22.deviantart.com/art/Space-Invaders-Sprite-Sheet-135338373  */
    GLuint spriteSheetTexture = LoadTexture("space_invaders_sprite_sheet_by_gooperblooper22.png");
    GLuint white = LoadTexture("white.png");
    GLuint textTexture = LoadTexture("font1.png");
    Mix_Music *bgm = Mix_LoadMUS("bgm.mp3");
    Mix_Chunk *missile = Mix_LoadWAV("157439__nengisuls__misslie-1.wav");
    bool pressed = false;
    int score = 0;
    int currentIndex = 0;
    int state;
    const int numFrames = 2;
    float animation[] = {(387.0f/617.0f),(491.0f/617.0f)};
    float animationElapsed = 0.0f;
    float framesPerSecond = 0.0004f;
    std::vector<SheetSprite> aliens;
    
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
        
        animationElapsed += 1/elapsed;
        if(animationElapsed > 1.0/framesPerSecond) {
            currentIndex++;
            animationElapsed = 0.0;
            if(currentIndex > numFrames-1) {
                currentIndex = 0;
            }
        }
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        
        score = ticks;
        SheetSprite menu(spriteSheetTexture, 160.0f/617.0f, 1.0f/2035.0f, 250.0f/617.0f, 171.0f/2035.0f, 0.5);
        SheetSprite splash(spriteSheetTexture, 419.0f/617.0f, 12.0f/2035.0f, 187.0f/617.0f, 163.0f/2035.0f, 0.5);
        Entity text;
        Player cannon(spriteSheetTexture, 355.0f/617.0f, 1163.0f/2035.0f, 104.0f/617.0f, 64.0f/2035.0f, 0.05);
        Player cannon2(spriteSheetTexture, 355.0f/617.0f, 1163.0f/2035.0f, 104.0f/617.0f, 64.0f/2035.0f, 0.05);
        SheetSprite alien(spriteSheetTexture, animation[currentIndex], (581.0f/2035.0f), 88.0f/617.0f, 64.0f/2035.0f, 0.05);
        
//        Fullscreen
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        SDL_ShowCursor(0);
        
        if (keys[SDL_SCANCODE_LGUI] && keys[SDL_SCANCODE_F]) {
            SDL_SetWindowFullscreen(displayWindow, SDL_WINDOW_FULLSCREEN);
        }
        
        switch (state) {
            case STATE_SPLASH:
                splash.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true);
                splash.draw(program);
                if (ticks > 3.0f || keys[SDL_SCANCODE_SPACE]) {
                    state++;
                }
                break;
                
            case STATE_MAIN_MENU:
                menu.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true);
                menu.draw(program);
                if (!Mix_PlayingMusic()) {
                    Mix_PlayMusic(bgm, -1);
                }
                if (keys[SDL_SCANCODE_RETURN]) {
                    state++;
                }
                break;
                
            case STATE_GAME_LEVEL:
                if (Mix_PausedMusic()) {
                    Mix_ResumeMusic();
                }
                text.DrawText(program, textTexture, "Score:" + std::to_string(score), 0.075, 0, -0.23, 0.9);
                cannon.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, white, alien, 1);
                cannon.draw(program);
                cannon2.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, white, alien, 2);
                cannon2.draw(program);
                alien.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, false);
                alien.draw(program);
                if (keys[SDL_SCANCODE_SPACE] || keys[SDL_SCANCODE_UP]) {
                    Mix_PlayChannel( -1, missile, 0);
                    Mix_VolumeChunk(missile, 15);
                }
                if (keys[SDL_SCANCODE_ESCAPE]) {
                    state++;
                }
                break;
                
            case STATE_PAUSE:
                if (Mix_PlayingMusic()) {
                    Mix_PauseMusic();
                }
                score = score;
                text.DrawText(program, textTexture, "Paused", 0.1, 0, -0.23, 0);
                text.DrawText(program, textTexture, "Press Escape to Exit", 0.09, 0, -0.63, -0.8);
                if (keys[SDL_SCANCODE_RETURN]) {
                    state--;
                }
//                Debounce Key
                if (pressed) {
                    done = true;
                }
                if (keys[SDL_SCANCODE_ESCAPE]) {
                    int time = 0;
                    time += ticks;
                    if ((ticks - time) > 0.5f) {
                        pressed = true;
                    }
                }
                break;
        }
        SDL_GL_SwapWindow(displayWindow);
    }
    Mix_FreeMusic(bgm);
    Mix_FreeChunk(missile);
    cleanup();
    return 0;
}
