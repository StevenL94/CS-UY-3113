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
#include "ShaderProgram.h"
#include "Matrix.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "Final Project.app/Contents/Resources/"
#endif

#define LEVEL_HEIGHT 100
#define LEVEL_WIDTH 100
#define TILE_SIZE 16

unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
int mapHeight, mapWidth;

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
    displayWindow = SDL_CreateWindow("Final Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
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
    Mix_Quit();
    SDL_Quit();
}

void processEvents(Matrix& projectionMatrix, Matrix& modelMatrix, Matrix& viewMatrix, ShaderProgram& program, GLuint& textureID) {
    
}

bool readHeader(ifstream &stream) {
    string line;
    mapWidth = -1;
    mapHeight = -1;
    while(getline(stream, line)) {
        if(line == "")
            break;
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "width") {
            mapWidth = atoi(value.c_str());
        }
        else if(key == "height"){
            mapHeight = atoi(value.c_str());
        }
    }
    if (mapWidth == -1 || mapHeight == -1) {
        return false;
    }
    else { // allocate our map data
//        levelData = new unsigned char*[mapHeight];
        for(int i = 0; i < mapHeight; ++i) {
//            levelData[i] = new unsigned char[mapWidth];
        }
        return true;
    }
}

bool readLayerData(std::ifstream &stream) {
    string line;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "data") {
            for(int y=0; y < mapHeight; y++) {
                getline(stream, line);
                istringstream lineStream(line);
                string tile;
                for(int x=0; x < mapWidth; x++) {
                    getline(lineStream, tile, ',');
                    unsigned char val =  (unsigned char)atoi(tile.c_str());
                    if(val > 0) {
                        // be careful, the tiles in this format are indexed from 1 not 0
                        levelData[y][x] = val-1;
                    } else {
                        levelData[y][x] = 0;
                    }
                }
            } }
    }
    return true;
}

bool readEntityData(std::ifstream &stream) {
    string line;
    string type;
    while(getline(stream, line)) {
        if(line == "") { break; }
        istringstream sStream(line);
        string key,value;
        getline(sStream, key, '=');
        getline(sStream, value);
        if(key == "type") {
            type = value;
        }
        else if(key == "location") {
            istringstream lineStream(value);
            string xPosition, yPosition;
            getline(lineStream, xPosition, ',');
            getline(lineStream, yPosition, ',');
            float placeX = atoi(xPosition.c_str())/16*TILE_SIZE;
            float placeY = atoi(yPosition.c_str())/16*-TILE_SIZE;
//            placeEntity(type, placeX, placeY);
        }
    }
    return true;
}

void readFile(const string& filename) {
    ifstream infile(filename);
    string line;
    while (getline(infile, line)) {
        if(line == "[header]") {
            if(!readHeader(infile)) {
                return;
            }
        } else if(line == "[layer]") {
            readLayerData(infile);
        } else if(line == "[ObjectsLayer]") {
            readEntityData(infile);
        }
    }
}

int main(int argc, char *argv[]) {
    init();
    ShaderProgram program(RESOURCE_FOLDER "vertex_textured.glsl", RESOURCE_FOLDER "fragment_textured.glsl");
    glUseProgram(program.programID);
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 );
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;
    float lastFrameTicks = 0.0f;
    enum GameState {STATE_SPLASH, STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_PAUSE};
    int state;
//    GLuint spriteSheetTexture = LoadTexture("space_invaders_sprite_sheet_by_gooperblooper22.png");
//    GLuint white = LoadTexture("white.png");
//    GLuint textTexture = LoadTexture("font1.png");
//    Mix_Music *bgm = Mix_LoadMUS("bgm.mp3");
//    Mix_Chunk *missile = Mix_LoadWAV("157439__nengisuls__misslie-1.wav");
    bool pressed = false;
    int score = 0;
    int currentIndex = 0;
    const int numFrames = 2;
    float animation[] = {(74.0f/617.0f),(107.0f/617.0f)};
    float animationElapsed = 0.0f;
    float framesPerSecond = 0.0004f;
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
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
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
        
        score = ticks;
        
        glClear(GL_COLOR_BUFFER_BIT);
        switch (state) {
            case STATE_SPLASH:
                
                break;
                
            case STATE_MAIN_MENU:
//                menu.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, true);
//                menu.draw(program);
//                if (!Mix_PlayingMusic()) {
//                    Mix_PlayMusic(bgm, -1);
//                }
//                if (keys[SDL_SCANCODE_RETURN]) {
//                    state++;
//                }
                break;
                
            case STATE_GAME_LEVEL:
//                text.DrawText(program, textTexture, "Score:" + std::to_string(score), 0.075, 0, -0.23, 0.9);
//                cannon.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, white, alien);
//                cannon.draw(program);
//                alien.update(lastFrameTicks, elapsed, projectionMatrix, viewMatrix, program, false);
//                alien.draw(program);
//                if (keys[SDL_SCANCODE_SPACE]) {
//                    Mix_PlayChannel( -1, missile, 0);
//                    Mix_VolumeChunk(missile, 15);
//                }
//                if (keys[SDL_SCANCODE_ESCAPE] && (state == 1)) {
//                    pressed = true;
//                    state++;
//                }
                
                break;
            case STATE_PAUSE:
//                if (Mix_PlayingMusic()) {
//                    Mix_PausedMusic();
//                }
//                text.DrawText(program, textTexture, "Paused", 0.1, 0, -0.23, 0);
//                if (keys[SDL_SCANCODE_ESCAPE] && pressed) {
//                    pressed = false;
//                    state--;
//                }
                break;
        }
        
        SDL_GL_SwapWindow(displayWindow);
    }
    
    cleanup();
    return 0;
}
